################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-386284913: ../c2000.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"D:/CCS/ccs/utils/sysconfig_1.9.0/sysconfig_cli.bat" -s "D:/CCS/C2000Ware_3_04_00_00_Software/.metadata/sdk.json" -d "F2837xD" --script "D:/CCS_workspace/PFC_1.4/empty_sysconfig_337zwt/c2000.syscfg" -o "syscfg" --package F2837xD_337ZWT --part F2837xD_337ZWT --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/board.c: build-386284913 ../c2000.syscfg
syscfg/board.h: build-386284913
syscfg/pinmux.csv: build-386284913
syscfg/: build-386284913

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/CCS/ccs/tools/compiler/ti-cgt-c2000_20.2.5.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="D:/CCS_workspace/PFC_1.4/empty_sysconfig_337zwt" --include_path="D:/CCS_workspace/PFC_1.4/empty_sysconfig_337zwt/device" --include_path="D:/CCS/C2000Ware_3_04_00_00_Software/driverlib/f2837xd/driverlib" --include_path="D:/CCS/ccs/tools/compiler/ti-cgt-c2000_20.2.5.LTS/include" --define=DEBUG --define=CPU1 --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="D:/CCS_workspace/PFC_1.4/empty_sysconfig_337zwt/CPU1_FLASH/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/CCS/ccs/tools/compiler/ti-cgt-c2000_20.2.5.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="D:/CCS_workspace/PFC_1.4/empty_sysconfig_337zwt" --include_path="D:/CCS_workspace/PFC_1.4/empty_sysconfig_337zwt/device" --include_path="D:/CCS/C2000Ware_3_04_00_00_Software/driverlib/f2837xd/driverlib" --include_path="D:/CCS/ccs/tools/compiler/ti-cgt-c2000_20.2.5.LTS/include" --define=DEBUG --define=CPU1 --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="D:/CCS_workspace/PFC_1.4/empty_sysconfig_337zwt/CPU1_FLASH/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


