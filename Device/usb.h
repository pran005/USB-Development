#ifndef _usb_h
#define _usb_h

#include <stdint.h>
#include <stdbool.h>
#include "TM4C123.h" 

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

void USB0_Handler(void); 
void USBEnum_Handler(void);
void USBCommunicate(void); 
void fetch_out_packet(void); 
void FillFIFO(uint8_t *dat, uint16_t lengthBytes) ;

#endif /* #ifndef _usb_h */
