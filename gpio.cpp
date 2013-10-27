#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>

#define PIN0 RPI_V2_GPIO_P1_13
#define PIN1 RPI_V2_GPIO_P1_07
#define POUT0 RPI_V2_GPIO_P1_11
#define POUT1 RPI_V2_GPIO_P1_15


int main(int argc, char **argv)
{
  if (!bcm2835_init()) return 1;

  bcm2835_gpio_fsel(PIN0, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(PIN1, BCM2835_GPIO_FSEL_INPT);

  bcm2835_gpio_set_pud(PIN0, BCM2835_GPIO_PUD_UP);
  bcm2835_gpio_set_pud(PIN1, BCM2835_GPIO_PUD_UP);
    
  bcm2835_gpio_fsel(POUT0, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(POUT1, BCM2835_GPIO_FSEL_OUTP);

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
  bcm2835_close();
  return 0;
}

