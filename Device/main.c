#include "TM4C123.h"         
#include "usb_descriptors.h"
#include "usb.h"
#include "usb_driver.h"
#include <stdio.h> 
#include <stdlib.h>

void init_onBoardLed(void);
volatile static uint32_t gl_usb_intr_stat, gl_usb_tx_stat, gl_usb_rx_stat ;
static usb_drv_context_t usb_drv_context = 
{
    .base = (USB0_Type*)USB0_BASE,
    .dataRole = USB_MODE_DEVICE,
    .USBIntrMask = ((1u<<0) | (1u<<1) | (1u<<2) | (1u<<3) | (1u<<5)),
    .EpIntMsk = 0xFF
}; 
	
int main(void)
{
    init_onBoardLed(); 
	
    /* Configure USB GPIOs */ 
    SYSCTL->RCGCGPIO |= (1u<<3) ; 
    
    while(!(SYSCTL->PRGPIO & (1u<<3))) ; 

    GPIOD->AFSEL &= ~ ( (1u<<4) | (1u<<5) ) ; 
    GPIOD->DEN   &= ~ ( (1u<<4) | (1u<<5) ) ; 
    GPIOD->AMSEL |=   ( (1u<<4) | (1u<<5) ) ; 
		
    /* Reset USB SRCR2 */ 
    SYSCTL->SRCR2 |= (1u<<16) ;

    for (volatile uint8_t i=0; i<16; i++) ; 

    SYSCTL->SRCR2 &= ~(1u<<16) ; 

    /* Enable USB in SYSCTL */ 
    SYSCTL->RCGCUSB |= (1<<0) ;

    /* Enable USB PLL  */
    SYSCTL->RCC2 &= ~(1u<<14); 

    /* Setup USB PHY */ 
    USB0->GPCS |= 0x03; 
    
    /* Clear Interrupt Status Registers */ 
    gl_usb_intr_stat= USB0->IS ;
    gl_usb_tx_stat = USB0->TXIS  ;
    gl_usb_rx_stat = USB0->RXIS  ; 
		
    /* Initialize the USB IP */ 
    USB0->IE    |= (1u<<0) | (1u<<1) | (1u<<2) | (1u<<3) | (1u<<5) ;
    USB0->TXIE  |= 0xFF ;
    USB0->RXIE  |= 0xFE ;
    USB0->TXFIFOSZ |= 0x4; 
   		
    /* Enable 'em D+/D- Terminations */ 
    USBDevice_Connect(&usb_drv_context);
		
    /* You should know this one */
    NVIC_EnableIRQ(USB0_IRQn); 

    while(1); 

}

/* USB Interrupt Handler */
void USB0_Handler(void)
{
    gl_usb_intr_stat = USB0->IS;
    gl_usb_tx_stat = USB0->TXIS;
    gl_usb_rx_stat = USB0->RXIS;

    /* Check if there's TX/RX Interrupt asserted on EP0 */ 
    if(gl_usb_tx_stat & 0x01) 
    {
	    /* Enumeration Handler */ 
        ControlEP_Handler() ; 
    }		
}

void init_onBoardLed(void)
{
    /* Enable GPIOF Clocks */ 
    SYSCTL -> RCGCGPIO |= (1<<5) ;
	
	/* Wait for the clocks to stabilize */
	while(! ((SYSCTL->PRGPIO) & (1<<5)) ) ;

	/* GPIO Digital Enable */
	GPIOF->DEN = (1<<1) | (1<<2) | (1<<3) ;

	/* Set direction -> output */
	GPIOF->DIR = (1<<1) | (1<<2) | (1<<3) ;

}

