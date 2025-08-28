#ifndef MYSPI_H_
#define MYSPI_H_

#include "main.h"
#include "gpio.h"
#include "spi.h"

// NSS引脚配置结构体
typedef struct
{
    GPIO_TypeDef *GPIO_Port;
    uint16_t GPIO_Pin;
} NSS_GPIO_ENTRANCE;

// SPI传输模式枚举
typedef enum
{
    SPI_MODE_BLOCKING,  // 阻塞式
    SPI_MODE_INTERRUPT, // 中断模式
    SPI_MODE_DMA        // DMA模式
} SPI_TransferMode;

// SPI操作状态
typedef enum
{
    SPI_STATE_READY,
    SPI_STATE_BUSY,
    SPI_STATE_ERROR
} SPI_State;


// 函数声明
HAL_StatusTypeDef SPI_Transmit(NSS_GPIO_ENTRANCE nss, uint8_t *pData, uint16_t size, SPI_TransferMode mode);
HAL_StatusTypeDef SPI_ReadData(NSS_GPIO_ENTRANCE nss, uint8_t *sendCmd, uint8_t cmdSize, uint8_t *recvBuffer, uint16_t recvSize, SPI_TransferMode mode);

// DMA专用接口
HAL_StatusTypeDef SPI_Transmit_DMA(NSS_GPIO_ENTRANCE nss, uint8_t *pData, uint16_t size);
HAL_StatusTypeDef SPI_Receive_DMA(NSS_GPIO_ENTRANCE nss, uint8_t *sendCmd, uint8_t cmdSize,uint8_t *recvBuffer, uint16_t recvSize);

SPI_State SPI_GetState(void);

void SPI_CS_Enable(NSS_GPIO_ENTRANCE nss);
void SPI_CS_Disable(NSS_GPIO_ENTRANCE nss);

#endif /* MYSPI_H_ */