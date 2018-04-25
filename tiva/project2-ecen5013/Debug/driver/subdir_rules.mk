################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
driver/ethernet.obj: ../driver/ethernet.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/UCBoulder/Acads/Sem_2/5013-002_APES/repos/project2_ecen5013-002/tiva/project2-ecen5013" --include_path="../../Support_files/FreeRTOS" --include_path="../../Support_files/FreeRTOS/Source/include" --include_path="../../Support_files/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="../../Support_files" --include_path="../../Support_files/FreeRTOS-Plus-TCP/include" --include_path="../../Support_files/FreeRTOS-Plus-TCP/portable/Compiler/CCS" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/include" --define=ccs="ccs" --define=PART_TM4C1292NCPDT -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="driver/ethernet.d_raw" --obj_directory="driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driver/leds.obj: ../driver/leds.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/UCBoulder/Acads/Sem_2/5013-002_APES/repos/project2_ecen5013-002/tiva/project2-ecen5013" --include_path="../../Support_files/FreeRTOS" --include_path="../../Support_files/FreeRTOS/Source/include" --include_path="../../Support_files/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="../../Support_files" --include_path="../../Support_files/FreeRTOS-Plus-TCP/include" --include_path="../../Support_files/FreeRTOS-Plus-TCP/portable/Compiler/CCS" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/include" --define=ccs="ccs" --define=PART_TM4C1292NCPDT -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="driver/leds.d_raw" --obj_directory="driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driver/serial_interface.obj: ../driver/serial_interface.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/UCBoulder/Acads/Sem_2/5013-002_APES/repos/project2_ecen5013-002/tiva/project2-ecen5013" --include_path="../../Support_files/FreeRTOS" --include_path="../../Support_files/FreeRTOS/Source/include" --include_path="../../Support_files/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="../../Support_files" --include_path="../../Support_files/FreeRTOS-Plus-TCP/include" --include_path="../../Support_files/FreeRTOS-Plus-TCP/portable/Compiler/CCS" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/include" --define=ccs="ccs" --define=PART_TM4C1292NCPDT -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="driver/serial_interface.d_raw" --obj_directory="driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


