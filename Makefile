# Generic Makefile (based on gcc)

# 目标名称
TARGET = bare_metal

# 编译路径
BUILD_DIR = build

# 文件列表
C_SOURCES =  \
	$(wildcard user/*.c) \
	$(wildcard user/hardware/*.c) \
	$(wildcard drivers/MCIMX6Y2/*.c) \
	$(wildcard drivers/MCIMX6Y2/utilities/*.c) \
	drivers/MCIMX6Y2/drivers/fsl_uart.c \
	drivers/MCIMX6Y2/drivers/fsl_clock.c \
	drivers/MCIMX6Y2/drivers/fsl_gpio.c \
	drivers/MCIMX6Y2/drivers/fsl_common.c

C_INCLUDES =  \
	drivers/CMSIS/Include \
	drivers/CORTEXA/Include \
	drivers/MCIMX6Y2 \
	drivers/MCIMX6Y2/drivers \
	drivers/MCIMX6Y2/utilities \
	user/hardware 

ASM_SOURCES = 
ASM_SOURCES_S =  \
	drivers/MCIMX6Y2/gcc/startup_MCIMX6Y2.S

LDSCRIPT = \
	drivers/MCIMX6Y2/gcc/MCIMX6Y2xxx08_ram.ld

LIBDIR = 

# C全局宏定义
C_DEFS =  \
	CPU_MCIMX6Y2CVM08 

# 汇编全局宏定义
AS_DEFS = \
	__ATOLLIC__ \
	__STARTUP_CLEAR_BSS \
	__STARTUP_INITIALIZE_NONCACHEDATA 

# 优化等级 
# -O0 不优化
# -O1/-O -O2 -O3 数字越大优化等级越高
# -Og	 在-O1的基础上去掉影响调试的优化
# -Os	 在-O2的基础上去掉会导致可执行程序增大的优化,可节省存储空间
# -Ofast 在-O3的基础上添加一些非常规优化,会打破一些国际标准,一般不推荐
OPT = -O0

# 编译选项汇总
FLAGS = -march=armv7-a -Wall -mfloat-abi=hard -mfpu=vfpv4 -fno-common -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mthumb -mapcs 

CFLAGS = $(FLAGS) $(OPT) -std=gnu99 
CFLAGS += $(patsubst %, -I%, $(C_INCLUDES))
CFLAGS += $(patsubst %, -D%, $(C_DEFS))

ASFLAGS = $(FLAGS) -std=gnu99 
ASFLAGS += $(patsubst %, -D%, $(AS_DEFS))

LDFLAGS = $(FLAGS) -Xlinker --gc-sections -Xlinker -static -Xlinker -z -Xlinker muldefs --specs=nosys.specs #--specs=nano.specs 
LDFLAGS += -T$(LDSCRIPT) -L$(LIBDIR) -Wl,-Map,$(BUILD_DIR)/$(TARGET).map 
LDFLAGS += -Wl,--start-group -lm -lc -lgcc -lnosys -Wl,--end-group 

# 生成文件依赖关系到.d文件
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# 开启调试信息(不影响hex/bin文件,影响elf文件,不开启不能gdb调试)
# CFLAGS += -g -gdwarf-2

# printf支持浮点数(生成的可执行程序大概会增大8KB)
# LDFLAGS += -lc -lrdimon -u _printf_float

# GCC编译器
PREFIX = C:/gcc-linaro-7.5.0-2019.12-i686-mingw32_arm-eabi/bin/arm-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc #-x assembler-with-cpp
LD = $(PREFIX)gcc
SZ = $(PREFIX)size
CP = $(PREFIX)objcopy

# 最终目标三种格式文件 build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin 

# C目标列表
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# 汇编目标列表
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES_S:.S=.o)))
vpath %.S $(sort $(dir $(ASM_SOURCES_S)))

# 编译C文件
$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) $< -o $@

# 编译汇编文件(.s只汇编 .S先预处理再汇编)
$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@
$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

# 链接生成可执行文件
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile $(LDSCRIPT)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

# 生成BIN文件
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(CP) -O binary $< $@ -j.interrupts -j.text -j.ARM.extab -j.ARM -j.ctors -j.dtors -j.preinit_array -j.init_array -j.fini_array

# 如果没有则创建BUILD目录
$(BUILD_DIR):
	mkdir $@

# 包含依赖文件(保证有头文件修改时make能重新编译)
-include $(wildcard $(BUILD_DIR)/*.d)

# 删除build文件夹
clean:
	-rmdir /S/Q $(BUILD_DIR)


