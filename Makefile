phony := all
all:

ifneq ($(MAKE_LIB),1)
TARGET := ./build/rtthread.elf
endif

$(if $(strip $(RTT_ROOT)),,$(error RTT_ROOT not defined))


TARGET_MODULE = rtthread
TARGET_BIN = rtthread.bin

BUILD_PATH := .

export ARM_PREFIX := $(TOOL_PATH)arm-none-eabi-
export GDB := $(ARM_PREFIX)gdb

flash :
	#-scons -j4
	@echo "For st-flash tool, see https://github.com/texane/stlink"
	@echo "cc $(TARGET_BIN)"
	@st-flash --reset --format binary write $(TARGET_BIN) 0x08020000

boot :
	#-scons -j4
	@echo "For st-flash tool, see https://github.com/texane/stlink"
	@echo "cc $(TARGET_BIN)"
	@st-flash --reset --format binary write rtboot_f429.bin 0x08000000


#######################################
# Debug flags
#######################################
OOCD             := openocd
OOCD_INTERFACE   := /usr/share/openocd/scripts/interface/cmsis-dap.cfg
OOCD_TARGET      := /usr/share/openocd/scripts/target/stm32f4x.cfg
PWD 			 := $(shell pwd)

debug: $(BUILD_PATH)/$(TARGET_MODULE).debug
OOCDFLAGS := -f $(OOCD_INTERFACE) -f $(OOCD_TARGET)

%.debug: $(TARGET)
	@printf "  GDB DEBUG $<\n"
	@$(GDB) -iex 'target extended | $(OOCD) $(OOCDFLAGS) -c "gdb_port pipe"' -iex 'monitor reset halt' -ex 'load' -ex '-' -ex 'break main' -ex 'c' $^

download:
	@printf "GD-LINK download $(TARGET)\n"
	@#$(OOCD) $(OOCDFLAGS) -c init -c halt -c "flash write_image erase $(PWD)/$(TARGET)" -c reset -c shutdown
	@$(OOCD) $(OOCDFLAGS) -c init -c halt -c "program $(PWD)/$(TARGET)" -c reset -c shutdown
