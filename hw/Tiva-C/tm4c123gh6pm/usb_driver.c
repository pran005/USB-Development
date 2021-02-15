/**
  ******************************************************************************
  * @file    usb_driver.c 
  * @author  Pranjal Shrivastava
  * @version v0.1 - Hardware Driver Layer  
  * @date    15-February-2021
  * @brief   Peripheral Driver for USB IP in TM4C123GH6PM MCU 
  ******************************************************************************
 **/


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

/* TODO : Define a mechanism to separate IN & OUT 
 *        endpoints in host & device modes.
 */
void USBEnable_EpInterrupts(uint32_t EpIntMsk)
{
    USB0->TXIE = EpIntMsk; 
    USB0->RXIE = (EpIntMsk & (~0x01)); 
}

/* A read clears the interrupts
 * 
 * Retval structure- 
 *
 * 15            8              0 
 *  -----------------------------
 * | Tx EP status | Rx EP Status |
 *  -----------------------------
 */

uint32_t USBRead_EpInterrupts(void)
{
    uint32_t retval = USB0->TXIS; 
    retval = (retval << 8) | USB0->RXIS; 
    return retval; 
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
    USBRead_GeneralInterrupts(); 
    USBRead_EpInterrupts(); 
		
    /* Initialize the USB IP */ 
    USBEnable_GeneralInterrupts((1u<<0) | (1u<<1) | (1u<<2) | (1u<<3) | (1u<<5));
    USBEnable_EpInterrupts(0xFF); 

    /* TODO: Configure FIFO API*/
    USB0->TXFIFOSZ |= 0x4; 
   		
    /* Enable 'em D+/D- Terminations */ 
    USB0->POWER |= (1<<6); 
		
    /* You should know this one */
    NVIC_EnableIRQ(USB0_IRQn); 

}
