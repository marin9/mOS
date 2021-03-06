#include "gpio.h"

#define RPI_PERIPHERAL_ADDR     0x20000000
#define GPIO_ADDR   (RPI_PERIPHERAL_ADDR+0x200000)

#define GPFSEL0     ((volatile unsigned int*)(GPIO_ADDR+0x00))
#define GPFSEL1     ((volatile unsigned int*)(GPIO_ADDR+0x04))
#define GPFSEL2     ((volatile unsigned int*)(GPIO_ADDR+0x08))
#define GPFSEL3     ((volatile unsigned int*)(GPIO_ADDR+0x0C))
#define GPFSEL4     ((volatile unsigned int*)(GPIO_ADDR+0x10))
#define GPFSEL5     ((volatile unsigned int*)(GPIO_ADDR+0x14))

#define GPSET0      ((volatile unsigned int*)(GPIO_ADDR+0x1C))
#define GPSET1      ((volatile unsigned int*)(GPIO_ADDR+0x20))
#define GPCLR0      ((volatile unsigned int*)(GPIO_ADDR+0x28))
#define GPCLR1      ((volatile unsigned int*)(GPIO_ADDR+0x2C))
#define GPLEV0      ((volatile unsigned int*)(GPIO_ADDR+0x34))
#define GPLEV1      ((volatile unsigned int*)(GPIO_ADDR+0x38))

#define GPPUD       ((volatile unsigned int*)(GPIO_ADDR+0x94))
#define GPPUDCLK0   ((volatile unsigned int*)(GPIO_ADDR+0x98))
#define GPPUDCLK1   ((volatile unsigned int*)(GPIO_ADDR+0x9C))



void gpio_mode(int pin, int mode){
    if(pin<10)      *GPFSEL0=*(GPFSEL0) | mode<<((pin-0)*3);
    else if(pin<20) *GPFSEL1=*(GPFSEL1) | mode<<((pin-10)*3);
    else if(pin<30) *GPFSEL2=*(GPFSEL2) | mode<<((pin-20)*3);
    else if(pin<40) *GPFSEL3=*(GPFSEL3) | mode<<((pin-30)*3);
    else if(pin<50) *GPFSEL4=*(GPFSEL4) | mode<<((pin-40)*3);
    else if(pin<60) *GPFSEL5=*(GPFSEL5) | mode<<((pin-50)*3);
}

void gpio_set(int pin, int stat){
    if(pin<32){
        if(stat)    *GPSET0=1<<pin;
        else        *GPCLR0=1<<pin;
    }else if(pin<64){
        if(stat)    *GPSET1=1<<(pin-32);
        else        *GPCLR1=1<<(pin-32);
    }
}

int gpio_get(int pin){
    if(pin<32)      return ((*GPLEV0)>>pin)&1;
    else if(pin<64) return ((*GPLEV1)>>pin)&1;
    else            return -1;
}

void gpio_pull(int pin, int pmode){
    int i;
    *GPPUD=pmode;

    for(i=0;i<200;++i);
    if(pin>=32){
        *GPPUDCLK1=1<<(pin%32);
    }else if(pin>=0){
        *GPPUDCLK0=1<<(pin%32);
    }
    for(i=0;i<200;++i);
}
