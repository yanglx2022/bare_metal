/**
 * @brief   : 按键
 * @author  : yanglx2022
 * @version : V0.1.0
 * @date    : 2024-05-29
 * @note    : 
 */

#include "key.h"

// 按键按下状态
static uint8_t key_pressed = 0;

void GPIO5_Combined_0_15_IRQHandler(uint32_t gicc_iar, void *param);


/**
 * @brief  按键初始化
 * @return 
 */
void key_init(void)
{
    // 配置GPIO
    gpio_pin_config_t gpio_pin_config;
    gpio_pin_config.direction     = kGPIO_DigitalInput;
    gpio_pin_config.interruptMode = kGPIO_IntRisingOrFallingEdge;
    GPIO_PinInit(GPIO5, 1,  &gpio_pin_config);

    // 配置引脚复用
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0);
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0x10B0);

    // 中断配置
    GPIO_EnableInterrupts(GPIO5, 1 << 1);
    SystemInstallIrqHandler(GPIO5_Combined_0_15_IRQn, GPIO5_Combined_0_15_IRQHandler, NULL);
    GIC_EnableIRQ(GPIO5_Combined_0_15_IRQn);
}

/**
 * @brief  获取按键状态
 * @return 0未按下 1按下
 */
uint8_t key_state(void)
{
    return key_pressed;
}

/**
 * @brief  GPIO5中断响应
 * @param  gicc_iar
 * @param  *param
 * @return 
 */
void GPIO5_Combined_0_15_IRQHandler(uint32_t gicc_iar, void *param)
{
    if (GPIO_ReadPinInput(GPIO5, 1))    // 按下高电平
    {
        key_pressed = 1;
    }
    else
    {
        key_pressed = 0;
    }

    GPIO_ClearPinsInterruptFlags(GPIO5, 1 << 1);
}


