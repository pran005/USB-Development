#ifndef _usb_h
#define _usb_h

#include <stdint.h>
#include <stdbool.h>
#include "TM4C123.h" 
#include "usb_driver.h"

#define IN  0x01 
#define OUT 0x00

typedef struct
{
    
    uint8_t  bmRequestType; 
    uint8_t  bRequest; 
    uint16_t wValue; 
    uint16_t wIndex;
    uint16_t wLength; 

} __attribute__((packed)) USB_Request_t ;

typedef struct
{
    USB_Request_t request;      /** USB Request Packet **/
    uint8_t* xferBuffer;        /** Transfer Buffer **/
    uint8_t* BufferSize;        /** Transfer Buffer Size **/
    uint8_t* HostRxBuffer;      /** Buffer to store Host data **/
    uint16_t CountRemain;       /** Remaining bytes to xfer complete **/
    uint16_t xferSize;          /** Total size of transfer **/
    bool xferdirection;         /** Transfer Direction : IN/OUT **/
    bool zlp;                   /** ZLP required to complete data stage **/ 

} usb_control_xfer_t;

typedef struct
{
   usb_drv_context_t *driverContext;
   usb_control_xfer_t ControlXfer; 

} USB_DeviceStack_context_t; 

void USBSetDevStackContext(usb_drv_context_t *driverContext, void* devStackCtx);
void USBCommunicate(usb_drv_context_t *context); 

#endif /* #ifndef _usb_h */
