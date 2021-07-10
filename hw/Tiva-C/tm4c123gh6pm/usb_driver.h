#ifndef __usb_driver_h_
#define __usb_driver_h_

#include "TM4C123.h"         
#include <stdint.h>

typedef enum usb_drv_ret_type_t
{
    USB_DRV_BAD_PARAM = -1,
    USB_DRV_SUCCESS

}usb_drv_ret_type_t;

typedef enum usb_en_EpType_t
{
    EP_TYP_IN=0,
    EP_TYP_OUT=1,
    EP_TYP_ALL=2,
}usb_en_EpType_t;

typedef enum usb_en_mode_t
{
    USB_MODE_HOST=0, 
    USB_MODE_OTG,
    USB_MODE_DEVICE,
}usb_en_mode_t;

void USBEnable_GeneralInterrupts(uint32_t intrMask);
void USBDisable_GeneralInterrupts(uint32_t intrMask);
uint32_t USBRead_GeneralInterrupts(void);
uint32_t USBRead_EpInterrupts(usb_en_EpType_t EpType, usb_en_mode_t usbMode);
usb_drv_ret_type_t USBEnable_EpInterrupts(usb_en_EpType_t EpType, usb_en_mode_t usbMode, uint16_t EpIntMsk);
void initialize_usb_driver(void);
void USBDevice_Disconnect(void);
void USBDevice_Connect(void);


#endif /* __usb_driver_h_ */ 


