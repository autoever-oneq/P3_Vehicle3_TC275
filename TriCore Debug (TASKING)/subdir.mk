################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Can.c" \
"../Cpu0_Main.c" \
"../Cpu1_Main.c" \
"../Cpu2_Main.c" \
"../Driver_Adc.c" \
"../Driver_Stm.c" \
"../GTM_ATOM_PWM.c" \
"../PID_CON.c" \
"../STM_Interrupt.c" 

COMPILED_SRCS += \
"Can.src" \
"Cpu0_Main.src" \
"Cpu1_Main.src" \
"Cpu2_Main.src" \
"Driver_Adc.src" \
"Driver_Stm.src" \
"GTM_ATOM_PWM.src" \
"PID_CON.src" \
"STM_Interrupt.src" 

C_DEPS += \
"./Can.d" \
"./Cpu0_Main.d" \
"./Cpu1_Main.d" \
"./Cpu2_Main.d" \
"./Driver_Adc.d" \
"./Driver_Stm.d" \
"./GTM_ATOM_PWM.d" \
"./PID_CON.d" \
"./STM_Interrupt.d" 

OBJS += \
"Can.o" \
"Cpu0_Main.o" \
"Cpu1_Main.o" \
"Cpu2_Main.o" \
"Driver_Adc.o" \
"Driver_Stm.o" \
"GTM_ATOM_PWM.o" \
"PID_CON.o" \
"STM_Interrupt.o" 


# Each subdirectory must supply rules for building sources it contributes
"Can.src":"../Can.c" "subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/3670l/AURIX-v1.10.6-workspace/Motor_GGB_PI_appsche_recode_Final/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Can.o":"Can.src" "subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Cpu0_Main.src":"../Cpu0_Main.c" "subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/3670l/AURIX-v1.10.6-workspace/Motor_GGB_PI_appsche_recode_Final/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Cpu0_Main.o":"Cpu0_Main.src" "subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Cpu1_Main.src":"../Cpu1_Main.c" "subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/3670l/AURIX-v1.10.6-workspace/Motor_GGB_PI_appsche_recode_Final/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Cpu1_Main.o":"Cpu1_Main.src" "subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Cpu2_Main.src":"../Cpu2_Main.c" "subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/3670l/AURIX-v1.10.6-workspace/Motor_GGB_PI_appsche_recode_Final/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Cpu2_Main.o":"Cpu2_Main.src" "subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Driver_Adc.src":"../Driver_Adc.c" "subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/3670l/AURIX-v1.10.6-workspace/Motor_GGB_PI_appsche_recode_Final/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Driver_Adc.o":"Driver_Adc.src" "subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Driver_Stm.src":"../Driver_Stm.c" "subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/3670l/AURIX-v1.10.6-workspace/Motor_GGB_PI_appsche_recode_Final/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Driver_Stm.o":"Driver_Stm.src" "subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"GTM_ATOM_PWM.src":"../GTM_ATOM_PWM.c" "subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/3670l/AURIX-v1.10.6-workspace/Motor_GGB_PI_appsche_recode_Final/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"GTM_ATOM_PWM.o":"GTM_ATOM_PWM.src" "subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"PID_CON.src":"../PID_CON.c" "subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/3670l/AURIX-v1.10.6-workspace/Motor_GGB_PI_appsche_recode_Final/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"PID_CON.o":"PID_CON.src" "subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"STM_Interrupt.src":"../STM_Interrupt.c" "subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/3670l/AURIX-v1.10.6-workspace/Motor_GGB_PI_appsche_recode_Final/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"STM_Interrupt.o":"STM_Interrupt.src" "subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean--2e-

clean--2e-:
	-$(RM) ./Can.d ./Can.o ./Can.src ./Cpu0_Main.d ./Cpu0_Main.o ./Cpu0_Main.src ./Cpu1_Main.d ./Cpu1_Main.o ./Cpu1_Main.src ./Cpu2_Main.d ./Cpu2_Main.o ./Cpu2_Main.src ./Driver_Adc.d ./Driver_Adc.o ./Driver_Adc.src ./Driver_Stm.d ./Driver_Stm.o ./Driver_Stm.src ./GTM_ATOM_PWM.d ./GTM_ATOM_PWM.o ./GTM_ATOM_PWM.src ./PID_CON.d ./PID_CON.o ./PID_CON.src ./STM_Interrupt.d ./STM_Interrupt.o ./STM_Interrupt.src

.PHONY: clean--2e-

