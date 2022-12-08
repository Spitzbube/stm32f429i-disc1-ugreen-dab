
#include "main.h"

extern SPI_HandleTypeDef hspi5;

#define ILI9341_COLUMN_ADDRESS_SET           0x2a
#define ILI9341_PAGE_ADDRESS_SET             0x2b
#define ILI9341_MEMORY_WRITE                 0x2c


void acquire_bus(void)
{
	HAL_GPIO_WritePin(Display_CS_GPIO_Port, Display_CS_Pin, GPIO_PIN_RESET);
}


void release_bus(void)
{
	HAL_GPIO_WritePin(Display_CS_GPIO_Port, Display_CS_Pin, GPIO_PIN_SET);
}


int ili9341_write_command(uint8_t index)
{
	HAL_StatusTypeDef res;

	acquire_bus();

	HAL_GPIO_WritePin(Display_DC_GPIO_Port, Display_DC_Pin, GPIO_PIN_RESET);

	res = HAL_SPI_Transmit(&hspi5, &index, 1, 10);

	release_bus();

    if (res != HAL_OK)
    {
       return 1;
    }

    return 0;
}

int ili9341_write_data(uint8_t data)
{
	HAL_StatusTypeDef res;

	acquire_bus();

	HAL_GPIO_WritePin(Display_DC_GPIO_Port, Display_DC_Pin, GPIO_PIN_SET);

	res = HAL_SPI_Transmit(&hspi5, &data, 1, 10);

	release_bus();

    if (res != HAL_OK)
    {
       return 1;
    }

    return 0;
}

int ili9341_write_data16(uint16_t data)
{
	HAL_StatusTypeDef res;
	uint8_t buffer[2];

	acquire_bus();

	HAL_GPIO_WritePin(Display_DC_GPIO_Port, Display_DC_Pin, GPIO_PIN_SET);

	buffer[0] = (data >> 8) & 0xFF;
	buffer[1] = (data >> 0) & 0xFF;

	res = HAL_SPI_Transmit(&hspi5, buffer, 2, 10);

	release_bus();

    if (res != HAL_OK)
    {
       return 1;
    }

    return 0;
}

int ili9341_write_data_block(uint8_t data[], uint16_t size)
{
	HAL_StatusTypeDef res;

	acquire_bus();

	HAL_GPIO_WritePin(Display_DC_GPIO_Port, Display_DC_Pin, GPIO_PIN_SET);

	res = HAL_SPI_Transmit(&hspi5, data, size, 10);

	release_bus();

    if (res != HAL_OK)
    {
       return 1;
    }

    return 0;
}

void ili9341_set_address(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
   uint8_t data[4];

   ili9341_write_command(ILI9341_COLUMN_ADDRESS_SET);

   data[0] = x1 >> 8;
   data[1] = x1 & 0xff;
   data[2] = x2 >> 8;
   data[3] = x2 & 0xff;

   ili9341_write_data_block(data, sizeof(data));

   ili9341_write_command(ILI9341_PAGE_ADDRESS_SET);

   data[0] = y1 >> 8;
   data[1] = y1 & 0xff;
   data[2] = y2 >> 8;
   data[3] = y2 & 0xff;

   ili9341_write_data_block(data, sizeof(data));

   ili9341_write_command(ILI9341_MEMORY_WRITE);
}

void ili9341_fill_screen(uint16_t a)
{
   uint32_t count;

   ili9341_set_address(0, 0, 319, 239);

   count = 320 * 240;
   while (count--)
   {
      ili9341_write_data16(a);
   }
}

void ili9341_draw_pixel(uint16_t a, uint16_t b, uint16_t c)
{
#if 0
   if ((a >= ScreenResolution.width) ||
		   (b >= ScreenResolution.height))
   {
	   return;
   }
#endif

   ili9341_set_address(a, b, a, b);
   ili9341_write_data16(c);
}

void ili9341_draw_hor_line(uint16_t x1, uint16_t x2, uint16_t y, uint16_t color)
{
   int i;

   if (x2 < x1)
   {
      uint16_t x = x1;
      x1 = x2;
      x2 = x;
   }

#if 0
   if (x2 >= ScreenResolution.width)
   {
      x2 = ScreenResolution.width - 1;
   }

   if (x1 >= ScreenResolution.width)
   {
      x1 = ScreenResolution.width - 1;
   }
#endif

   ili9341_set_address(x1, y, x2, y);

   for (i = x1; i <= x2; i++)
   {
	   ili9341_write_data16(color);
   }
}


/* 8005648 - todo */
void ili9341_draw_vert_line(uint16_t x, uint16_t y1, uint16_t y2, uint16_t color)
{
   int i;

   if (y2 < y1)
   {
      uint16_t y = y1;
      y1 = y2;
      y2 = y;
   }

#if 0
   if (x >= ScreenResolution.width)
   {
      x = ScreenResolution.width - 1;
   }

   if (y1 >= ScreenResolution.height)
   {
      y1 = ScreenResolution.height - 1;
   }

   if (y2 >= ScreenResolution.height)
   {
      y2 = ScreenResolution.height - 1;
   }
#endif

   ili9341_set_address(x, y1, x, y2);

   for (i = y1; i <= y2; i++)
   {
      ili9341_write_data16(color);
   }
}

void ili9341_setup_interface(void)
{
	// Hardware reset
	HAL_GPIO_WritePin(Display_Reset_GPIO_Port, Display_Reset_Pin, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(Display_Reset_GPIO_Port, Display_Reset_Pin, GPIO_PIN_SET);
	HAL_Delay(20);

	ili9341_write_command(0x01); //software reset
	HAL_Delay(50);
}


void ili9341_init(void)
{
	ili9341_setup_interface();

	ili9341_write_command(0x28);
	// display off

	ili9341_write_command(0xcf);
	ili9341_write_data(0x00);
	ili9341_write_data(0x83);
	ili9341_write_data(0x30);

	ili9341_write_command(0xed);
	ili9341_write_data(0x64);
	ili9341_write_data(0x03);
	ili9341_write_data(0x12);
	ili9341_write_data(0x81);
	ili9341_write_command(0xe8);
	ili9341_write_data(0x85);
	ili9341_write_data(0x01);
	ili9341_write_data(0x79);
	ili9341_write_command(0xcb);
	ili9341_write_data(0x39);
	ili9341_write_data(0x2c);
	ili9341_write_data(0x00);
	ili9341_write_data(0x34);
	ili9341_write_data(0x02);
	ili9341_write_command(0xf7);
	ili9341_write_data(0x20);
	ili9341_write_command(0xea);
	ili9341_write_data(0x00);
	ili9341_write_data(0x00);
	//------------power control------------------------------
	ili9341_write_command(0xc0); //power control
	ili9341_write_data(0x26);
	ili9341_write_command(0xc1); //power control
	ili9341_write_data(0x11);
	//--------------VCOM
	ili9341_write_command(0xc5); //vcom control
	ili9341_write_data(0x35);//35
	ili9341_write_data(0x3e);//3E
	ili9341_write_command(0xc7); //vcom control
	ili9341_write_data(0xbe); // 0x94
	//------------memory access control------------------------
	ili9341_write_command(0x36);
	// memory access control
	ili9341_write_data(0xE8); //0048 my,mx,mv,ml,BGR,mh,0.0
	ili9341_write_command(0x3a); // pixel format set
	ili9341_write_data(0x55);//16bit /pixel
	//----------------- frame rate------------------------------
	ili9341_write_command(0xb1);
	// frame rate
	ili9341_write_data(0x00);
	ili9341_write_data(0x1B); //70
	//----------------Gamma---------------------------------
	ili9341_write_command(0xf2); // 3Gamma Function Disable
	ili9341_write_data(0x08);
	ili9341_write_command(0x26);
	ili9341_write_data(0x01); // gamma set 4 gamma curve 01/02/04/08

	ili9341_write_command(0xE0); //positive gamma correction
	ili9341_write_data(0x1f);
	ili9341_write_data(0x1a);
	ili9341_write_data(0x18);
	ili9341_write_data(0x0a);
	ili9341_write_data(0x0f);
	ili9341_write_data(0x06);
	ili9341_write_data(0x45);
	ili9341_write_data(0x87);
	ili9341_write_data(0x32);
	ili9341_write_data(0x0a);
	ili9341_write_data(0x07);
	ili9341_write_data(0x02);
	ili9341_write_data(0x07);
	ili9341_write_data(0x05);
	ili9341_write_data(0x00);
	ili9341_write_command(0xE1); //negamma correction
	ili9341_write_data(0x00);
	ili9341_write_data(0x25);
	ili9341_write_data(0x27);
	ili9341_write_data(0x05);
	ili9341_write_data(0x10);
	ili9341_write_data(0x09);
	ili9341_write_data(0x3a);
	ili9341_write_data(0x78);
	ili9341_write_data(0x4d);
	ili9341_write_data(0x05);
	ili9341_write_data(0x18);
	ili9341_write_data(0x0d);
	ili9341_write_data(0x38);
	ili9341_write_data(0x3a);
	ili9341_write_data(0x1f);
	//--------------ddram ---------------------
	ili9341_write_command(ILI9341_COLUMN_ADDRESS_SET);
	// column set
	// size = 239
	ili9341_write_data(0x00);
	ili9341_write_data(0x00);
	ili9341_write_data(0x00);
	ili9341_write_data(0xEF);
	ili9341_write_command(ILI9341_PAGE_ADDRESS_SET);
	// page address set
	// size = 319
	ili9341_write_data(0x00);
	ili9341_write_data(0x00);
	ili9341_write_data(0x01);
	ili9341_write_data(0x3F);
	// ili9341_write_command(0x34);
	//ili9341_write_command(0x35);
	// tearing effect off
	// tearing effect on
	// ili9341_write_command(0xb4); // display inversion
	// ili9341_write_data(0x00);
	ili9341_write_command(0xb7); //entry mode set
	ili9341_write_data(0x07);
	//-----------------display---------------------
	ili9341_write_command(0xb6);
	// display function control
	ili9341_write_data(0x0a);
	ili9341_write_data(0x82);
	ili9341_write_data(0x27);
	ili9341_write_data(0x00);
	ili9341_write_command(0x11); //sleep out
	HAL_Delay(100);
	ili9341_write_command(0x29); // display on
	HAL_Delay(100);
}
