#include "sd.h"
//--------------------------------------------------
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;
//--------------------------------------------------
extern volatile uint16_t Timer1;
sd_info_ptr sdinfo;
//-----------------------------------------------
// Definitions for MMC/SDC command
#define CMD0 (0x40+0) // GO_IDLE_STATE
#define CMD1 (0x40+1) // SEND_OP_COND (MMC)
#define ACMD41 (0xC0+41) // SEND_OP_COND (SDC)
#define CMD8 (0x40+8) // SEND_IF_COND
#define CMD9 (0x40+9) // SEND_CSD
#define CMD16 (0x40+16) // SET_BLOCKLEN
#define CMD17 (0x40+17) // READ_SINGLE_BLOCK
#define CMD24 (0x40+24) // WRITE_BLOCK
#define CMD55 (0x40+55) // APP_CMD
#define CMD58 (0x40+58) // READ_OCR
//--------------------------------------------------
static void Error (void)
{
  LD_ON;
}
//-----------------------------------------------
uint8_t SPIx_WriteRead(uint8_t Byte)	
{
  uint8_t receivedbyte = 0;

  if(HAL_SPI_TransmitReceive(&hspi2,(uint8_t*) &Byte,(uint8_t*) &receivedbyte,1,0x1000)!=HAL_OK)
  {
    Error();
  }
  return receivedbyte;
}
//-----------------------------------------------
void SPI_SendByte(uint8_t bt)
{
  SPIx_WriteRead(bt);
}
//-----------------------------------------------
void SPI_Release(void)
{
  SPIx_WriteRead(0xFF);
}
//-----------------------------------------------
	void SD_PowerOn(void)
{
  Timer1 = 0;
  while(Timer1<2); // wait 20 milliseconds for the voltage to settle
}
//-----------------------------------------------
static uint8_t SD_cmd (uint8_t cmd, uint32_t arg)
{
  uint8_t n, res;
  return res;
}
//-----------------------------------------------
uint8_t sd_ini(void)
{
	uint8_t i;
  int16_t tmr;
  uint32_t temp;
  LD_OFF;
  sdinfo.type = 0;
	//temp = hspi2.Init.BaudRatePrescaler;
	//hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128; //156.25 kbbs
	//HAL_SPI_Init(&hspi2);
	SS_SD_DESELECT();
	for(i=0;i<10;i++) //80 §Ú§Þ§á§å§Ý§î§ã§à§Ó (§ß§Ö §Þ§Ö§ß§Ö§Ö 74) §¥§Ñ§ä§Ñ§ê§Ú§ä §ã§ä§â 91
	SPI_Release();
	//hspi2.Init.BaudRatePrescaler = temp;
	//HAL_SPI_Init(&hspi2);
	SS_SD_SELECT();
  return 0;
}		
//-----------------------------------------------
uint8_t SPI_ReceiveByte(void)
{
  uint8_t bt = SPIx_WriteRead(0xFF);
  return bt;
}
