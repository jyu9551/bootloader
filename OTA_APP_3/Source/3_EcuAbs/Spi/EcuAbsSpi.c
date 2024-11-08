#include "EcuAbsSpi.h"
#include "Spi.h"
#include "Spi_Cfg.h"

#define CMD_AXIS6_WRITE 0u
#define CMD_AXIS6_READ 1u
#define SPI_CHANNEL_AXIS6 SpiConf_SpiChannel_SpiChannel_0
#define SPI_DUMMY_DATA   0xFFu

#define REG_VALUE_AXIS6_CTRL3C_BDU_NOT_UPDATE_UNTIL_READ_MSB_LSB    0x40u
#define REG_VALUE_AXIS6_GYRO_ODR1667_CTRL2                          0x80u
#define REG_VALUE_AXIS6_GYRO_SCALE125DPS_CTRL2                      0x02u
#define REG_VALUE_AXIS6_ACEEL_ODR1667_CTRL1                         0x80u
#define REG_VALUE_AXIS6_ACEEL_SCALE4G_CTRL1                         0x08u
#define REG_VALUE_AXIS6_WHO_AM_I                                    0x6Bu

#define REG_ADDR_AXIS6_WHO_AM_I             0x0Fu
#define REG_ADDR_AXIS6_CTRL1_XL             0x10u
#define REG_ADDR_AXIS6_CTRL2_G              0x11u
#define REG_ADDR_AXIS6_CTRL3_C              0x12u
#define REG_ADDR_AXIS6_STATUS_REG           0x1Eu
#define REG_ADDR_AXIS6_STATUS_REG_TDA       0x04u
#define REG_ADDR_AXIS6_STATUS_REG_GDA       0x02u
#define REG_ADDR_AXIS6_STATUS_REG_XLDA      0x01u
#define REG_ADDR_AXIS6_OUT_TEMP_L           0x20u
#define REG_ADDR_AXIS6_OUT_TEMP_H           0x21u
#define REG_ADDR_AXIS6_OUTX_L_G             0x22u
#define REG_ADDR_AXIS6_OUTX_H_G             0x23u
#define REG_ADDR_AXIS6_OUTY_L_G             0x24u
#define REG_ADDR_AXIS6_OUTY_H_G             0x25u
#define REG_ADDR_AXIS6_OUTZ_L_G             0x26u
#define REG_ADDR_AXIS6_OUTZ_H_G             0x27u
#define REG_ADDR_AXIS6_OUTX_L_A             0x28u
#define REG_ADDR_AXIS6_OUTX_H_A             0x29u
#define REG_ADDR_AXIS6_OUTY_L_A             0x2Au
#define REG_ADDR_AXIS6_OUTY_H_A             0x2Bu
#define REG_ADDR_AXIS6_OUTZ_L_A             0x2Cu
#define REG_ADDR_AXIS6_OUTZ_H_A             0x2Du

typedef struct {
    uint8 whoAmI;
    sint16 temperature;
    sint16 gyroX;
    sint16 gyroY;
    sint16 gyroZ;
    sint16 accelX;
    sint16 accelY;
    sint16 accelZ;
}ST_Axis6Data;
static ST_Axis6Data stAxis6Data;

typedef struct {
    unsigned int value : 8;
    unsigned int address : 7;
    unsigned int commend : 1;
} ST_SpiCommunication;

static uint16_t spiCommunication(Spi_ChannelType channel, uint8 commend, uint8 address, uint8 value);
static void readAxis6Data(void);

/* Common setting */
static uint8 axis6RegValueCtrl3C = REG_VALUE_AXIS6_CTRL3C_BDU_NOT_UPDATE_UNTIL_READ_MSB_LSB;
/* Gyroscope => ODR : 1667Hz, scale : +-125dps, Factor : 0.00437 */
static uint8 axis6RegValueCtrl2G = REG_VALUE_AXIS6_GYRO_ODR1667_CTRL2|REG_VALUE_AXIS6_GYRO_SCALE125DPS_CTRL2;
/* Accelerometer => ODR : 1667Hz, scale : +-4g, Factor : 0.000122 */
static uint8 axis6RegValueCtrl1XL = REG_VALUE_AXIS6_ACEEL_ODR1667_CTRL1|REG_VALUE_AXIS6_ACEEL_SCALE4G_CTRL1;

void EA_initSpi(void)
{
    uint16 readAxis6RegValueCtrl3C;
    uint16 readAxis6RegValueCtrl2G;
    uint16 readAxis6RegValueCtrl1XL;

    /* WhoAmI */
    while(stAxis6Data.whoAmI != REG_VALUE_AXIS6_WHO_AM_I)
    {
        stAxis6Data.whoAmI = ((uint8_t) spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ,(uint16_t)REG_ADDR_AXIS6_WHO_AM_I, SPI_DUMMY_DATA));
    }
    /* Common setting */
    while(readAxis6RegValueCtrl3C != axis6RegValueCtrl3C)
    {
        (void) spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_WRITE, REG_ADDR_AXIS6_CTRL3_C, axis6RegValueCtrl3C);
        readAxis6RegValueCtrl3C = spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ, REG_ADDR_AXIS6_CTRL3_C, SPI_DUMMY_DATA);
    }
    /* Gyroscope setting */
    while(readAxis6RegValueCtrl2G != axis6RegValueCtrl2G)
    {
        (void) spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_WRITE, REG_ADDR_AXIS6_CTRL2_G, axis6RegValueCtrl2G);
        readAxis6RegValueCtrl2G = spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ, REG_ADDR_AXIS6_CTRL2_G, SPI_DUMMY_DATA);
    }
    /* Accelerometer setting */
    while(readAxis6RegValueCtrl1XL != axis6RegValueCtrl1XL)
    {
        (void) spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_WRITE, REG_ADDR_AXIS6_CTRL1_XL, axis6RegValueCtrl1XL);
        readAxis6RegValueCtrl1XL = spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ, REG_ADDR_AXIS6_CTRL1_XL, SPI_DUMMY_DATA);
    }
}

void EA_mainSpi(void)
{
    readAxis6Data();
}

static void readAxis6Data(void)
{
    uint8_t status;
    status = (uint8_t) spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ, REG_ADDR_AXIS6_STATUS_REG, SPI_DUMMY_DATA);
    if((status & REG_ADDR_AXIS6_STATUS_REG_TDA) != 0u)
    {
        /* ODR : 52Hz, Factor : 0.00390625 (1/256), Offset : 25C */
        stAxis6Data.temperature = (int16_t)spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ,(uint16_t)REG_ADDR_AXIS6_OUT_TEMP_L, SPI_DUMMY_DATA);
        stAxis6Data.temperature |= (spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ,(uint16_t)REG_ADDR_AXIS6_OUT_TEMP_H, SPI_DUMMY_DATA)<<8);
    }
    if((status & REG_ADDR_AXIS6_STATUS_REG_GDA) != 0u)
    {
        stAxis6Data.gyroX = (sint16)spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ,(uint16_t)REG_ADDR_AXIS6_OUTX_L_G, SPI_DUMMY_DATA);
        stAxis6Data.gyroX |= (sint16)spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ,(uint16_t)REG_ADDR_AXIS6_OUTX_H_G, SPI_DUMMY_DATA) << 8;
        stAxis6Data.gyroY = (sint16)spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ,(uint16_t)REG_ADDR_AXIS6_OUTY_L_G, SPI_DUMMY_DATA);
        stAxis6Data.gyroY |= (sint16)spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ,(uint16_t)REG_ADDR_AXIS6_OUTY_H_G, SPI_DUMMY_DATA)<<8;
        stAxis6Data.gyroZ = (sint16)spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ,(uint16_t)REG_ADDR_AXIS6_OUTZ_L_G, SPI_DUMMY_DATA);
        stAxis6Data.gyroZ |= (sint16)spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ,(uint16_t)REG_ADDR_AXIS6_OUTZ_H_G, SPI_DUMMY_DATA)<<8;
    }
    if((status & REG_ADDR_AXIS6_STATUS_REG_XLDA) != 0u)
    {
        stAxis6Data.accelX = (sint16)spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ,(uint16_t)REG_ADDR_AXIS6_OUTX_L_A, SPI_DUMMY_DATA);
        stAxis6Data.accelX |= (sint16)spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ,(uint16_t)REG_ADDR_AXIS6_OUTX_H_A, SPI_DUMMY_DATA)<<8;
        stAxis6Data.accelY = (sint16)spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ,(uint16_t)REG_ADDR_AXIS6_OUTY_L_A, SPI_DUMMY_DATA);
        stAxis6Data.accelY |= (sint16)spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ,(uint16_t)REG_ADDR_AXIS6_OUTY_H_A, SPI_DUMMY_DATA)<<8;
        stAxis6Data.accelZ = (sint16)spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ,(uint16_t)REG_ADDR_AXIS6_OUTZ_L_A, SPI_DUMMY_DATA);
        stAxis6Data.accelZ |= (sint16)spiCommunication(SPI_CHANNEL_AXIS6, CMD_AXIS6_READ,(uint16_t)REG_ADDR_AXIS6_OUTZ_H_A, SPI_DUMMY_DATA)<<8;
    }
}

static uint16_t spiCommunication(Spi_ChannelType channel, uint8 commend, uint8 address, uint8 value)
{
    ST_SpiCommunication stSpiCommunication;
    uint16 rxData;
    stSpiCommunication.commend = commend;
    stSpiCommunication.address = address;
    stSpiCommunication.value = value;
    (void)Spi_SetupEB(channel, (const Spi_DataType*)&stSpiCommunication, (Spi_DataType*)&rxData, (Spi_NumberOfDataType)1u);
    (void)Spi_SyncTransmit(channel);
    return rxData;
}
