#ifndef __usb_driver_h_
#define __usb_driver_h_

#include "TM4C123.h"  
#include <stdint.h>

typedef enum usb_en_drv_ret_type_t
{
    USB_DRV_BAD_PARAM = -1,
    USB_DRV_SUCCESS

}usb_en_drv_ret_type_t;

typedef enum usb_en_EpType_t
{
    EP_TYP_IN=0,
    EP_TYP_OUT=1,
    EP_TYP_ALL=2,
}usb_en_EpType_t;

typedef enum
{
    EP0_STATE_IDLE=0,
    EP0_STATE_TX,
    EP0_STATE_RX,
    EP0_STATE_STATUS,
    EP0_STATE_STALL,

} usb_en_ep0_state_t; 

typedef enum usb_en_mode_t
{
    USB_MODE_HOST=0, 
    USB_MODE_OTG,
    USB_MODE_DEVICE,
}usb_en_mode_t;

typedef struct
{
    USB0_Type *base;                /** Base Address of the USB peripheral **/ 
    usb_en_mode_t dataRole;         /** Data Role : Host/Device **/ 
    uint8_t USBIntrMask;            /** USB Events Interrupt Mask **/
                                    /** For Device Mode:     |  For Host Mode
                                                             |
                                        Bit 0: Suspend       |   Bit 0: Reserved
                                        Bit 1: Resume        |   Bit 1: Resume      
                                        Bit 2: Bus Reset     |   Bit 2: Babble   
                                        Bit 3: SOF           |   Bit 3: SOF         
                                        Bit 4: Reserved      |   Bit 4: Connect    
                                        Bit 5: Disconnect    |   Bit 5: Disconnect  
                                        Bit [7:6]: Reserved  |   Bit [7:6]: [VbusErr : SessionReq] 
    
                                     **/ 

    uint16_t EpIntMsk;              /** Endpoint Interrupt Mask [7:0] - EP0-7 **/ 
    uint32_t devAddress;             /** USB Device Address set by the Host */
    void *usb_stack_ctx;            /** Pointer to the Middleware stack context **/

}usb_drv_context_t;

void USBEnable_GeneralInterrupts(usb_drv_context_t *context);
void USBDisable_GeneralInterrupts(uint32_t intrMask);
uint32_t USBRead_GeneralInterrupts(usb_drv_context_t *context);
uint32_t USBRead_EpInterrupts(usb_drv_context_t *context, usb_en_EpType_t EpType);
usb_en_drv_ret_type_t USBEnable_EpInterrupts(usb_drv_context_t *context, usb_en_EpType_t EpType);
void initialize_usb_driver(usb_drv_context_t *context);
void USBDevice_Disconnect(usb_drv_context_t *context);
void USBDevice_Connect(usb_drv_context_t *context);
void USB_Handler(usb_drv_context_t *context); 

void ControlEP_Handler(usb_drv_context_t *context);
void fetch_out_packet(void); 
void FillFIFO(uint8_t *dat, uint16_t lengthBytes) ;


#endif /* __usb_driver_h_ */ 


