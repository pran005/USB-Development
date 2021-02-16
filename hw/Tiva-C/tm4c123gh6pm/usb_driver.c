/**
  ******************************************************************************
  * @file    usb_driver.c 
  * @author  Pranjal Shrivastava
  * @version v0.2 - USB Hardware Driver Layer  
  * @date    15-February-2021
  * @brief   Peripheral Driver for USB IP in TM4C123GH6PM MCU 
  ******************************************************************************
 **/


#include "usb_driver.h"

/* TODO: Remove magic numbers */

/* TODO: Add param check for all functions & decide retval */ 

/* TODO: Add init FIFO API. The FIFO for EP0 doesn't support
 *       dynamic sizing, instead the first 64-bytes of the 
 *       FIFO RAM are reserved for EP0 which is shared between
 *       IN & OUT transfers. 
 */
  	
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

void USBEnable_GeneralInterrupts(uint32_t intrMask)
{
    USB0->IE |= intrMask; 
}

void USBDisable_GeneralInterrupts(uint32_t intrMask)
{
    USB0->IE &= ~intrMask; 
}

/* A read clears the interrupts */ 
uint32_t USBRead_GeneralInterrupts(void)
{
    return USB0->IS; 
}

void USBEnable_EpInterrupts(usb_en_EpType_t EpType, usb_eb_mode_t usbMode, uint32_t EpIntMsk)
{
    if((EpType==EP_TYP_IN) && (usbMode == USB_MODE_DEVICE))
    {
        USB0->TXIE = EpIntMsk; 
    }

    else if((EpType==EP_TYP_OUT) && (usbMode == USB_MODE_DEVICE))
    {
        USB0->RXIE = (EpIntMsk & (~0x01)); 
    }

    else if((EpType==EP_TYP_ALL) && (usbMode == USB_MODE_DEVICE))
    {
        USB0->TXIE = EpIntMsk; 
        USB0->RXIE = (EpIntMsk & (~0x01)); 
    }
    
    /* TODO: Add Host Mode support */ 
}

uint32_t USBRead_EpInterrupts(usb_en_EpType_t EpType, usb_en_mode_t usbMode)
{
    uint32_t retval 
    
    if((EpType==EP_TYP_IN) && (usbMode == USB_MODE_DEVICE))
    {
        retval = USB0->TXIS; 
    }

    else if((EpType==EP_TYP_OUT) && (usbMode == USB_MODE_DEVICE))
    {
        retval = USB0->RXIS; 
    }

    /* TODO: Add Host mode support */ 
  
    return retval; 
}

void initialize_usb_driver(void)
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
    USBRead_GeneralInterrupts(); 
    USBRead_EpInterrupts(EP_TYP_IN, USB_MODE_DEVICE); 
    USBRead_EpInterrupts(EP_TYP_OUT, USB_MODE_DEVICE);	
    
    /* Initialize the USB IP */ 
    USBEnable_GeneralInterrupts((1u<<0) | (1u<<1) | (1u<<2) | (1u<<3) | (1u<<5));
    USBEnable_EpInterrupts(EP_TYP_ALL, USB_MODE_DEVICE,0xFF); 
  	
    /* Enable 'em D+/D- Terminations */ 
    USB0->POWER |= (1<<6); 
		
    /* You should know this one */
    NVIC_EnableIRQ(USB0_IRQn); 

}
