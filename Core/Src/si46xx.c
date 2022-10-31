
#include <string.h>
#include "main.h"

extern const uint8_t fw_rom00_patch016[5796];
extern const uint8_t fw_dab_radio_5_0_5[521448];
extern const uint8_t fw_dab_radio_6_0_6[496940];
extern const uint8_t fw_fmhd_radio_5_0_4[530180];

uint8_t si46xx_buffer[2048];

#define SI46XX_RD_REPLY                      0x00
#define SI46XX_POWER_UP                      0x01
#define SI46XX_HOST_LOAD                     0x04
#define SI46XX_FLASH_LOAD                    0x05
#define SI46XX_LOAD_INIT                     0x06
#define SI46XX_BOOT                          0x07
#define SI46XX_GET_FUNC_INFO                 0x12
#define SI46XX_SET_PROPERTY                  0x13
#define SI46XX_FM_TUNE_FREQ                  0x30
#define SI46XX_FM_RSQ_STATUS                 0x32

#define SI46XX_FM_INT_CTL_ENABLE             0x0000
#define SI46XX_DIGITAL_IO_OUTPUT_SELECT      0x0200
#define SI46XX_AUDIO_ANALOG_VOLUME           0x0300
#define SI46XX_AUDIO_MUTE                    0x0301
#define SI46XX_PIN_CONFIG_ENABLE             0x0800
#define SI46XX_FM_SEEK_FREQUENCY_SPACING     0x3102
#define SI46XX_FM_VALID_SNR_THRESHOLD        0x3204
#define SI46XX_FM_RSQ_SNR_HIGH_THRESHOLD     0x3301
#define SI46XX_FM_RSQ_RSSI_HIGH_THRESHOLD    0x3303
#define SI46XX_FM_RDS_INTERRUPT_SOURCE       0x3C00
#define SI46XX_FM_RDS_INTERRUPT_FIFO_COUNT   0x3C01
#define SI46XX_FM_RDS_CONFIG                 0x3C02
#define SI46XX_FM_RDS_CONFIDENCE             0x3C03
#define SI46XX_DAB_TUNE_FE_CFG               0x1712
#define SI46XX_DAB_TUNE_FE_VARM              0x1710
#define SI46XX_DAB_TUNE_FE_VARB              0x1711


const uint8_t Data_800bfae[180] =
{
   0xb8, //CMD_DAB_SET_FREQ_LIST
   0x2c,
   0x01,
   0x00,
   0x50,
   0xab,
   0x02,
   0x00,
   0x00,
   0xb2,
   0x02,
   0x00,
   0xb0,
   0xb8,
   0x02,
   0x00,
   0x60,
   0xbf,
   0x02,
   0x00,
   0xb0,
   0xc6,
   0x02,
   0x00,
   0x60,
   0xcd,
   0x02,
   0x00,
   0x10,
   0xd4,
   0x02,
   0x00,
   0xc0,
   0xda,
   0x02,
   0x00,
   0x00,
   0xe2,
   0x02,
   0x00,
   0xb0,
   0xe8,
   0x02,
   0x00,
   0x60,
   0xef,
   0x02,
   0x00,
   0x10,
   0xf6,
   0x02,
   0x00,
   0x60,
   0xfd,
   0x02,
   0x00,
   0x10,
   0x04,
   0x03,
   0x00,
   0xc0,
   0x0a,
   0x03,
   0x00,
   0x70,
   0x11,
   0x03,
   0x00,
   0xb0,
   0x18,
   0x03,
   0x00,
   0x60,
   0x1f,
   0x03,
   0x00,
   0x10,
   0x26,
   0x03,
   0x00,
   0xc0,
   0x2c,
   0x03,
   0x00,
   0x10,
   0x34,
   0x03,
   0x00,
   0xc0,
   0x3a,
   0x03,
   0x00,
   0x70,
   0x41,
   0x03,
   0x00,
   0x20,
   0x48,
   0x03,
   0x00,
   0x60,
   0x4f,
   0x03,
   0x00,
   0x10,
   0x56,
   0x03,
   0x00,
   0xc0,
   0x5c,
   0x03,
   0x00,
   0x70,
   0x63,
   0x03,
   0x00,
   0xc0,
   0x6a,
   0x03,
   0x00,
   0x70,
   0x71,
   0x03,
   0x00,
   0x20,
   0x78,
   0x03,
   0x00,
   0xd0,
   0x7e,
   0x03,
   0x00,
   0x80,
   0x85,
   0x03,
   0x00,
   0x30,
   0x8c,
   0x03,
   0x00,
   0xe0,
   0x92,
   0x03,
   0x00,
   0x00,
   0x99,
   0x03,
   0x00,
   0xb0,
   0x9f,
   0x03,
   0x00,
   0x60,
   0xa6,
   0x03,
   0x00,
   0x20,
   0xd8,
   0x03,
   0x00,
   0xd0,
   0xde,
   0x03,
   0x00,
   0x80,
   0xe5,
   0x03,
   0x00,
   0x30,
   0xec,
   0x03,
   0x00,
   0x70,
   0xf3,
   0x03,
   0x00,
   0x20,
   0xfa,
   0x03,
   0x00,
};


int si46xx_read_reply(uint16_t r8, uint16_t Size)
{
   int res = 0;
   uint8_t i = 10;

   while (i--)
   {
      HAL_Delay(r8);
	  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);

	  si46xx_buffer[0] = SI46XX_RD_REPLY;
	  if (0 != HAL_SPI_Transmit(&hspi3, si46xx_buffer, 1, 10))
	  {
         return 0;
	  }

	  if (0 != HAL_SPI_Receive(&hspi3, si46xx_buffer, Size, 10))
	  {
         return 0;
	  }

	  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);

	  if (si46xx_buffer[0] & 0x80)
	  {
		  break;
	  }
   }

   if (si46xx_buffer[0] & 0x40)
   {
      res = (si46xx_buffer[4] != 0);
   }
   if (i == 0)
   {
      res = (si46xx_buffer[4] != 0);
   }

   return res;
}


int si46xx_send_command(uint16_t sendSize, uint16_t recSize, uint16_t c)
{
   HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);

   if (0 != HAL_SPI_Transmit(&hspi3, si46xx_buffer, sendSize, 10))
   {
      return 1;
   }

   HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);

   return si46xx_read_reply(c, recSize);
}


int si46xx_set_property(uint16_t name, uint16_t value)
{
   si46xx_buffer[0] = SI46XX_SET_PROPERTY;
   si46xx_buffer[1] = 0;
   si46xx_buffer[2] = name;
   si46xx_buffer[3] = name >> 8;
   si46xx_buffer[4] = value;
   si46xx_buffer[5] = value >> 8;

   if (0 != si46xx_send_command(6, 4, 1))
   {
      return 1;
   }

   return 0;
}


int si46xx_mute(uint8_t a)
{
   if (0 != si46xx_set_property(SI46XX_AUDIO_MUTE,
               (a == 0)? 0/*Do not mute audio outputs*/:
                         3/*Mute both Left and Right Audio Out*/))
   {
      return 1;
   }

#if 0
   if (a != 0)
   {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
   }
   else
   {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
   }
#endif

   return 0;
}


int si46xx_write_host_load_data(const uint8_t* data, uint32_t len)
{
   si46xx_buffer[0] = SI46XX_LOAD_INIT;
   si46xx_buffer[1] = 0;

   if (0 != si46xx_send_command(2, 4, 1))
   {
	  return 1;
   }

   uint32_t offset = 0;
	while (offset < len)
	{
		uint8_t* buf = &si46xx_buffer[0];

		buf[0] = SI46XX_HOST_LOAD;
		buf[1] = 0;
		buf[2] = 0;
		buf[3] = 0;

		uint32_t count = sizeof(si46xx_buffer) - 4;
		uint32_t remain = len - offset;

		if (count > remain)
		{
			count = remain;
		}

		memcpy(buf + 4, data + offset, count);

		offset += count;

		/* CS to low */
		HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);

	   if (0 != HAL_SPI_Transmit(&hspi3, buf, count + 4, 100))
	   {
		  return 1;
	   }

	   /* CS to high */
	   HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);

	   HAL_Delay(1);
	}

   HAL_Delay(4);

   if (0 != si46xx_read_reply(1, 4))
   {
	  return 1;
   }

   HAL_Delay(4);

   return 0;
}


int si46xx_load_and_boot(uint8_t isDab)
{
   HAL_GPIO_WritePin(Si4684x_Reset_GPIO_Port, Si4684x_Reset_Pin, GPIO_PIN_RESET);

   HAL_Delay(10);

   HAL_GPIO_WritePin(Si4684x_Reset_GPIO_Port, Si4684x_Reset_Pin, GPIO_PIN_SET);

   if (0 != si46xx_read_reply(100, 6))
   {
      return 1;
   }

   si46xx_buffer[0] = SI46XX_POWER_UP;
   si46xx_buffer[1] = 0x80;
   si46xx_buffer[2] = (1 << 4)/*1: Reference clock generator is in crystal mode.*/
                      | 7/*TR_SIZE*/;
   si46xx_buffer[3] = 0x48;

   si46xx_buffer[4] = 0; //XTAL_FREQ: 0x0124F900 = 19200256
   si46xx_buffer[5] = 0xf9;
   si46xx_buffer[6] = 0x24;
   si46xx_buffer[7] = 1;

   si46xx_buffer[8] = 0x1f;
   si46xx_buffer[9] = 0x10;
   si46xx_buffer[10] = 0;
   si46xx_buffer[11] = 0;
   si46xx_buffer[12] = 0;
   si46xx_buffer[13] = 0;
   si46xx_buffer[14] = 0;
   si46xx_buffer[15] = 0;

   if (0 != si46xx_send_command(16, 4, 100))
   {
      return 1;
   }

   if ((si46xx_buffer[3] & 0xC0) != 0x80)
   {
      return 1;
   }

#if 0
   if (0 != si46xx_write_host_load_data(fw_rom00_patch016, sizeof(fw_rom00_patch016)))
   {
      return 1;
   }
#endif

   if (isDab != 0)
   {
	   if (0 != si46xx_write_host_load_data(fw_dab_radio_5_0_5, sizeof(fw_dab_radio_5_0_5)))
	   {
		  return 1;
	   }
   }
   else
   {
	   if (0 != si46xx_write_host_load_data(fw_fmhd_radio_5_0_4, sizeof(fw_fmhd_radio_5_0_4)))
	   {
		  return 1;
	   }
   }

   si46xx_buffer[0] = SI46XX_BOOT;
   si46xx_buffer[1] = 0;

   if (0 != si46xx_send_command(2, 4, 500))
   {
      return 1;
   }

   if ((si46xx_buffer[3] & 0xC0) != 0xC0)
   {
      return 1;
   }

   return 0;
}


static int si46xx_set_dab_config(void)
{
   if (0 != si46xx_set_property(0x8100, 1))
   {
      return 1;
   }

   if (0 != si46xx_set_property(SI46XX_DAB_TUNE_FE_VARM, 0xfbf5))
   {
      return 1;
   }

   if (0 != si46xx_set_property(SI46XX_DAB_TUNE_FE_VARB, 0x122))
   {
      return 1;
   }

   if (0 != si46xx_set_property(SI46XX_DAB_TUNE_FE_CFG, 1))
   {
      return 1;
   }

   if (0 != si46xx_set_property(SI46XX_FM_INT_CTL_ENABLE,
               (1 << 4)/*DSRVIEN (Interrupt when DSRVINT is set)*/))
   {
      return 1;
   }

   return 0;
}


static int si46xx_set_audio_output(uint8_t a)
{
   if (a == 1)
   {
      if (0 != si46xx_set_property(SI46XX_DIGITAL_IO_OUTPUT_SELECT,
                  (1 << 15)/*Master mode selected*/))
      {
         return 1;
      }
   }

   if (0 != si46xx_set_property(SI46XX_PIN_CONFIG_ENABLE, (a == 0)?
                (1 << 15)/*INTBOUTEN*/ | (1 << 0)/*DACOUTEN*/:
                (1 << 15)/*INTBOUTEN*/ | (1 << 1)/*I2SOUTEN*/))
   {
      return 1;
   }

   return 0;
}


int si46xx_set_volume(uint8_t a)
{
   if (a > 0x3f)
   {
      a = 0x3f;
   }

   if (0 != si46xx_set_property(SI46XX_AUDIO_ANALOG_VOLUME, a & 0x3f))
   {
      return 1;
   }

   return 0;
}


static int si46xx_set_config(void)
{
   if (0 != si46xx_set_property(SI46XX_FM_SEEK_FREQUENCY_SPACING, 5/*50kHz*/))
   {
      return 1;
   }

   if (0 != si46xx_set_property(SI46XX_FM_VALID_SNR_THRESHOLD, 0))
   {
      return 1;
   }

   if (0 != si46xx_set_property(SI46XX_FM_RSQ_SNR_HIGH_THRESHOLD, 0))
   {
      return 1;
   }

   if (0 != si46xx_set_property(SI46XX_FM_RSQ_RSSI_HIGH_THRESHOLD, 0))
   {
      return 1;
   }

   if (0 != si46xx_set_property(SI46XX_FM_RDS_INTERRUPT_SOURCE, 1))
   {
      return 1;
   }

   if (0 != si46xx_set_property(SI46XX_FM_RDS_INTERRUPT_FIFO_COUNT, 0x25))
   {
      return 1;
   }

   if (0 != si46xx_set_property(SI46XX_FM_RDS_CONFIG, 1))
   {
      return 1;
   }

   if (0 != si46xx_set_property(SI46XX_FM_RDS_CONFIDENCE, 0x7777))
   {
      return 1;
   }

#if 0
   if (0 != si46xx_set_property(SI46XX_DAB_TUNE_FE_VARM, 0))
   {
      return 1;
   }

   if (0 != si46xx_set_property(SI46XX_DAB_TUNE_FE_VARB, 0))
   {
      return 1;
   }

   if (0 != si46xx_set_property(SI46XX_DAB_TUNE_FE_CFG, 0))
   {
      return 1;
   }
#endif

#if 0
   if (0 != si46xx_set_property(SI46XX_FM_INT_CTL_ENABLE,
                (1 << 2)/*RDSIEN (Interrupt when RDSINT is set)*/))
   {
      return 1;
   }
#endif

   return 0;
}


int si46xx_start_fm(uint8_t volume)
{
   si46xx_mute(1);

   HAL_Delay(1);

   if (0 != si46xx_load_and_boot(0))
   {
      return 1;
   }

   if (0 != si46xx_set_config())
   {
      return 1;
   }

   if (0 != si46xx_set_audio_output(0))
   {
      return 1;
   }

   if (0 != si46xx_set_volume(volume))
   {
      return 1;
   }

   return 0;
}


int si46xx_fm_tune_freq(uint16_t khz)
{
   si46xx_buffer[0] = SI46XX_FM_TUNE_FREQ;
   si46xx_buffer[1] = 0;
   si46xx_buffer[2] = khz;
   si46xx_buffer[3] = khz >> 8;
   si46xx_buffer[4] = 0;
   si46xx_buffer[5] = 0;

   if (si46xx_send_command(6, 4, 5) != 0)
   {
      return 1;
   }

   return 0;
}


int si46xx_get_fm_received_signal_quality(void)
{
   si46xx_buffer[0] = SI46XX_FM_RSQ_STATUS;
   si46xx_buffer[1] = 0;

   if (0 != si46xx_send_command(2, 22, 20))
   {
      return 1;
   }

   return 0;
}


int si46xx_start_dab(uint8_t volume, uint8_t mute)
{
   si46xx_mute(1);

   HAL_Delay(1);

   if (0 != si46xx_load_and_boot(1))
   {
      return 1;
   }

   if (0 != si46xx_set_dab_config())
   {
      return 1;
   }

   HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);

   if (0 != HAL_SPI_Transmit(&hspi3, Data_800bfae, 180, 10))
   {
      return 1;
   }

   HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);

   si46xx_read_reply(100, 4);

   if (0 != si46xx_set_audio_output(0))
   {
      return 1;
   }

   if (0 != si46xx_set_volume(volume))
   {
      return 1;
   }

   si46xx_mute(mute);

   return 0;
}


int si46xx_get_func_info(char* a)
{
   si46xx_buffer[0] = SI46XX_GET_FUNC_INFO;
   si46xx_buffer[1] = 0;

   if (0 != si46xx_send_command(2, 12, 10))
   {
	  return 1;
   }

   sprintf(a, "%d.%d.%d",
      si46xx_buffer[4],
      si46xx_buffer[5],
      si46xx_buffer[6]);

   return 0;
}


