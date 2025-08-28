#include "configStorage.h"
#include "crc.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "main.h"

#define W25Q64_WRITE_ENABLE 0x06
#define W25Q64_PAGE_PROGRAM 0x02
#define W25Q64_SECTOR_ERASE 0x20
#define W25Q64_READ_DATA 0x03
#define W25Q64_READ_STATUS 0x05
#define W25Q64_WRITE_STATUS_ENABLE 0x50

static NSS_GPIO_ENTRANCE w25qxx_nss={GPIOB,GPIO_PIN_0};
static  ConfigStorage currentStorage;
static volatile uint8_t isBusy = 0;

static SemaphoreHandle_t xSPIMutex = NULL;

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：ConfigStorage_Init
 *  * 功能描述：配置存储器初始化
 *  * 输入参数：spi_nss {type}
 ***********************************************************************************************************************/
void ConfigStorage_Init()
{
    if (xSPIMutex == NULL)
    {
        xSPIMutex = xSemaphoreCreateMutex();
    }
    memset((void *)&currentStorage, 0, sizeof(currentStorage));
    ConfigStorage_Load();
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：ConfigStorage_GetConfig
 *  * 功能描述：获取配置数据
 *  * 返 回 值：Config*
 ***********************************************************************************************************************/
Config *ConfigStorage_GetConfig(void)
{
    return &currentStorage.config;
}
/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：W25Q64_ReadStatus
 *  * 功能描述：检测W25Q64状态寄存器
 *  * 返 回 值：uint8_t
 ***********************************************************************************************************************/
uint8_t W25Q64_ReadStatus(void)
{
    uint8_t status;
    uint8_t cmd = 0x05;

    HAL_GPIO_WritePin(w25qxx_nss.GPIO_Port, w25qxx_nss.GPIO_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);
    HAL_SPI_Receive(&hspi1, &status, 1, 100);
    HAL_GPIO_WritePin(w25qxx_nss.GPIO_Port, w25qxx_nss.GPIO_Pin, GPIO_PIN_SET);

    return status;
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：W25Q64_WaitBusy
 *  * 功能描述：检测W25Q64状态寄存器，等待BUSY位清零
 ***********************************************************************************************************************/
static void W25Q64_WaitBusy(void)
{
    while (W25Q64_ReadStatus() & 0x01);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：ConfigStorage_Load
 *  * 功能描述：从存储器中加载配置数据
 *  * 返 回 值：ConfigStatus
 ***********************************************************************************************************************/
ConfigStatus ConfigStorage_Load(void)
{
    taskENTER_CRITICAL();

    if (xSemaphoreTake(xSPIMutex, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        taskEXIT_CRITICAL();
        return CONFIG_BUSY;
    }

    isBusy = 1;

    uint8_t readCmd[4] = {
        W25Q64_READ_DATA,
        (CONFIG_STORAGE_ADDR >> 16) & 0xFF,
        (CONFIG_STORAGE_ADDR >> 8) & 0xFF,
        CONFIG_STORAGE_ADDR & 0xFF};

    ConfigStorage tempStorage;
    HAL_GPIO_WritePin(w25qxx_nss.GPIO_Port, w25qxx_nss.GPIO_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, readCmd, 4, 100);
    HAL_SPI_Receive(&hspi1, (uint8_t *)&tempStorage, sizeof(ConfigStorage), 100);
    HAL_GPIO_WritePin(w25qxx_nss.GPIO_Port, w25qxx_nss.GPIO_Pin, GPIO_PIN_SET);

    // 计算CRC并验证
    uint32_t calculatedCrc = HAL_CRC_Calculate(&hcrc,(uint32_t *)&tempStorage.config,sizeof(Config) / sizeof(uint32_t));
    if (calculatedCrc != tempStorage.crc)
    {
        isBusy = 0;
        xSemaphoreGive(xSPIMutex);
        taskEXIT_CRITICAL();
        return CONFIG_CRC_ERROR; 
    }

    // CRC 验证通过
    memcpy(&currentStorage, &tempStorage, sizeof(ConfigStorage));

    isBusy = 0;
    xSemaphoreGive(xSPIMutex);

    taskEXIT_CRITICAL();
    return CONFIG_OK;
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：ConfigStorage_Save
 *  * 功能描述：保存配置数据到存储器
 *  * 返 回 值：ConfigStatus
 ***********************************************************************************************************************/
ConfigStatus ConfigStorage_Save(void)
{

   taskENTER_CRITICAL();

   if (xSemaphoreTake(xSPIMutex, pdMS_TO_TICKS(100)) != pdTRUE)
   {
       taskEXIT_CRITICAL(); 
       return CONFIG_BUSY;
   }

    isBusy = 1;

    currentStorage.crc = HAL_CRC_Calculate(&hcrc,(uint32_t *)&currentStorage.config,sizeof(Config) / sizeof(uint32_t));

    // 擦除扇区
    uint8_t sectorEraseCmd[4] = {
        W25Q64_SECTOR_ERASE,
        (CONFIG_STORAGE_ADDR >> 16) & 0xFF,
        (CONFIG_STORAGE_ADDR >> 8) & 0xFF,
        CONFIG_STORAGE_ADDR & 0xFF};

    // 写使能
    HAL_GPIO_WritePin(w25qxx_nss.GPIO_Port, w25qxx_nss.GPIO_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t[]){W25Q64_WRITE_ENABLE}, 1, 100);
    HAL_GPIO_WritePin(w25qxx_nss.GPIO_Port, w25qxx_nss.GPIO_Pin, GPIO_PIN_SET);

    // 发送擦除命令
    HAL_GPIO_WritePin(w25qxx_nss.GPIO_Port, w25qxx_nss.GPIO_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, sectorEraseCmd, 4, 100);
    HAL_GPIO_WritePin(w25qxx_nss.GPIO_Port, w25qxx_nss.GPIO_Pin, GPIO_PIN_SET);

    W25Q64_WaitBusy();

    uint8_t pageProgramCmd[4] = {
        W25Q64_PAGE_PROGRAM,
        (CONFIG_STORAGE_ADDR >> 16) & 0xFF,
        (CONFIG_STORAGE_ADDR >> 8) & 0xFF,
        CONFIG_STORAGE_ADDR & 0xFF};

    // 再次写使能
    HAL_GPIO_WritePin(w25qxx_nss.GPIO_Port, w25qxx_nss.GPIO_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t[]){W25Q64_WRITE_ENABLE}, 1, 100);
    HAL_GPIO_WritePin(w25qxx_nss.GPIO_Port, w25qxx_nss.GPIO_Pin, GPIO_PIN_SET);

    // 发送数据
    HAL_GPIO_WritePin(w25qxx_nss.GPIO_Port, w25qxx_nss.GPIO_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, pageProgramCmd, 4, 100);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&currentStorage, sizeof(currentStorage), 100);
    HAL_GPIO_WritePin(w25qxx_nss.GPIO_Port, w25qxx_nss.GPIO_Pin, GPIO_PIN_SET);

    // 等待写入完成
    W25Q64_WaitBusy();

    isBusy = 0;
    xSemaphoreGive(xSPIMutex);
    taskEXIT_CRITICAL(); 

    return CONFIG_OK;
}

