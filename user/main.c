/**
 * @brief   : IMX6ULL裸机程序
 * @author  : yanglx2022
 * @version : V0.1.0
 * @date    : 2024-05-12
 * @note    : 
 */

#include "common.h"
#include "led.h"
#include "key.h"


int main(void)
{
    PRINTF("Hello World! %d 0x%02X %.2f\n", 255, 255, 1.1f);

    while(1)
    {
        if (key_state())
        {
            uint8_t led[] = {LED1, LED2, LED3, LED4};
            for(int i = 0; i < 4; i++)
            {
                led_ctrl(led[i], 1);
                delay(20000000);
            }
            led_ctrl(LED1 | LED2 | LED3 | LED4, 0);
            delay(20000000);
        }
    }
}


