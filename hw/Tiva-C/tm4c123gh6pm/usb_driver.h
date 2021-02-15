#ifndef __usb_driver_h_
#define __usb_driver_h_

#include "TM4C123.h"         

typedef enum usb_en_mode_t
{
    USB_MODE_HOST=0, 
    USB_MODE_OTG,
    USB_MODE_DEVICE,
}usb_en_mode_t

#endif /* __usb_driver_h_ */ 

