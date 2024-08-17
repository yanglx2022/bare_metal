/**
 * @brief   : 
 * @author  : yanglx2022
 * @version : V0.1.0
 * @date    : 2024-05-17
 * @note    : 
 */

#include "common.h"
#include "clock_config.h"
#include "led.h"
#include "key.h"

static const mmu_attribute_t s_mmuDevAttr = {.type = MMU_MemoryDevice,
                                             .domain = 0U,
                                             .accessPerm = MMU_AccessRWNA,
                                             .shareable = 0U,
                                             .notSecure = 0U,
                                             .notGlob = 0U,
                                             .notExec = 1U};

static const mmu_attribute_t s_mmuRomAttr = {.type = MMU_MemoryWriteBackWriteAllocate,
                                             .domain = 0U,
                                             .accessPerm = MMU_AccessRORO,
                                             .shareable = 0U,
                                             .notSecure = 0U,
                                             .notGlob = 0U,
                                             .notExec = 0U};

static const mmu_attribute_t s_mmuRamAttr = {.type = MMU_MemoryWriteBackWriteAllocate,
                                             .domain = 0U,
                                             .accessPerm = MMU_AccessRWRW,
                                             .shareable = 0U,
                                             .notSecure = 0U,
                                             .notGlob = 0U,
                                             .notExec = 0U};

static const mmu_attribute_t s_mmuBufferAttr = {.type = MMU_MemoryNonCacheable,
                                                .domain = 0U,
                                                .accessPerm = MMU_AccessRWRW,
                                                .shareable = 0U,
                                                .notSecure = 0U,
                                                .notGlob = 0U,
                                                .notExec = 0U};

static uint32_t MMU_L1Table[4096] __attribute__((section("OcramData"), aligned(16384)));


/* Initialize memory system (MMU). */
static void BOARD_InitMemory(void)
{
    uint32_t i;

    MMU_Init(MMU_L1Table);

    MMU_ConfigSection(MMU_L1Table, (const void *)0x00000000U, 0x00000000U, &s_mmuRomAttr); /* ROM */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x00900000U, 0x00900000U, &s_mmuRamAttr); /* OCRAM */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x00A00000U, 0x00A00000U, &s_mmuDevAttr); /* GIC */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x00B00000U, 0x00B00000U, &s_mmuDevAttr); /* GPV_0 PL301 */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x00C00000U, 0x00C00000U, &s_mmuDevAttr); /* GPV_1 PL301 */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x00D00000U, 0x00D00000U, &s_mmuDevAttr); /* cpu */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x00E00000U, 0x00E00000U, &s_mmuDevAttr); /* per_m */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x01800000U, 0x01800000U, &s_mmuDevAttr); /* APBH DMA */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x02000000U, 0x02000000U, &s_mmuDevAttr); /* AIPS-1 */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x02100000U, 0x02100000U, &s_mmuDevAttr); /* AIPS-2 */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x02200000U, 0x02200000U, &s_mmuDevAttr); /* AIPS-3 */

    for (i = 0; i < 32; i++)
    {
        MMU_ConfigSection(MMU_L1Table, (const void *)(0x0C000000U + (i << 20)), (0x0C000000U + (i << 20)),
                          &s_mmuDevAttr); /* QSPI Rx Buf */
    }

    for (i = 0; i < 256; i++)
    {
        MMU_ConfigSection(MMU_L1Table, (const void *)(0x50000000U + (i << 20)), (0x50000000U + (i << 20)),
                          &s_mmuRamAttr); /* EIM */
    }

    for (i = 0; i < 256; i++)
    {
        MMU_ConfigSection(MMU_L1Table, (const void *)(0x60000000U + (i << 20)), (0x60000000U + (i << 20)),
                          &s_mmuRomAttr); /* QSPI */
    }

    for (i = 0; i < 2048; i++)
    {
        MMU_ConfigSection(MMU_L1Table, (const void *)(0x80000000U + (i << 20)), (0x80000000U + (i << 20)),
                          &s_mmuRamAttr); /* DDR */
    }

/* You can place global or static variables in NonCacheable section to make it uncacheable.*/
#if defined(__ICCARM__)
#pragma section = "NonCacheable"
    uint32_t ncahceStart = (uint32_t)__section_begin("NonCacheable");
    uint32_t size = (uint32_t)__section_size("NonCacheable");
#elif defined(__GNUC__)
    extern uint32_t __noncachedata_start__[];
    extern uint32_t __noncachedata_end__[];
    uint32_t ncahceStart = (uint32_t)__noncachedata_start__;
    uint32_t size = (uint32_t)__noncachedata_end__ - (uint32_t)__noncachedata_start__;
#else
#error Not supported compiler type
#endif
    size = (size + 0xFFFFFU) & (~0xFFFFFU);

    for (i = 0; i < ((size) >> 20); i++)
    {
        MMU_ConfigSection(MMU_L1Table, (const void *)(ncahceStart + (i << 20)), (ncahceStart + (i << 20)),
                          &s_mmuBufferAttr); /* Frame buffer */
    }

    MMU_Enable();
}

/* Get debug console frequency. */
static uint32_t BOARD_DebugConsoleSrcFreq(void)
{
    uint32_t freq;

    /* To make it simple, we assume default PLL and divider settings, and the only variable
       from application is use PLL3 source or OSC source */
    if (CLOCK_GetMux(kCLOCK_UartMux) == 0) /* PLL3 div6 80M */
    {
        freq = (CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 6U) / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    }
    else
    {
        freq = CLOCK_GetOscFreq() / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    }

    return freq;
}

/**
 * @brief  串口打印初始化(UART1)
 * @return 
 */
static void print_init(void)
{
    // GPIO配置
    IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);

    // 串口配置
    DbgConsole_Init((uint32_t)UART1, 115200, DEBUG_CONSOLE_DEVICE_TYPE_IUART, BOARD_DebugConsoleSrcFreq());
}

/**
 * @brief : 初始化(被__libc_init_array调用, 启动文件中先调用__libc_init_array再调用main)
 * @param  
 * @return 
 */
__attribute__((constructor)) void sys_init(void)
{
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    print_init();
    led_init();
    key_init();
}

// 延时
void delay(uint32_t count)
{
    volatile uint32_t i = 0;
    for(i = 0; i < count; i++)
    {
        __asm("nop");
    }
}


