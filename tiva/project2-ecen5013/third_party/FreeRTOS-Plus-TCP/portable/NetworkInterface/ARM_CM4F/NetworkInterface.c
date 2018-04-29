/*
 * FreeRTOS+TCP V2.0.0
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software. If you wish to use our Amazon
 * FreeRTOS name, please do so in a fair use way that does not cause confusion.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* Standard includes. */
#include <stdint.h>
#include <stdbool.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_UDP_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_IP_Private.h"
#include "NetworkBufferManagement.h"

#include "driver/ethernet.h"

#include "driver/leds.h"

/* Demo includes. */
#include "NetworkInterface.h"

extern QueueHandle_t xPingReplyQueue;
uint32_t ip;

/* The queue used to communicate Ethernet events with the IP task. */
extern QueueHandle_t xNetworkEventQueue;

/* The semaphore used to wake the deferred interrupt handler task when an Rx
interrupt is received. */

SemaphoreHandle_t xEMACRxEventSemaphore;


/*
 * A deferred interrupt handler task that processes
 */

void vEMACHandlerTask( void *pvParameters )
{

    UARTprintf("\n\nIn deffered task handler\n");

    size_t xDataLength,received_size;
    const uint16_t usCRCLength = 4;
    NetworkBufferDescriptor_t *pxNetworkBuffer;
    IPStackEvent_t xRxEvent = { eNetworkRxEvent, NULL };


    ( void ) pvParameters;
    configASSERT( xEMACRxEventSemaphore );

    while(1)
    {
        // Wait for the EMAC interrupt to indicate that another packet has been
        //received.  The while() loop is only needed if INCLUDE_vTaskSuspend is
        //set to 0 in FreeRTOSConfig.h.
        while( xSemaphoreTake( xEMACRxEventSemaphore, portMAX_DELAY ) == pdFALSE );

        // At least one packet has been received.

           //
          // Make sure that we own the receive descriptor.
          //
    if(!(g_psRxDescriptor[g_ui32RxDescIndex].ui32CtrlStatus & DES0_RX_CTRL_OWN))
    {

        //
        // We own the receive descriptor so check to see if it contains a valid
        // frame.
        //
        if(!(g_psRxDescriptor[g_ui32RxDescIndex].ui32CtrlStatus &
        DES0_RX_STAT_ERR))
        {

            // Obtain the length, minus the CRC.  The CRC is four bytes
            //but the length is already minus 1.
            received_size=
                    ((g_psRxDescriptor[g_ui32RxDescIndex].ui32CtrlStatus &
                    DES0_RX_STAT_FRAME_LENGTH_M) >>
                    DES0_RX_STAT_FRAME_LENGTH_S);

            xDataLength = ( size_t ) received_size - ( usCRCLength - 1U );

            if( xDataLength > 0U )
            {
                // Obtain a network buffer to pass this data into the
                //stack.  No storage is required as the network buffer
                //will point directly to the buffer that already holds
                //the received data.
                pxNetworkBuffer = pxGetNetworkBufferWithDescriptor( 0, ( TickType_t ) 0 );

                if( pxNetworkBuffer != NULL )
                {
                    pxNetworkBuffer->pucEthernetBuffer = g_psRxDescriptor[g_ui32RxDescIndex].pvBuffer1;
                    pxNetworkBuffer->xDataLength = xDataLength;
                    xRxEvent.pvData = ( void * ) pxNetworkBuffer;

                    // Data was received and stored.  Send a message to the IP
                    //task to let it know.
                    //if( xSendEventStructToIPTask( &xRxEvent, ( TickType_t ) 0 ) == pdFAIL )
                    if( xSendEventToIPTask(&xRxEvent) == pdFAIL )
                    {
                        vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
                        iptraceETHERNET_RX_EVENT_LOST();
                    }
                }
               /* else
                {
                    iptraceETHERNET_RX_EVENT_LOST();
                }

                iptraceNETWORK_INTERFACE_RECEIVE();*/
            }

            //
            // Now that we are finished dealing with this descriptor, hand
            // it back to the hardware. Note that we assume
            // ApplicationProcessFrame() is finished with the buffer at this point
            // so it is safe to reuse.
            //
            g_psRxDescriptor[g_ui32RxDescIndex].ui32CtrlStatus =
            DES0_RX_CTRL_OWN;
            //
            // Move on to the next descriptor in the chain.
            //
            g_ui32RxDescIndex++;

            if(g_ui32RxDescIndex == NUM_RX_DESCRIPTORS)
            {
                g_ui32RxDescIndex = 0;
            }
        }
       }

    }


}



//*****************************************************************************
//
// The interrupt handler for the Ethernet interrupt.
//
//*****************************************************************************
void
EthernetIntHandler(void)
{
    uint32_t ui32Temp;
    //
    // Read and Clear the interrupt.
    //
    ui32Temp = EMACIntStatus(EMAC0_BASE, true);
    EMACIntClear(EMAC0_BASE, ui32Temp);
    //
    // Check to see if an RX Interrupt has occurred.
    //
    if(ui32Temp & EMAC_INT_RECEIVE)
    {
        //
        // Indicate that a packet has been received.
        //Gives sempahore to task that handles deffered processing
        // for TCP Rx packets .

        xSemaphoreGiveFromISR( xEMACRxEventSemaphore, NULL );

    }

    //portEND_SWITCHING_ISR( ulInterruptCause );

}


BaseType_t xNetworkInterfaceInitialise( void )
{

    enable_eth0();
    xEMACRxEventSemaphore=xSemaphoreCreateBinary();

    return pdPASS;
	
}


BaseType_t xNetworkInterfaceOutput( NetworkBufferDescriptor_t * const pxNetworkBuffer, BaseType_t xReleaseAfterSend )
{

    extern int32_t PacketTransmit(uint8_t *pui8Buf, int32_t i32BufLen);

    PacketTransmit(pxNetworkBuffer->pucEthernetBuffer, pxNetworkBuffer->xDataLength);

    UARTprintf("\nxNetIntOut packet length %d \n",pxNetworkBuffer->xDataLength);

    // Finished with the network buffer, release the buffer
    vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
    return pdTRUE;
}


void vNetworkInterfaceAllocateRAMToBuffers( NetworkBufferDescriptor_t pxNetworkBuffers[ ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS ] )
{
    UARTprintf("\vNetworkInterfaceAllocateRAMToBuffers called \n");
}


BaseType_t xGetPhyLinkStatus( void )
{
    UARTprintf("\n xGetPhyLinkStatus called \n");
    return pdPASS;
}


void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent )
{
    char cBuffer[20];
    if(eNetworkEvent==eNetworkUp)
    {
        // The handler task is created at the highest possible priority to
        //ensure the interrupt handler can return directly to it.

        xTaskCreate( vEMACHandlerTask, "EMAC", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, NULL );
        UARTprintf("\n\n Connected to network \n");

        ip=FreeRTOS_GetIPAddress();
        /* Convert the IP address to a string then print it out. */
        FreeRTOS_inet_ntoa( ip, cBuffer );
        printf( "IP Address: %s\r\n", cBuffer );


        UARTprintf("\n IP setup %x\n",ip);

        //ping host machine to check working
        //vSendPing("192.168.0.100");
    }
    else if(eNetworkEvent==eNetworkDown)
        UARTprintf("\n\n Disconnected from network \n");
    else
        UARTprintf("\n\n Unknown Event \n");
}

/* If ipconfigSUPPORT_OUTGOING_PINGS is set to 1 in FreeRTOSIPConfig.h then
vApplicationPingReplyHook() is called by the IP stack when the stack receives a
ping reply. */

extern QueueHandle_t xPingReplyQueue;

void vApplicationPingReplyHook( ePingReplyStatus_t eStatus, uint16_t usIdentifier )
{
    switch( eStatus )
    {
        case eSuccess    :
            UARTprintf("\n\n Successful ping response \n");
            /* A valid ping reply has been received.  Post the sequence number
            on the queue that is read by the vSendPing() function below.  Do
            not wait more than 10ms trying to send the message if it cannot be
            sent immediately because this function is called from the IP stack
            task - blocking in this function will block the IP stack. */
            xQueueSend( xPingReplyQueue, &usIdentifier, 10 / portTICK_PERIOD_MS );
            break;

        case eInvalidChecksum :
            UARTprintf("\n\n Invalid Checksum \n");
            break;
        case eInvalidData :
            UARTprintf("\n\n Invalid Data \n");
            /* A reply was received but it was not valid. */
            break;
    }
}


























#if ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES != 1
	#define ipCONSIDER_FRAME_FOR_PROCESSING( pucEthernetBuffer ) eProcessBuffer
#else
	#define ipCONSIDER_FRAME_FOR_PROCESSING( pucEthernetBuffer ) eConsiderFrameForProcessing( ( pucEthernetBuffer ) )
#endif

/* When a packet is ready to be sent, if it cannot be sent immediately then the
task performing the transmit will block for niTX_BUFFER_FREE_WAIT
milliseconds.  It will do this a maximum of niMAX_TX_ATTEMPTS before giving
up. */
#define niTX_BUFFER_FREE_WAIT	( ( TickType_t ) 2UL / portTICK_PERIOD_MS )
#define niMAX_TX_ATTEMPTS		( 5 )

/* The length of the queue used to send interrupt status words from the
interrupt handler to the deferred handler task. */
#define niINTERRUPT_QUEUE_LENGTH	( 10 )








// test ping function
BaseType_t vSendPing( const char *pcIPAddress )
{
    uint16_t usRequestSequenceNumber, usReplySequenceNumber;
    uint32_t ulIPAddress;

    /* The pcIPAddress parameter holds the destination IP address as a string in
    decimal dot notation (for example, "192.168.0.200").  Convert the string into
    the required 32-bit format. */
    ulIPAddress = FreeRTOS_inet_addr( pcIPAddress );

    /* Send a ping containing 8 data bytes.  Wait (in the Blocked state) a
    maximum of 100ms for a network buffer into which the generated ping request
    can be written and sent. */
    usRequestSequenceNumber = FreeRTOS_SendPingRequest( ulIPAddress, 8, 1000 / portTICK_PERIOD_MS );

    if( usRequestSequenceNumber == pdFAIL )
    {
        /* The ping could not be sent because a network buffer could not be
        obtained within 100ms of FreeRTOS_SendPingRequest() being called. */
        return pdFAIL;
    }
    else
    {
        /* The ping was sent.  Wait 200ms for a reply.  The sequence number from
        each reply is sent from the vApplicationPingReplyHook() on the
        xPingReplyQueue queue (this is not standard behaviour, but implemented in
        the example function above).  It is assumed the queue was created before
        this function was called! */
        if( xQueueReceive( xPingReplyQueue,
                           &usReplySequenceNumber,
                           200 / portTICK_PERIOD_MS ) == pdPASS )
        {
            /* A ping reply was received.  Was it a reply to the ping just sent? */
            if( usRequestSequenceNumber == usReplySequenceNumber )
            {
                /* This was a reply to the request just sent. */

            }
        }
        return pdTRUE;
    }
}

