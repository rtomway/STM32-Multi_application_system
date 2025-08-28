#include "mySpi.h"

static SPI_State spi_state = SPI_STATE_READY;
static NSS_GPIO_ENTRANCE spi_current_nss;

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：SPI_Transmit
 *  * 功能描述：SPI发送函数
 *  * 输入参数：nss {type}
 *  * 输入参数：pData {type}
 *  * 输入参数：size {type}
 *  * 输入参数：mode {type}
 ***********************************************************************************************************************/
HAL_StatusTypeDef SPI_Transmit(NSS_GPIO_ENTRANCE nss, uint8_t *pData, uint16_t size, SPI_TransferMode mode)
{
    SPI_CS_Enable(nss);
    HAL_StatusTypeDef status = HAL_OK;
    spi_current_nss = nss;

    switch (mode)
    {
    case SPI_MODE_BLOCKING:
        status = HAL_SPI_Transmit(&hspi1, pData, size, HAL_MAX_DELAY);
        break;
    case SPI_MODE_INTERRUPT:
        status = HAL_SPI_Transmit_IT(&hspi1, pData, size);
        break;
    case SPI_MODE_DMA:
        status = HAL_SPI_Transmit_DMA(&hspi1, pData, size);
        break;
    }

    if (mode == SPI_MODE_BLOCKING)
    {
        SPI_CS_Disable(nss);
    }
    else
    {
        spi_state = SPI_STATE_BUSY;
    }

    return status;
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：SPI_ReadData
 *  * 功能描述：SPI接收函数
 *  * 输入参数：nss {type}
 *  * 输入参数：sendCmd {type}
 *  * 输入参数：cmdSize {type}
 *  * 输入参数：recvBuffer {type}
 *  * 输入参数：recvSize {type}
 *  * 输入参数：mode {type}
 *  * 返 回 值：HAL_StatusTypeDef
 ***********************************************************************************************************************/
HAL_StatusTypeDef SPI_ReadData(NSS_GPIO_ENTRANCE nss, uint8_t *sendCmd, uint8_t cmdSize,
                               uint8_t *recvBuffer, uint16_t recvSize, SPI_TransferMode mode)
{
    SPI_CS_Enable(nss);

    HAL_StatusTypeDef status;
    spi_current_nss = nss;

    // 发送命令
    status = HAL_SPI_Transmit(&hspi1, sendCmd, cmdSize, HAL_MAX_DELAY);
    if (status != HAL_OK)
    {
        SPI_CS_Disable(nss);
        return status;
    }

    // 接收数据
    switch (mode)
    {
    case SPI_MODE_BLOCKING:
        status = HAL_SPI_Receive(&hspi1, recvBuffer, recvSize, HAL_MAX_DELAY);
        SPI_CS_Disable(nss);
        break;
    case SPI_MODE_INTERRUPT:
        status = HAL_SPI_Receive_IT(&hspi1, recvBuffer, recvSize);
        spi_state = SPI_STATE_BUSY;
        break;
    case SPI_MODE_DMA:
        status = HAL_SPI_Receive_DMA(&hspi1, recvBuffer, recvSize);
        spi_state = SPI_STATE_BUSY;
        break;
    }

    return status;
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：SPI_Transmit_DMA
 *  * 功能描述：SPI DMA发送函数
 *  * 输入参数：nss {type}
 *  * 输入参数：pData {type}
 *  * 输入参数：size {type}
 *  * 返 回 值：HAL_StatusTypeDef
 ***********************************************************************************************************************/
HAL_StatusTypeDef SPI_Transmit_DMA(NSS_GPIO_ENTRANCE nss, uint8_t *pData, uint16_t size)
{
    SPI_CS_Enable(nss);
    spi_current_nss = nss;
    spi_state = SPI_STATE_BUSY;
    return HAL_SPI_Transmit_DMA(&hspi1, pData, size);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：SPI_Receive_DMA
 *  * 功能描述：SPI DMA接收函数
 *  * 输入参数：nss {type}
 *  * 输入参数：sendCmd {type}
 *  * 输入参数：cmdSize {type}
 *  * 输入参数：recvBuffer {type}
 *  * 输入参数：recvSize {type}
 *  * 返 回 值：HAL_StatusTypeDef
 ***********************************************************************************************************************/
HAL_StatusTypeDef SPI_Receive_DMA(NSS_GPIO_ENTRANCE nss, uint8_t *sendCmd, uint8_t cmdSize,
                                  uint8_t *recvBuffer, uint16_t recvSize)
{
    SPI_CS_Enable(nss);
    spi_current_nss = nss;

    HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi1, sendCmd, cmdSize, HAL_MAX_DELAY);
    if (status != HAL_OK)
    {
        SPI_CS_Disable(nss);
        return status;
    }

    spi_state = SPI_STATE_BUSY;

    status = HAL_SPI_Receive_DMA(&hspi1, recvBuffer, recvSize);

    return status;
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：SPI_GetState
 *  * 功能描述：SPI状态获取函数
 *  * 返 回 值：SPI_State
 ***********************************************************************************************************************/
SPI_State SPI_GetState(void)
{
    return spi_state;
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：SPI_CS_Enable
 *  * 功能描述：SPI片选使能函数
 *  * 输入参数：nss {type}
 ***********************************************************************************************************************/
void SPI_CS_Enable(NSS_GPIO_ENTRANCE nss)
{
    HAL_GPIO_WritePin(nss.GPIO_Port, nss.GPIO_Pin, GPIO_PIN_RESET);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：SPI_CS_Disable
 *  * 功能描述：SPI片选失能函数
 *  * 输入参数：nss {type}
 ***********************************************************************************************************************/
void SPI_CS_Disable(NSS_GPIO_ENTRANCE nss)
{
    HAL_GPIO_WritePin(nss.GPIO_Port, nss.GPIO_Pin, GPIO_PIN_SET);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：HAL_SPI_TxCpltCallback
 *  * 功能描述：SPI发送完成回调函数
 *  * 输入参数：hspi {type}
 ***********************************************************************************************************************/
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == hspi1.Instance)
    {
        SPI_CS_Disable(spi_current_nss);

        /* 更新状态 */
        spi_state = SPI_STATE_READY;
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：HAL_SPI_RxCpltCallback
 *  * 功能描述：SPI接收完成回调函数
 *  * 输入参数：hspi {type}
 ***********************************************************************************************************************/
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == hspi1.Instance)
    {
        SPI_CS_Disable(spi_current_nss);
        spi_state = SPI_STATE_READY;
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：HAL_SPI_TxRxCpltCallback
 *  * 功能描述：SPI发送接收完成回调函数
 *  * 输入参数：hspi {type}
 ***********************************************************************************************************************/
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    /* 如果使用了全双工模式，可能会用到这个回调 */
    if (hspi->Instance == hspi1.Instance)
    {
        SPI_CS_Disable(spi_current_nss);
        spi_state = SPI_STATE_READY;
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：SPI_WAIT_FOR_READY
 *  * 功能描述：SPI正在传输时，等待函数
 ***********************************************************************************************************************/
void SPI_WAIT_FOR_READY(void)
{
    while (spi_state == SPI_STATE_BUSY)
    {
    }
}