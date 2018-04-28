################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
third_party/FreeRTOS/Source/croutine.obj: ../third_party/FreeRTOS/Source/croutine.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/UCBoulder/Acads/Sem_2/5013-002_APES/repos/project2_ecen5013-002/tiva/project2-ecen5013" --include_path="../../Support_files/FreeRTOS" --include_path="../../Support_files/FreeRTOS/Source/include" --include_path="../../Support_files/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="../../Support_files" --include_path="../../../common" --include_path="../../Support_files/FreeRTOS-Plus-TCP/include" --include_path="../../Support_files/FreeRTOS-Plus-TCP/portable/Compiler/CCS" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/include" --define=ccs="ccs" --define=PART_TM4C1292NCPDT -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="third_party/FreeRTOS/Source/croutine.d_raw" --obj_directory="third_party/FreeRTOS/Source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

third_party/FreeRTOS/Source/event_groups.obj: ../third_party/FreeRTOS/Source/event_groups.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/UCBoulder/Acads/Sem_2/5013-002_APES/repos/project2_ecen5013-002/tiva/project2-ecen5013" --include_path="../../Support_files/FreeRTOS" --include_path="../../Support_files/FreeRTOS/Source/include" --include_path="../../Support_files/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="../../Support_files" --include_path="../../../common" --include_path="../../Support_files/FreeRTOS-Plus-TCP/include" --include_path="../../Support_files/FreeRTOS-Plus-TCP/portable/Compiler/CCS" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/include" --define=ccs="ccs" --define=PART_TM4C1292NCPDT -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="third_party/FreeRTOS/Source/event_groups.d_raw" --obj_directory="third_party/FreeRTOS/Source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

third_party/FreeRTOS/Source/list.obj: ../third_party/FreeRTOS/Source/list.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/UCBoulder/Acads/Sem_2/5013-002_APES/repos/project2_ecen5013-002/tiva/project2-ecen5013" --include_path="../../Support_files/FreeRTOS" --include_path="../../Support_files/FreeRTOS/Source/include" --include_path="../../Support_files/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="../../Support_files" --include_path="../../../common" --include_path="../../Support_files/FreeRTOS-Plus-TCP/include" --include_path="../../Support_files/FreeRTOS-Plus-TCP/portable/Compiler/CCS" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/include" --define=ccs="ccs" --define=PART_TM4C1292NCPDT -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="third_party/FreeRTOS/Source/list.d_raw" --obj_directory="third_party/FreeRTOS/Source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

third_party/FreeRTOS/Source/queue.obj: ../third_party/FreeRTOS/Source/queue.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/UCBoulder/Acads/Sem_2/5013-002_APES/repos/project2_ecen5013-002/tiva/project2-ecen5013" --include_path="../../Support_files/FreeRTOS" --include_path="../../Support_files/FreeRTOS/Source/include" --include_path="../../Support_files/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="../../Support_files" --include_path="../../../common" --include_path="../../Support_files/FreeRTOS-Plus-TCP/include" --include_path="../../Support_files/FreeRTOS-Plus-TCP/portable/Compiler/CCS" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/include" --define=ccs="ccs" --define=PART_TM4C1292NCPDT -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="third_party/FreeRTOS/Source/queue.d_raw" --obj_directory="third_party/FreeRTOS/Source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

third_party/FreeRTOS/Source/tasks.obj: ../third_party/FreeRTOS/Source/tasks.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/UCBoulder/Acads/Sem_2/5013-002_APES/repos/project2_ecen5013-002/tiva/project2-ecen5013" --include_path="../../Support_files/FreeRTOS" --include_path="../../Support_files/FreeRTOS/Source/include" --include_path="../../Support_files/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="../../Support_files" --include_path="../../../common" --include_path="../../Support_files/FreeRTOS-Plus-TCP/include" --include_path="../../Support_files/FreeRTOS-Plus-TCP/portable/Compiler/CCS" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/include" --define=ccs="ccs" --define=PART_TM4C1292NCPDT -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="third_party/FreeRTOS/Source/tasks.d_raw" --obj_directory="third_party/FreeRTOS/Source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

third_party/FreeRTOS/Source/timers.obj: ../third_party/FreeRTOS/Source/timers.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/UCBoulder/Acads/Sem_2/5013-002_APES/repos/project2_ecen5013-002/tiva/project2-ecen5013" --include_path="../../Support_files/FreeRTOS" --include_path="../../Support_files/FreeRTOS/Source/include" --include_path="../../Support_files/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="../../Support_files" --include_path="../../../common" --include_path="../../Support_files/FreeRTOS-Plus-TCP/include" --include_path="../../Support_files/FreeRTOS-Plus-TCP/portable/Compiler/CCS" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/include" --define=ccs="ccs" --define=PART_TM4C1292NCPDT -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="third_party/FreeRTOS/Source/timers.d_raw" --obj_directory="third_party/FreeRTOS/Source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


