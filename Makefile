ARMGNU ?= aarch64-linux-gnu

COPS = -fPIC -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude
ASMOPS = -fPIC -Iinclude 

BUILD_DIR = build
SRC_DIR = src

all : kernel8.img

con : connect_to_serial

ul : upload_and_connect

mu : | all ul

clean :
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(COPS) -MMD -c $< -o $@

$(BUILD_DIR)/%_s.o: $(SRC_DIR)/%.S
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(ASMOPS) -MMD -c $< -o $@

C_FILES = $(wildcard $(SRC_DIR)/**/*.c)
C_FILES += $(wildcard $(SRC_DIR)/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)/**/*.S)
ASM_FILES += $(wildcard $(SRC_DIR)/*.S)
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)

DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

kernel8.img: $(SRC_DIR)/linker.ld $(OBJ_FILES)
	$(ARMGNU)-ld -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/kernel8.elf  $(OBJ_FILES)
	$(ARMGNU)-objcopy $(BUILD_DIR)/kernel8.elf -O binary out/kernel8.img

connect_to_serial:	
	sudo screen -DR pi /dev/ttyS3 115200

upload_and_connect:
	python3 scripts/boot_send.py -d /dev/ttyS3 -b 115200 -k out/kernel8.img -i