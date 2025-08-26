
/**
 * *****************************************************************************
 * @file    delay.c
 * @author  kabudac (kabudac@outlook.com)
 * @version V1.0
 * @date    2024-08-27
 * @brief   
 * *****************************************************************************
 */
#include "delay.h"


static uint32_t g_fac_us = 0;       /* us延时倍乘数 */


/**
 * @brief     初始化延迟函数
 * @param     sysclk: 系统时钟频率, 即CPU频率(rcc_c_ck), 如：72MHz则填入72
 * @retval    无
 */  
void delay_init(uint16_t sysclk)
{
    g_fac_us = sysclk;                                  /* 由于在HAL_Init中已对systick做了配置，所以这里无需重新配置 */
}

/**
 * @brief     延时nus
 * @note      用时钟摘取法来做us延时
 * @param     nus: 要延时的us数
 * @note      nus取值范围: 0 ~ (2^32 / fac_us) (fac_us一般等于系统主频, 自行套入计算)
 * @retval    无
 */
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        /* LOAD的值 */
    ticks = nus * g_fac_us;                 /* 需要的节拍数 */
    
    told = SysTick->VAL;                    /* 刚进入时的计数器值 */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        /* 这里注意一下SYSTICK是一个递减的计数器就可以了 */
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks) 
            {
                break;                      /* 时间超过/等于要延迟的时间,则退出 */
            }
        }
    }

}

/**
 * @brief     延时nms
 * @param     nms: 要延时的ms数 (0< nms <= (2^32 / fac_us / 1000))(fac_us一般等于系统主频, 自行套入计算)
 * @retval    无
 */
void delay_ms(uint16_t nms)
{
    delay_us((uint32_t)(nms * 1000));                   /* 普通方式延时 */
}

/**
 * @brief       HAL库内部函数用到的延时
 * @note        HAL库的延时默认用Systick，如果我们没有开Systick的中断会导致调用这个延时后无法退出
 * @param       Delay : 要延时的毫秒数
 * @retval      None
 */
void HAL_Delay(uint32_t Delay)
{
    delay_ms(Delay);
}

