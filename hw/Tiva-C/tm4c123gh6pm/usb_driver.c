/**
  ******************************************************************************
  * @file    usb_driver.c 
  * @author  Pranjal Shrivastava
  * @version v0.7 - USB Hardware Driver Layer  
  * @date    11-July-2021
  * @brief   Peripheral Driver for USB IP in TM4C123GH6PM MCU 
  ******************************************************************************
 **/


#include "usb_driver.h"
#include "usb.h"
#include "usb_descriptors.h"
/* TODO: Remove magic numbers */

/* TODO: Add init FIFO API. The FIFO for EP0 doesn't support
 *       dynamic sizing, instead the first 64-bytes of the 
 *       FIFO RAM are reserved for EP0 which is shared between
 *       IN & OUT transfers (Section 18.3.3.1 of Datasheet). 
 */
 

extern volatile uint16_t data;
extern volatile uint32_t c;  
extern volatile uint32_t gl_control_ep_out_buffer[64]; 
/* Device Specific */
static void initialize_usb_pins(void)
{
    /* Configure USB GPIOs (section 18.2 of the datasheet) */ 
    SYSCTL->RCGCGPIO |= (1u<<3); 
    
    /* Wait for clock to stabilize */ 
    while(!(SYSCTL->PRGPIO & (1u<<3))); 

    /* Configure GPIOs to Analog Mode */ 
    GPIOD->AFSEL &= ~ ( (1u<<4) | (1u<<5) ); 
    GPIOD->DEN   &= ~ ( (1u<<4) | (1u<<5) ); 
    GPIOD->AMSEL |=   ( (1u<<4) | (1u<<5) );
}

/* Device Specific */
static void reset_usb_peripheral(void)
{
    SYSCTL->SRCR2 |= (1u<<16);

    for (volatile uint8_t i=0; i<16; i++); 
    
    SYSCTL->SRCR2 &= ~(1u<<16); 
}

/* Device Specific */
static void usb_clock_config(void)
{
    /* Enable USB clocks */ 
    SYSCTL->RCGCUSB |= (1<<0);

    /* Enable USB PLL */
    SYSCTL->RCC2 &= ~(1u<<14); 
}

static usb_en_drv_ret_type_t usb_set_mode(usb_drv_context_t *context)
{
    usb_en_drv_ret_type_t retval = USB_DRV_BAD_PARAM;
    
    USB0_Type* usb = context->base; 

    switch(context->dataRole)
    {
        case USB_MODE_HOST: 

            usb->GPCS = 0x02;
            retval = USB_DRV_SUCCESS; 
            break; 

        case USB_MODE_OTG:
       
            usb->GPCS = 0x00;
            retval = USB_DRV_SUCCESS; 

            break;

        case USB_MODE_DEVICE: 

            usb->GPCS = 0x03;
            retval = USB_DRV_SUCCESS; 

            break;

    }

    return retval;
}

void USBDevice_Connect(usb_drv_context_t *context)
{
    /* Enable 'em D+/D- Terminations */ 
    context->base->POWER |= (1<<6); 
}

void USBDevice_Disconnect(usb_drv_context_t *context)
{
    /* Disable 'em D+/D- Terminations */ 
    context->base->POWER &= ~(1<<6); 
}

void USBEnable_GeneralInterrupts(usb_drv_context_t *context)
{
    context->base->IE |= context->USBIntrMask; 
}

void USBDisable_GeneralInterrupts(uint32_t intrMask)
{
    USB0->IE &= ~intrMask; 
}

/* A read clears the interrupts */ 
uint32_t USBRead_GeneralInterrupts(usb_drv_context_t *context)
{
    return context->base->IS; 
}

usb_en_drv_ret_type_t USBEnable_EpInterrupts(usb_drv_context_t *context, usb_en_EpType_t EpType)
{
    usb_en_drv_ret_type_t retval = USB_DRV_SUCCESS; 

    if((EpType==EP_TYP_IN) && (context->dataRole == USB_MODE_DEVICE))
    {
        context->base->TXIE |= context->EpIntMsk; 
    }

    else if((EpType==EP_TYP_OUT) && (context->dataRole == USB_MODE_DEVICE))
    {
        context->base->RXIE |= (context->EpIntMsk & (~0x01)); 
    }

    else if((EpType==EP_TYP_ALL) && (context->dataRole == USB_MODE_DEVICE))
    {
        context->base->TXIE |= context->EpIntMsk; 
        context->base->RXIE |= (context->EpIntMsk & (~0x01)); 
    }
    
    /* TODO: Add Host Mode support */ 

    else
    {
        retval = USB_DRV_BAD_PARAM;
    }
		
	return retval;
}

uint32_t USBRead_EpInterrupts(usb_drv_context_t *context, usb_en_EpType_t EpType)
{
    uint32_t retval = 0;
    
    if((EpType==EP_TYP_IN) && (context->dataRole == USB_MODE_DEVICE))
    {
        retval = context->base->TXIS; 
    }

    else if((EpType==EP_TYP_OUT) && (context->dataRole == USB_MODE_DEVICE))
    {
        retval = context->base->RXIS; 
    }

    /* TODO: Add Host mode support */ 
  
    return retval; 
}

void initialize_usb_driver(usb_drv_context_t *context)
{
    /* Configure GPIOs for USB */
    initialize_usb_pins(); 
    
    /* Reset USB peripheral */ 
    reset_usb_peripheral();

    /* Enable USB Clocks */
    usb_clock_config();

    /* Set USB mode */ 
    usb_set_mode(context); 

    /* Clear Interrupt Status Registers */ 
    USBRead_GeneralInterrupts(context); 
    USBRead_EpInterrupts(context, EP_TYP_IN); 
    USBRead_EpInterrupts(context, EP_TYP_OUT);	
    
    /* Initialize the USB IP */ 
    USBEnable_GeneralInterrupts(context) ;   // ((1u<<0) | (1u<<1) | (1u<<2) | (1u<<3) | (1u<<5));
    USBEnable_EpInterrupts(context, EP_TYP_ALL); 
  	
}

void USB_Handler(usb_drv_context_t *context)
{
    volatile uint32_t usb_intr_status = context->base->IS; 
    volatile uint32_t usb_tx_status = context->base->TXIS; 
    volatile uint32_t usb_rx_status = context->base->RXIS; 

    /* Check for EP0 interrupt */ 
    if(usb_tx_status & 0x01)
    {
        ControlEP_Handler(context);
    }
}

volatile usb_en_ep0_state_t Ep0State = EP0_STATE_STATUS;

/* TODO: Define clear states, use typedef enum for state variable */
void ControlEP_Handler(usb_drv_context_t *context)
{
    switch(Ep0State)
    {
        /* Idle State */ 
        case EP0_STATE_IDLE: 
        { 
            if(context->base->CSRL0 & 0x01)
            {
                USBCommunicate(context); 
            }
            break;
        }
				
		/* Status Stage */ 
        case EP0_STATE_STATUS: 
        { 
            if (context->devAddress & (1u<<31))
            {
                USB0->FADDR = (uint8_t)context->devAddress; 
                context->devAddress &= ~(1u<<31); 
            }

            if(context->base->CSRL0 & 0x01)
            {
                USBCommunicate(context); 
            }
            break ;
        }
				
		/* The "still transmitting" Stage (bMaxPacketSize < data length) */
		case EP0_STATE_TX:
        {
		    if(data)
			{
			    fetch_out_packet(); 
				while(USB0->CSRL0 & 0x02) ;
						
				/* TODO: Remove numeric constant */ 
				FillFIFO(&configurationDescriptors[64],data) ;
				data = 0;
				Ep0State = EP0_STATE_STATUS;
					
				while(USB0->CSRL0 & 0x02) ;
				USB0->CSRL0 = 0x0A ; 
			}	
				break;
        }

    }
}

void FillFIFO(uint8_t *dat, uint16_t lengthBytes) 
{
    for(uint16_t i=0; i<lengthBytes; i++)
    {
			 *((volatile uint8_t*)&USB0->FIFO0) = dat[i] ; 
    }
}

void fetch_out_packet() 
{
    /* Check num bytes received */
    c = USB0->COUNT0 ;
       
    for (uint32_t i=0; i<(c/sizeof(gl_control_ep_out_buffer[0])); i++)
    {
             gl_control_ep_out_buffer[i] =  USB0->FIFO0;
    }
}
