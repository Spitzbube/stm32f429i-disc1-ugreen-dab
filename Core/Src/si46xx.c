
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "cmsis_os.h"

#if 0
extern const uint8_t fw_rom00_patch016[5796];
extern const uint8_t fw_dab_radio_5_0_5[521448];
extern const uint8_t fw_dab_radio_6_0_6[496940];
extern const uint8_t fw_fmhd_radio_5_0_4[530180];
#else
#define SI46XX_DAB_FW_ADDR 					((void*)0x08180000)
#define SI46XX_DAB_FW_SIZE 					496940
#define SI46XX_FM_FW_ADDR					((void*)0x080F0000)
#define SI46XX_FM_FW_SIZE					530716
#endif

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
#define SI46XX_FM_RDS_STATUS                 0x34

#define SI46XX_GET_DIGITAL_SERVICE_LIST      0x80
#define SI46XX_DAB_START_DIGITAL_SERVICE     0x81
#define SI46XX_GET_DIGITAL_SERVICE_DATA      0x84
#define SI46XX_DAB_TUNE_FREQ                 0xB0
#define SI46XX_DAB_DIGRAD_STATUS             0xB2
#define SI46XX_DAB_GET_EVENT_STATUS          0xB3

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


int si46xx_read_stc_reply(uint16_t a, uint16_t b)
{
   uint8_t i = 20;

   do
   {
      i--;

      HAL_Delay(a);

      /* CS to low */
      HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);

      si46xx_buffer[0] = SI46XX_RD_REPLY;

      if (0 != HAL_SPI_Transmit(&hspi3, si46xx_buffer, 1, 10))
      {
         return 0;
      }

      if (0 != HAL_SPI_Receive(&hspi3, si46xx_buffer, b, 10))
      {
         return 0;
      }

      /* CS to high */
      HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
   }
   while (((si46xx_buffer[0] & 1/*STCINT*/) == 0) && (i != 0));

   if ((i == 0) || (si46xx_buffer[0] & 0x40)) //ERR_CMD
   {
      return 1;
   }

   return 0;
}


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
#if 0
	   if (0 != si46xx_write_host_load_data(fw_dab_radio_6_0_6, sizeof(fw_dab_radio_6_0_6)))
#else
	   if (0 != si46xx_write_host_load_data(SI46XX_DAB_FW_ADDR, SI46XX_DAB_FW_SIZE))
#endif
	   {
		  return 1;
	   }
   }
   else
   {
#if 0
	   if (0 != si46xx_write_host_load_data(fw_fmhd_radio_5_0_4, sizeof(fw_fmhd_radio_5_0_4)))
#else
	   if (0 != si46xx_write_host_load_data(SI46XX_FM_FW_ADDR, SI46XX_FM_FW_SIZE))
#endif
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

#if 0
   if (0 != si46xx_set_property(SI46XX_FM_INT_CTL_ENABLE,
               (1 << 4)/*DSRVIEN (Interrupt when DSRVINT is set)*/))
   {
      return 1;
   }
#endif

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

   if (0 != si46xx_set_property(SI46XX_FM_INT_CTL_ENABLE,
                (1 << 2)/*RDSIEN (Interrupt when RDSINT is set)*/))
   {
      return 1;
   }

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

#if 1
   HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);

   if (0 != HAL_SPI_Transmit(&hspi3, Data_800bfae, 180, 10))
   {
      return 1;
   }

   HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);

   si46xx_read_reply(100, 4);
#endif

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


int si46xx_get_fm_rds_status(uint8_t arg1, uint8_t retry)
{
   uint8_t i = 0;

   do
   {
      si46xx_buffer[0] = SI46XX_FM_RDS_STATUS;
      si46xx_buffer[1] = arg1 & 0x07; //STATUSONLY MTFIFO INTACK

      if (0 != si46xx_send_command(2, 20, 1))
      {
         return 1;
      }

      i++;
   }
   while (((si46xx_buffer[5] & (1 << 1)/*RDSSYNC*/) == 0) && (i < retry));

   if (i == retry)
   {
      return 1;
   }

   return 0;
}


void si46xx_get_rds_time(RTC_TimeTypeDef* sTime, RTC_DateTypeDef* sDate)
{
   uint8_t m; //r7_27
   uint8_t h; //r7_26
   uint32_t mjd; //r7_20
   uint8_t r7_1f;
   uint32_t l; //r7_18
   uint32_t n; //r7_14
   uint32_t i; //r7_10
   uint32_t j; //r7_c
   uint16_t y; //r7_a
   uint8_t r7_9;
   uint8_t r7_8;

   if (((si46xx_buffer[11] & 0x03) < 3) &&
		   ((si46xx_buffer[11] & 0x0c) < 12) &&
		   ((si46xx_buffer[11] & 0x30) < 48) &&
		   ((si46xx_buffer[11] & 0xc0) < 192))
   {
      mjd = ((si46xx_buffer[16] >> 1) & 0x7f) |
			   (si46xx_buffer[17] << 7) |
			   ((si46xx_buffer[14] << 15) & 0x18000);
      r7_1f = si46xx_buffer[18] & 0x3f;
      m = (si46xx_buffer[18] >> 6) | ((si46xx_buffer[19] << 2) & 0x3c);
      h = (si46xx_buffer[19] >> 4) | ((si46xx_buffer[16] << 4) & 0x10);

      l = mjd + 2468570;
      n = l * 4 / 146097;
      l -= (n * 146097 + 3) / 4;
      i = (l + 1) * 4000 / 1461001;
      l -= i * 1461 / 4 - 31;
      j = l * 80 / 2447;
      sDate->Date = l - (j * 2447 / 80);

      l = j / 11;
      sDate->Month = j - l * 12 + 2;

      y = 100 * (n - 49) + i + l; //TODO

      if ((r7_1f & 0x20) != 0)
      {
    	  r7_9 = (r7_1f & 1) * 30;
    	  r7_8 = (r7_9 > m)? 1: 0;

    	  if (r7_9 > m)
    	  {
    		  m -= r7_9 - 59;
    	  }
    	  else
    	  {
    		  m -= r7_9;
    	  }

    	  r7_9 = (r7_1f & 0x1f) / 2 + r7_8;

    	  if (r7_9 > h)
    	  {
    		  h -= r7_9 - 23;
    	  }
    	  else
    	  {
    		  h -= r7_9;
    	  }
      }
      else
      {
    	  r7_9 = (r7_1f & 1) * 30;
    	  r7_8 = ((r7_9 + m) > 59)? 1: 0;

    	  if ((r7_9 + m) > 59)
    	  {
    		  m += r7_9 - 59;
    	  }
    	  else
    	  {
    		  m += r7_9;
    	  }

    	  r7_9 = (r7_1f & 0x1f) / 2 + r7_8;

    	  if ((r7_9 + h) > 23)
    	  {
    		  h += r7_9 - 23;
    	  }
    	  else
    	  {
    		  h += r7_9;
    	  }
      }

      sDate->Year = y + 48;
      sTime->Hours = h;
      sTime->Minutes = m;
      sTime->Seconds = 0;

      sDate->WeekDay = mjd % 7 + 3;
      if (sDate->WeekDay > 7)
      {
    	  sDate->WeekDay -= 7;
      }
   }
}


uint8_t g_bGroup2AMaxAddress;
uint32_t g_dwGroup2AFlagsLow;
uint32_t g_dwGroup2AFlagsHigh;
uint8_t g_bGroup0AFlags;
uint8_t g_bGroup0ABuffer[8];
uint8_t g_bGroup2ABuffer[64];


int si46xx_fm_get_rds_data(void* pStationName, uint8_t* pRadioText, RTC_TimeTypeDef* sTime, RTC_DateTypeDef* sDate, uint16_t* pwMainloopEvents, uint8_t* pbRadioTextLength)
{
   uint8_t i;
   uint8_t* pByte;
   uint8_t resultMask;
   uint32_t dwGroup2AFlagsHighExpected;
   uint8_t addr;

   resultMask = 0;

   if (*pwMainloopEvents != 0)
   {
      g_bGroup2AMaxAddress = 0;
      g_dwGroup2AFlagsLow = 0;
      g_dwGroup2AFlagsHigh = 0;
   }

   if (si46xx_buffer[10]/*RDSFIFOUSED*/ != 0)
   {
	 switch (si46xx_buffer[15]/*BLOCKB[15:8]*/ & 0xf8)
	 {
		case 0:
		case 0x08:
			//group 0A
		   if (pStationName != NULL)
		   {
			  addr = (si46xx_buffer[14] << 1) & 0x06;

			  if ((addr == 0) || ((*pwMainloopEvents & 0x40) != 0))
			  {
				 g_bGroup0AFlags = 0;
				 *pwMainloopEvents &= ~0x40;
			  }

			  if (((si46xx_buffer[11]) & 0x03) < 3)
			  {
				 pByte = &g_bGroup0ABuffer[addr];
				 *pByte = si46xx_buffer[19];
				 g_bGroup0AFlags |= (1 << addr);
			  }

			  if (((si46xx_buffer[11]) & 0x0c) < 12)
			  {
				 pByte = &g_bGroup0ABuffer[addr + 1];
				 *pByte = si46xx_buffer[18];
				 g_bGroup0AFlags |= (1 << (addr + 1));
			  }

			  printf("Group 0A Addr=%d, Flags=0x%02x\r\n", addr, g_bGroup0AFlags);

			  if (g_bGroup0AFlags == 0xff)
			  {
				  pByte = pStationName;
				  for (i = 0; i < 8; i++)
				  {
					 *pByte++ = /*Data_8018ea4[*/ g_bGroup0ABuffer[i] /*]*/;
				  }

				  resultMask |= 0x02;
				  g_bGroup0AFlags = 0;
			  }
		   } //if (pStationName != NULL)
		   break;

		case 0x20:
		case 0x28:
		   // group 2A
		   if ((pbRadioTextLength != NULL) && (pRadioText != NULL))
		   {
			  addr = (si46xx_buffer[14] << 2) & 0x3c;

			  if (addr == 0)
			  {
				 g_dwGroup2AFlagsLow = 0;
				 g_dwGroup2AFlagsHigh = 0;
			  }

			  if ((si46xx_buffer[11] & 0x03) < 3)
			  {
				 pByte = &g_bGroup2ABuffer[addr];
				 *pByte = si46xx_buffer[17];

				 if (addr > 31)
				 {
					g_dwGroup2AFlagsHigh |= (1 << (addr - 32));
				 }
				 else
				 {
					g_dwGroup2AFlagsLow |= (1 << addr);
				 }
			  }

			  if ((si46xx_buffer[11] & 0x0c) < 12)
			  {
				 pByte = &g_bGroup2ABuffer[addr + 1];
				 *pByte = si46xx_buffer[16];

				 if (addr > 31)
				 {
					g_dwGroup2AFlagsHigh |= (1 << (addr - 31));
				 }
				 else
				 {
					g_dwGroup2AFlagsLow |= (1 << (addr + 1));
				 }
			  }

			  if ((si46xx_buffer[11] & 0x30) < 48)
			  {
				 pByte = &g_bGroup2ABuffer[addr + 2];
				 *pByte = si46xx_buffer[19];

				 if (addr > 31)
				 {
					g_dwGroup2AFlagsHigh |= (1 << (addr - 30));
				 }
				 else
				 {
					g_dwGroup2AFlagsLow |= (1 << (addr + 2));
				 }
			  }

			  if ((si46xx_buffer[11] & 0xC0) < 192)
			  {
				 pByte = &g_bGroup2ABuffer[addr + 3];
				 *pByte = si46xx_buffer[18];

				 if (addr > 31)
				 {
					g_dwGroup2AFlagsHigh |= (1 << (addr - 29));
				 }
				 else
				 {
					g_dwGroup2AFlagsLow |= (1 << (addr + 3));
				 }
			  }

			  if (g_bGroup2AMaxAddress < addr)
			  {
				 g_bGroup2AMaxAddress = addr;
			  }

			  dwGroup2AFlagsHighExpected = 1;

			  for (i = 0; i < (uint8_t)(g_bGroup2AMaxAddress - 29); i++)
			  {
				 dwGroup2AFlagsHighExpected = (dwGroup2AFlagsHighExpected << 1) + 1;
			  }

			  printf("Group 2A Addr=%d, Flags=0x%08x_%08x\r\n", addr, g_dwGroup2AFlagsHigh, g_dwGroup2AFlagsLow);

			  if ((g_dwGroup2AFlagsLow == 0xffffffff) &&
					  (g_dwGroup2AFlagsHigh == dwGroup2AFlagsHighExpected))
			  {
				 *pbRadioTextLength = g_bGroup2AMaxAddress + 4;

				 for (i = 0; i < *pbRadioTextLength; i++)
				 {
					pRadioText[i] = /*Data_8018ea4[*/ g_bGroup2ABuffer[i] /*]*/;
				 }

				 resultMask |= 0x04;
				 g_dwGroup2AFlagsLow = 0;
				 g_dwGroup2AFlagsHigh = 0;

				 for (i = 0; i < (uint8_t)(*pbRadioTextLength - 3); i++)
				 {
					if ((pRadioText[i] == ' ') &&
							(pRadioText[i + 1] == ' ') &&
							(pRadioText[i + 2] == ' '))
					{
					   *pbRadioTextLength = i + 1;
					   break;
					}
				 }

				 resultMask |= 0x04;
			  }
		   } //if ((pbRadioTextLength != NULL) && (pRadioText != NULL))
		   break;

		case 0x40:
		   si46xx_get_rds_time(sTime, sDate);

#if 0
		   HAL_RTC_SetTime(&hrtc, sTime, RTC_FORMAT_BIN);
		   HAL_RTC_SetDate(&hrtc, sDate, RTC_FORMAT_BIN);
#endif

		   *pwMainloopEvents |= 0x80;
		   resultMask |= 0x08;
		   break;

		default:
		   break;

	 } //switch (si46xx_buffer[15] & 0xf8)
   } //if (si46xx_buffer[10] != 0)

   return resultMask;
}


int si46xx_dab_tune_freq(uint8_t index)
{
   uint8_t f;

   si46xx_buffer[0] = SI46XX_DAB_TUNE_FREQ;
   si46xx_buffer[1] = 0x00;
   si46xx_buffer[2] = index;
   si46xx_buffer[3] = 0x00;
   si46xx_buffer[4] = 0x00;
   si46xx_buffer[5] = 0x00;

   if (0 != si46xx_send_command(6, 4, 5))
   {
      return 1;
   }

   f = si46xx_read_stc_reply(200, 4);
   f |= si46xx_read_reply(100, 4);

   return f;
}


int si46xx_get_dab_status(void)
{
   si46xx_buffer[0] = SI46XX_DAB_DIGRAD_STATUS;
   si46xx_buffer[1] = 0;

   if (0 != si46xx_send_command(2, 23, 20))
   {
	  return 1;
   }

   return 0;
}


int sub_8009f70(uint8_t* a)
{
   si46xx_buffer[1] = 0x80;
   si46xx_buffer[0] = SI46XX_RD_REPLY;

   if (0 != si46xx_send_command(2, 9, 1))
   {
	  return 1;
   }

   *a = si46xx_buffer[8];

   return 0;
}


int sub_8009f1c(uint8_t a)
{
   while (a-- != 0)
   {
      si46xx_buffer[0] = SI46XX_DAB_GET_EVENT_STATUS;
      si46xx_buffer[1] = 0;

      (void) si46xx_send_command(2, 12, 10);

      if ((si46xx_buffer[5] & 1/*SVRLIST*/) != 0)
      {
         break;
      }
   }

   if (a == 0)
   {
      return 1;
   }

   return 0;
}


typedef struct
{
	int8_t rssi;
	int8_t snr;

} Tuner_Values;

int si46xx_get_dab_values(Tuner_Values* a)
{
   int8_t rssi;
   int8_t snr;

   if (0 != si46xx_get_dab_status())
   {
      return 1;
   }

   rssi = si46xx_buffer[6];
   snr = si46xx_buffer[7];

   a->rssi = rssi;
   a->snr = snr;
#if 0
   a->fib_error_count = si46xx_buffer[10] | (si46xx_buffer[11] << 8);
   a->freq_index = si46xx_buffer[16];
   a->frequency = si46xx_buffer[12] | (si46xx_buffer[13] << 8) |
		 (si46xx_buffer[14] << 16)  | (si46xx_buffer[15] << 24);
#endif
}


typedef struct
{
	uint8_t arData[16];
	//16
} struct_8008d84_Inner8;

typedef struct
{
   uint32_t service_id; //0
   uint32_t component_id; //4
   struct_8008d84_Inner8 Data_8; //8
   uint16_t wData_24; //24
   //28
} struct_8008d84;


uint8_t bChannelCount;
struct_8008d84 ChannelList[200];


int sub_800b8d4(uint8_t r7_f, uint32_t r7_8, uint32_t r7_4, uint8_t* r7)
{
   uint8_t r7_17 = 0;
   uint8_t i;

   printf("sub_800b8d4: r7_f=%d, r7_8=%d, r7_4=%d, r7='%s'\r\n", r7_f, r7_8, r7_4, r7);

   if (bChannelCount < 200)
   {
      for (i = 0; i < bChannelCount; i++)
      {
         //loc_800b8f6
         if ((ChannelList[i].wData_24 == r7_f) &&
        		 (ChannelList[i].service_id == r7_8))
         {
            r7_17 = 2;
         }
         //loc_800b92a
      }

      if (r7_17 == 0)
      {
         ChannelList[bChannelCount].wData_24 = r7_f;
         ChannelList[bChannelCount].service_id = r7_8;
         ChannelList[bChannelCount].component_id = r7_4;
         memset(&ChannelList[bChannelCount].Data_8, ' ', 16);
         memcpy(&ChannelList[bChannelCount].Data_8, r7, 16);

         bChannelCount++;
         //->loc_800b9d8
      }
      //loc_800b9d8
   }
   else
   {
      //loc_800b9d4
      r7_17 = 1;
   }

   return r7_17;
}


static int si46xx_get_digital_service_list(uint8_t mux, uint16_t* r7)
{
   uint8_t* r7_24 = &si46xx_buffer[12];
   uint8_t r7_23;
   uint8_t i; //r7_22
   uint8_t r7_21;
   uint8_t r7_20;
   uint32_t r7_1c;
   uint32_t r7_18;
   uint8_t r7_8[16];

   si46xx_buffer[0] = SI46XX_GET_DIGITAL_SERVICE_LIST;
   si46xx_buffer[1] = 0;

   if (0 != si46xx_send_command(2, 2048, 1))
   {
      return 1;
   }

   *r7 = si46xx_buffer[6] | (si46xx_buffer[7] << 8);
   r7_20 = si46xx_buffer[8];

   for (i = 0; i < r7_20; i++)
   {
      //loc_800a00a
      r7_23 = r7_24[5] & 0x0f;

      if ((r7_24[4] & 0x01) == 0)
      {
         r7_1c = r7_24[0] | (r7_24[1] << 8) | (r7_24[2] << 16) | (r7_24[3] << 24);
         r7_24 += 4;
         r7_24 += 4;
         memcpy(r7_8, r7_24, 16);
         r7_24 += 16;
         r7_18 = r7_24[0] | (r7_24[1] << 8) | (r7_24[2] << 16) | (r7_24[3] << 24);
         r7_24 += 4;

         r7_21 = sub_800b8d4(mux, r7_1c, r7_18, r7_8);

         if ((r7_21 != 1) && (r7_23 > 2))
         {
            r7_23--;
            do
            {
               //loc_800a0ca
               if ((r7_24[1] & 0xc0) == 0)
               {
                  r7_18 = r7_24[0] | (r7_24[1] << 8) | (r7_24[2] << 16) | (r7_24[3] << 24);
                  r7_21 = sub_800b8d4(mux, r7_1c, r7_18, r7_8);
               }
               //loc_800a112
               r7_24 += 4;
               r7_23--;

               if (r7_21 == 1)
               {
            	   //->loc_800a146
            	   break;
               }
            }
            while (r7_23 != 0);
         }
         //loc_800a148
      }
      else
      {
         //loc_800a134
         r7_24 += 4*(r7_23 + 6);
      }
   }

   return 0;
}


int si46xx_dab_search(uint8_t* r7_4)
{
   uint8_t mux = 0; //r7_27
   uint8_t r7_26;
   uint8_t r7_25;
   uint16_t r7_22;
   uint8_t r7_21;
   uint8_t r7_20;
   uint8_t r7_1f = bChannelCount;
   uint16_t r7_1c;
   uint8_t r7_1b;

   r7_26 = bChannelCount;
   r7_25 = 0;

   Tuner_Values r7_8 = {0};

#if 0
   TouchEvent.bData_0 = 1;
   KeyEvent.bData_0 = 1;
#endif

   do
   {
	   //loc_8008b54
//	   osDelay(1000);
	   if (0 != si46xx_dab_tune_freq(mux))
	   {
		   //->loc_8008cd2
		   continue;
	   }
	   else
	   {
#if 0
		  if (TouchEvent.bData_0 == 0)
		  {
			 r7_21 = 0;
			 if (KeyEvent.bData_0 == 0)
			 {
				r7_21 = KeyEvent.bData_1;
				KeyEvent.bData_0 = 1;
			 }
			 //loc_8008b8a
			 r7_20 = 0;
			 if (TouchEvent.bData_0 == 0)
			 {
				r7_20 = menu_automatic_search_check_touch_fields(TouchEvent.wData_2, TouchEvent.wData_4);
			 }
			 //loc_8008bae
			 TouchEvent.bData_0 = 1;
			 KeyEvent.bData_0 = 1;

			 if ((r7_20 | r7_21) == 5)
			 {
				return 1;
			 }
			 //loc_8008bce
		  }
		  //loc_8008bce
		  draw_freq_mux_label(mux);
		  draw_progress_bar(242, 42, mux, 41);
#endif

		  if (bChannelCount != r7_26)
		  {
			 r7_26 = bChannelCount;

#if 0
			 draw_channel_list(ChannelList, bChannelCount);
#endif
		  }
		  //loc_8008c0a
		  si46xx_get_dab_values(&r7_8);
#if 0
		  draw_signal_strength_bars(142, 42, &r7_8.rssi);
#else
		  printf("mux=%d, rssi=%d, snr=%d\r\n", mux, r7_8.rssi, r7_8.snr);
#endif
		  if (0 != si46xx_get_dab_status())
		  {
			 return 1;
		  }
		  //loc_8008c30
		  if (((si46xx_buffer[5] & 1) != 0) &&
				  (si46xx_buffer[8] > 29))
		  {
			  r7_1b = 0;
			  r7_25 = 0;
			  do
			  {
				  //loc_8008c4e
				  r7_22 = 0;
				  do
				  {
					  //loc_8008c52
					  sub_8009f1c(100);

#if 0
					  if (TouchEvent.bData_0 == 0)
					  {
						  TouchEvent.bData_0 = 1;
						  if (5 == menu_automatic_search_check_touch_fields(TouchEvent.wData_2, TouchEvent.wData_4))
						  {
							  return 1;
						  }
						  //loc_8008c80
					  }
					  //loc_8008c80
#endif
				  }
				  while (((si46xx_buffer[5] & 1) == 0) && (r7_22++ < 3600));
				  //loc_8008c98
				  sub_8009f70(&r7_1b);
			  }
			  while ((r7_1b == 0) && (++r7_25 < 10));
			  //loc_8008cba
			  sub_8009f1c(10);

			  si46xx_get_digital_service_list(mux, &r7_1c);
			  //->loc_8008cd4
		  }
		  //loc_8008cd4
	   }
	   //loc_8008cd4
   }
   while (++mux < 41);

   *r7_4 = bChannelCount - r7_1f;

   return 0;
}


int si46xx_dab_start_digital_service(struct_8008d84* a)
{
   if (si46xx_dab_tune_freq(a->wData_24) != 0)
   {
	  return 1;
   }

   si46xx_buffer[0] = SI46XX_DAB_START_DIGITAL_SERVICE;
   si46xx_buffer[1] = 0;
   si46xx_buffer[2] = 0;
   si46xx_buffer[3] = 0;
   si46xx_buffer[4] = a->service_id;
   si46xx_buffer[5] = a->service_id >> 8;
   si46xx_buffer[6] = a->service_id >> 16;
   si46xx_buffer[7] = a->service_id >> 24;
   si46xx_buffer[8] = a->component_id;
   si46xx_buffer[9] = a->component_id >> 8;
   si46xx_buffer[10] = a->component_id >> 16;
   si46xx_buffer[11] = a->component_id >> 24;

   if (si46xx_send_command(12, 4, 10) != 0)
   {
      return 1;
   }

   return 0;
}


static TaskHandle_t irqTaskHandle;


void si46xx_handle_interrupt(void)
{
	BaseType_t xYieldRequired = pdFALSE;

	if (irqTaskHandle != NULL)
	{
		xYieldRequired = xTaskResumeFromISR(irqTaskHandle);
		if (xYieldRequired == pdTRUE)
		{
			portYIELD_FROM_ISR(irqTaskHandle);
		}
	}
}


void si46xx_interrupt_task(void* pTaskData)
{
	while (1)
	{
		vTaskSuspend(NULL); //wait for Interrupt

		if (0 == si46xx_read_reply(0, 4))
		{
			if ((si46xx_buffer[0] & (1 << 2)/*RDSINT*/) != 0)
			{
				if (0 == si46xx_get_fm_rds_status(1/*INTACK*/, 2))
				{
#if 0
					   uint8_t tp = (si46xx_buffer[6] & (1 << 5))? 1: 0;
					   uint8_t pty = si46xx_buffer[6] & 0x1f;
					   uint16_t pi = si46xx_buffer[8] | (si46xx_buffer[9] << 8);
					   uint8_t rds_fifo = si46xx_buffer[10];
					   uint8_t blea = (si46xx_buffer[11] >> 6) & 0x03;
					   uint8_t bleb = (si46xx_buffer[11] >> 4) & 0x03;
					   uint8_t blec = (si46xx_buffer[11] >> 2) & 0x03;
					   uint8_t bled = (si46xx_buffer[11] >> 0) & 0x03;
#endif
					   uint16_t blocka = si46xx_buffer[12] | (si46xx_buffer[13] << 8);
					   uint16_t blockb = si46xx_buffer[14] | (si46xx_buffer[15] << 8);
					   uint16_t blockc = si46xx_buffer[16] | (si46xx_buffer[17] << 8);
					   uint16_t blockd = si46xx_buffer[18] | (si46xx_buffer[19] << 8);

#if 0
					   printf("RDS: tp=%d, pty=0x%x, pi=0x%04x\r\n", tp, pty, pi);
					   printf("RDS: fifo=%d, errors a=%d, b=%d, c=%d, d=%d\r\n", rds_fifo, blea, bleb, blec, bled);
					   printf("RDS: block a=0x%04x, b=0x%04x, c=0x%04x, d=0x%04x\r\n", blocka, blockb, blockc, blockd);
#endif

					   uint8_t bRadioTextLength;
					   uint8_t radioText[64];
					   uint8_t stationName[8];
					   uint16_t wMainloopEvents = 0;
					   RTC_TimeTypeDef rtcTime;
					   RTC_DateTypeDef rtcDate;

					   int resultMask = si46xx_fm_get_rds_data(stationName, radioText, &rtcTime, &rtcDate, &wMainloopEvents, &bRadioTextLength);
					   if (resultMask & 2)
					   {
						   printf("stationName = '%s'\r\n", stationName);
					   }

					   if (resultMask & 4)
					   {
						   printf("radioText = '%s'\r\n", radioText);
					   }

					   if (resultMask & 8)
					   {
						   printf("Time = %d:%d:%d, %d.%d.%d\r\n", rtcTime.Hours, rtcTime.Minutes, rtcTime.Seconds, rtcDate.Date, rtcDate.Month, rtcDate.Year);
					   }
				}
			}
		}
	}
}


void si46xx_task(void* pTaskData)
{
	vTaskSuspend(NULL); //wait for Startup

	xTaskCreate(si46xx_interrupt_task, "Si46xxIrqTask", 10*configMINIMAL_STACK_SIZE, NULL, 1, &irqTaskHandle);

	si46xx_start_dab(0, 0);
	uint8_t a;
	si46xx_dab_search(&a);

	si46xx_dab_start_digital_service(&ChannelList[0]);

	si46xx_set_volume(48);

#if 0
	si46xx_start_fm(0);

//	si46xx_fm_tune_freq(10240);
	si46xx_fm_tune_freq(10530);

	si46xx_set_volume(48);
#endif

	while (1)
	{
#if 0
		if (0 == si46xx_get_fm_received_signal_quality())
		{
			int8_t rssi = si46xx_buffer[9]; //RSSI
			int8_t snr = si46xx_buffer[10]; //SNR
			uint8_t multipath = si46xx_buffer[11]; //MULT
			uint32_t freq = si46xx_buffer[6]; //READFREQ
			freq |= (si46xx_buffer[7] << 8); //READFREQ
			uint8_t freq_offset = si46xx_buffer[8]; //FREQOFF

			printf("FM(%u): rssi=%d, snr=%d, multipath=%d \r\n", freq, rssi, snr, multipath);
		}
#else

		   Tuner_Values r7_8 = {0};

		  si46xx_get_dab_values(&r7_8);
		  printf("rssi=%d, snr=%d\r\n", r7_8.rssi, r7_8.snr);

#endif

		HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);

		osDelay(1000);
	}
}



