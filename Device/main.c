#include "TM4C123.h"         
#include "usb_descriptors.h"
#include "usb.h"
#include "usb_driver.h"
#include <stdio.h> 
#include <stdlib.h>

void init_onBoardLed(void);
volatile static uint32_t gl_usb_intr_stat, gl_usb_tx_stat, gl_usb_rx_stat ;
volatile uint32_t ControlXfer_Buffer[64]; 
static USB_DeviceStack_context_t usb_dev_context; 

static usb_drv_context_t usb_drv_context = 
{
    .base = (USB0_Type*)USB0_BASE,
    .dataRole = USB_MODE_DEVICE,
    .USBIntrMask = ((1u<<0) | (1u<<1) | (1u<<2) | (1u<<3) | (1u<<5)),
    .EpIntMsk = 0xFF
}; 
	
static USB_DeviceStack_context_t usb_dev_context =
{
    .driverContext = &usb_drv_context,
    .ControlXfer.HostRxBuffer = ControlXfer_Buffer,
};



int main(void)
{
    init_onBoardLed(); 		
		
    initialize_usb_driver(&usb_drv_context);
   
    USBSetDevStackContext(&usb_drv_context, (void*) &usb_dev_context);


    /* Enable pull-up on D+ */ 
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

    /* Call single USB Handler function */
    /* Check if there's TX/RX Interrupt asserted on EP0 */ 
    if(gl_usb_tx_stat & 0x01) 
    {
	    /* Enumeration Handler */ 
        ControlEP_Handler(&usb_drv_context) ; 
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
