#include "usb_driver.h"

static void usb_set_mode(usb_en_mode_t usbMode)
{
    switch(usbMode)
    {
        case USB_MODE_HOST: 

            USB0->GPCS = 0x02;
            break; 

        case USB_MODE_OTG:
       
            USB0->GPCS = 0x00;
            break;

        case USB_MODE_HOST: 

            USB0->GPCS = 0x03;
            break; 
    }
}


void init_usb_hw(void)
{
    /* Configure USB GPIOs (section 18.2 of the datasheet) */ 
    SYSCTL->RCGCGPIO |= (1u<<3); 
    
    /* Wait for clock to stabilize */ 
    while(!(SYSCTL->PRGPIO & (1u<<3))); 

    /* Configure GPIOs to Analog Mode */ 
    GPIOD->AFSEL &= ~ ( (1u<<4) | (1u<<5) ); 
    GPIOD->DEN   &= ~ ( (1u<<4) | (1u<<5) ); 
    GPIOD->AMSEL |=   ( (1u<<4) | (1u<<5) ); 
		
    /* Reset USB peripheral */ 
    SYSCTL->SRCR2 |= (1u<<16);

    for (volatile uint8_t i=0; i<16; i++); 
    
    SYSCTL->SRCR2 &= ~(1u<<16); 

    /* Enable USB clocks */ 
    SYSCTL->RCGCUSB |= (1<<0);

    /* Enable USB PLL */
    SYSCTL->RCC2 &= ~(1u<<14); 

    /* Set USB mode */ 
    usb_set_mode(USB_MODE_DEVICE); 

    /* Clear Interrupt Status Registers */ 
    gl_usb_intr_stat= USB0->IS;
    gl_usb_tx_stat = USB0->TXIS;
    gl_usb_rx_stat = USB0->RXIS; 
		
    /* Initialize the USB IP */ 
    USB0->IE    |= (1u<<0) | (1u<<1) | (1u<<2) | (1u<<3) | (1u<<5);
    USB0->TXIE  |= 0xFF ;
    USB0->RXIE  |= 0xFE ;
    USB0->TXFIFOSZ |= 0x4; 
   		
    /* Enable 'em D+/D- Terminations */ 
    USB0->POWER |= (1<<6); 
		
    /* You should know this one */
    NVIC_EnableIRQ(USB0_IRQn); 

}
