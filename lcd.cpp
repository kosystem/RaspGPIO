#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>

#define PIN0 RPI_V2_GPIO_P1_13
#define PIN1 RPI_V2_GPIO_P1_07
#define POUT0 RPI_V2_GPIO_P1_11
#define POUT1 RPI_V2_GPIO_P1_15

void setReset()
{
  bcm2835_gpio_write(POUT0, HIGH);
}

void clrReset()
{
  bcm2835_gpio_write(POUT0, LOW);
}

void sendCommand(unsigned char command)
{
  bcm2835_gpio_write(POUT1, LOW);
  bcm2835_spi_transfer(command);
}

void sendData(unsigned char data)
{
  bcm2835_gpio_write(POUT1, HIGH);
  bcm2835_spi_transfer(data);
}

void initLCD()
{
  setReset();
  bcm2835_delay(100);
  clrReset();
  bcm2835_delay(100);

  sendCommand(0xCB);  
  sendData(0x39); 
  sendData(0x2C); 
  sendData(0x00); 
  sendData(0x34); 
  sendData(0x02); 

  sendCommand(0xCF);  
  sendData(0x00); 
  sendData(0XC1); 
  sendData(0X30); 

  sendCommand(0xE8);  
  sendData(0x85); 
  sendData(0x00); 
  sendData(0x78); 

  sendCommand(0xEA);  
  sendData(0x00); 
  sendData(0x00); 

  sendCommand(0xED);  
  sendData(0x64); 
  sendData(0x03); 
  sendData(0X12); 
  sendData(0X81); 

  sendCommand(0xF7);  
  sendData(0x20); 

  sendCommand(0xC0);    //Power control 
  sendData(0x23);   //VRH[5:0] 

  sendCommand(0xC1);    //Power control 
  sendData(0x10);   //SAP[2:0];BT[3:0] 

  sendCommand(0xC5);    //VCM control 
  sendData(0x3e); //ｶﾔｱﾈｶﾈｵﾚ
  sendData(0x28); 

  sendCommand(0xC7);    //VCM control2 
  sendData(0x86);  //--

  sendCommand(0x36);    // Memory Access Control 
  //0x48 0x68
  //0x28 0xE8
  sendData(0x48); //
    
  sendCommand(0x3A);    
  sendData(0x55); 

  sendCommand(0xB1);    
  sendData(0x00);  
  sendData(0x18); 

  sendCommand(0xB6);    // Display Function Control 
  sendData(0x08); 
  sendData(0x82);
  sendData(0x27);  

  sendCommand(0xF2);    // 3Gamma Function Disable 
  sendData(0x00); 

  sendCommand(0x26);    //Gamma curve selected 
  sendData(0x01); 

  sendCommand(0xE0);    //Set Gamma 
  sendData(0x0F); 
  sendData(0x31); 
  sendData(0x2B); 
  sendData(0x0C); 
  sendData(0x0E); 
  sendData(0x08); 
  sendData(0x4E); 
  sendData(0xF1); 
  sendData(0x37); 
  sendData(0x07); 
  sendData(0x10); 
  sendData(0x03); 
  sendData(0x0E); 
  sendData(0x09); 
  sendData(0x00); 

  sendCommand(0XE1);    //Set Gamma 
  sendData(0x00); 
  sendData(0x0E); 
  sendData(0x14); 
  sendData(0x03); 
  sendData(0x11); 
  sendData(0x07); 
  sendData(0x31); 
  sendData(0xC1); 
  sendData(0x48); 
  sendData(0x08); 
  sendData(0x0F); 
  sendData(0x0C); 
  sendData(0x31); 
  sendData(0x36); 
  sendData(0x0F); 

  sendCommand(0x11);    //Exit Sleep 

  bcm2835_delay(120); 

  sendCommand(0x29);    //Display on 
  sendCommand(0x2c); 
}
    
int main(int argc, char **argv)
{
  if (!bcm2835_init()) return 1;

  bcm2835_gpio_fsel(PIN0, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(PIN1, BCM2835_GPIO_FSEL_INPT);

  bcm2835_gpio_set_pud(PIN0, BCM2835_GPIO_PUD_UP);
  bcm2835_gpio_set_pud(PIN1, BCM2835_GPIO_PUD_UP);
    
  bcm2835_gpio_fsel(POUT0, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(POUT1, BCM2835_GPIO_FSEL_OUTP);

  bcm2835_spi_begin();
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
  bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
  bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

  initLCD();

  while (1)
    {
    uint8_t in0 = bcm2835_gpio_lev(PIN0);
    uint8_t in1 = bcm2835_gpio_lev(PIN1);

    if (in0 == 0)
      {
      bcm2835_gpio_write(POUT0, HIGH);
      }
    else
      {
      bcm2835_gpio_write(POUT0, LOW);
      }
    if (in1 == 0) break;
    }

  for (int i=0; i<5; i++)
    {
    bcm2835_gpio_write(POUT0, HIGH);
    bcm2835_delay(500);

    bcm2835_gpio_write(POUT0, LOW);
    bcm2835_gpio_write(POUT1, HIGH);
    bcm2835_delay(500);

    bcm2835_gpio_write(POUT0, HIGH);
    bcm2835_delay(500);

    bcm2835_gpio_write(POUT0, LOW);
    bcm2835_gpio_write(POUT1, LOW);
    bcm2835_delay(500);
    }
  bcm2835_spi_end();
  bcm2835_close();
  return 0;
}

