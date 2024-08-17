/**
 * @brief   : LED
 * @author  : yanglx2022
 * @version : V0.1.0
 * @date    : 2024-05-21
 * @note    : 
 */

#include "led.h"


/**
 * @brief  LED初始化
 * @return 
 */
void led_init(void)
{
    // 配置GPIO
    gpio_pin_config_t gpio_pin_config;
    gpio_pin_config.direction     = kGPIO_DigitalOutput;
    gpio_pin_config.outputLogic   = 1;
    gpio_pin_config.interruptMode = kGPIO_NoIntmode;
    GPIO_PinInit(GPIO1, 4,  &gpio_pin_config);
    GPIO_PinInit(GPIO4, 20, &gpio_pin_config);
    GPIO_PinInit(GPIO4, 19, &gpio_pin_config);
    GPIO_PinInit(GPIO5, 3,  &gpio_pin_config);

    // 配置引脚复用
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO04_GPIO1_IO04, 0);
    IOMUXC_SetPinMux(IOMUXC_CSI_HSYNC_GPIO4_IO20,  0);
    IOMUXC_SetPinMux(IOMUXC_CSI_VSYNC_GPIO4_IO19,  0);
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER3_GPIO5_IO03, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO04_GPIO1_IO04, 0x10B0);
    IOMUXC_SetPinConfig(IOMUXC_CSI_HSYNC_GPIO4_IO20,  0x10B0);
    IOMUXC_SetPinConfig(IOMUXC_CSI_VSYNC_GPIO4_IO19,  0x10B0);
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER3_GPIO5_IO03, 0x10B0);
}

/**
 * @brief  控制LED
 * @param  led    LED1~LED4
 * @param  enable 1亮 0灭
 * @return 
 */
void led_ctrl(uint8_t led, uint8_t enable)
{
    if (led & LED1)
    {
        GPIO_WritePinOutput(GPIO1, 4, enable ? 0 : 1);
    }
    if (led & LED2)
    {
        GPIO_WritePinOutput(GPIO4, 20, enable ? 0 : 1);
    }
    if (led & LED3)
    {
        GPIO_WritePinOutput(GPIO4, 19, enable ? 0 : 1);
    }
    if (led & LED4)
    {
        GPIO_WritePinOutput(GPIO5, 3, enable ? 0 : 1);
    }
}


