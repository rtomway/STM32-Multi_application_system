#ifndef CONFIGSTORAGE_H_
#define CONFIGSTORAGE_H_

#include "appAlarmPage/alarmManager.h"
#include "mySpi/mySpi.h"

#define CONFIG_STORAGE_ADDR 0x000000   // 配置存储起始地址
#define W25Q64_SECTOR_SIZE 4096        // W25Q64扇区大小
#define W25Q64_PAGE_SIZE 256           // W25Q64页大小

typedef struct
{
    uint8_t config_brightness;
    Alarm config_alarm[3];
    uint8_t config_alarm_count;
} Config;


typedef struct
{
    Config config;  
    uint32_t crc;   
} ConfigStorage;

// 配置管理错误码
typedef enum
{
    CONFIG_OK,
    CONFIG_CRC_ERROR,
    CONFIG_SPI_ERROR,
    CONFIG_WRITE_ERROR,
    CONFIG_BUSY
} ConfigStatus;

void ConfigStorage_Init();
Config *ConfigStorage_GetConfig(void);
ConfigStatus ConfigStorage_Load(void);
ConfigStatus ConfigStorage_Save(void);

static uint8_t W25Q64_ReadStatus(void);

#endif /* CONFIGSTORAGE_H_ */