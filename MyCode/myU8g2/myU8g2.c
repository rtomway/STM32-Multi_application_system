#include "myU8g2.h"
#include "i2c.h"
#include "delay/delay.h"

u8g2_t u8g2;

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：myU8g2_Init
 *  * 功能描述：初始化u8g2
 *  * 输入参数：u8g2 {type}
 ***********************************************************************************************************************/
void myU8g2_Init(u8g2_t *u8g2)
{
   u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_hw_i2c, u8g2_gpio_and_delay_stm32);
   u8g2_InitDisplay(u8g2);
   u8g2_SetPowerSave(u8g2, 0);
   u8g2_ClearBuffer(u8g2);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：u8x8_byte_hw_i2c
 *  * 功能描述：改成硬件I2C
 *  * 输入参数：u8x8 {type}
 *  * 输入参数：msg {type}
 *  * 输入参数：arg_int {type}
 *  * 输入参数：arg_ptr {type}
 *  * 返 回 值：uint8_t
 ***********************************************************************************************************************/
uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    uint8_t *data=(uint8_t*)arg_ptr;
    static uint8_t data_buf[32];
    static uint8_t data_buf_index;

    switch (msg)
    {
    case U8X8_MSG_BYTE_SEND:
        while (arg_int > 0)
        {
            data_buf[data_buf_index++] = *data++;
            arg_int--;
        }
        break;
    case U8X8_MSG_BYTE_INIT:
        MX_I2C1_Init();
        break;

    case U8X8_MSG_BYTE_SET_DC:
        break;

    case U8X8_MSG_BYTE_START_TRANSFER:
        data_buf_index = 0;
        break;

    case U8X8_MSG_BYTE_END_TRANSFER:
        if(HAL_I2C_Master_Transmit(&hi2c1,0x78,data_buf,data_buf_index,HAL_MAX_DELAY)!=HAL_OK)
        {
            return 0;
        }
        break;

    default:
        return 0;
    }
    return 1;
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：u8g2_gpio_and_delay_stm32
 *  * 功能描述：
 *  * 输入参数：u8x8 {type}
 *  * 输入参数：msg {type}
 *  * 输入参数：arg_int {type}
 *  * 输入参数：arg_ptr {type}
 *  * 返 回 值：uint8_t
 ***********************************************************************************************************************/
uint8_t u8g2_gpio_and_delay_stm32(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        /* HAL initialization contains all what we need so we can skip this part. */
        break;
    case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
        HAL_Delay(arg_int);
        break;
    case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
        __NOP();
        break;
    case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
        for (uint16_t n = 0; n < 320; n++)
        {
            __NOP();
        }
        break;
    case U8X8_MSG_DELAY_I2C: // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
        delay_us(5);
        break;                    // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int=0: Output low at I2C clock pin
        break;                    // arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:  // arg_int=0: Output low at I2C data pin
        break;                    // arg_int=1: Input dir with pullup high for I2C data pin
    case U8X8_MSG_GPIO_MENU_SELECT:
        u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_NEXT:
        u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_PREV:
        u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_HOME:
        u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
        break;
    default:
        u8x8_SetGPIOResult(u8x8, 1); // default return value
        break;
    }
    return 1;
}

