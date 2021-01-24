#ifndef _usb_descriptors_h
#define _usb_descriptors_h

#include <stdint.h>

static uint8_t const iProduct[]=
{
/******************************************************************************/
/* String Descriptor                                                          */
/******************************************************************************/
/* bLength */                                                             0x22U, 
/* bDescriptorType */                                                     0x03U, 
/* bString */
    (uint8_t)'C', 0U,
    (uint8_t)'D', 0U,
    (uint8_t)'C', 0U,
    (uint8_t)' ', 0U,
    (uint8_t)'D', 0U,
    (uint8_t)'e', 0U,
    (uint8_t)'v', 0U,
    (uint8_t)'i', 0U,
    (uint8_t)'c', 0U, 
    (uint8_t)'e', 0U,
    (uint8_t)' ', 0U,
    (uint8_t)'B', 0U,
    (uint8_t)'u', 0U, 
    (uint8_t)'o', 0U,
    (uint8_t)'o', 0U,
    (uint8_t)'y', 0U,
};

static uint8_t const iMan[]=
{
/******************************************************************************/
/* String Descriptor                                                          */
/******************************************************************************/
/* bLength */                                                             0x38U, 
/* bDescriptorType */                                                     0x03U, 
/* bString */ 
    (uint8_t)'P', 0U, 
    (uint8_t)'r', 0U,
    (uint8_t)'a', 0U,
    (uint8_t)'n', 0U,
    (uint8_t)' ', 0U,
    (uint8_t)'L', 0U, 
    (uint8_t)'a', 0U,
    (uint8_t)'b', 0U,
    (uint8_t)'s', 0U,
    (uint8_t)':', 0U, 
    (uint8_t)'T', 0U,
    (uint8_t)'e', 0U,
    (uint8_t)'a', 0U,
    (uint8_t)'m', 0U,
    (uint8_t)' ', 0U, 
    (uint8_t)'U', 0U,
    (uint8_t)'S', 0U, 
    (uint8_t)'B', 0U, 
    (uint8_t)' ', 0U, 
    (uint8_t)'F', 0U, 
    (uint8_t)'i', 0U,
    (uint8_t)'r', 0U, 
    (uint8_t)'m', 0U, 
    (uint8_t)'w', 0U,	
    (uint8_t)'a', 0U, 
    (uint8_t)'r', 0U, 
    (uint8_t)'e', 0U, 
};

static uint8_t const idevice[]=
{
/******************************************************************************/
/* String Descriptor                                                          */
/******************************************************************************/
/* bLength */                                                             0x26U, 
/* bDescriptorType */                                                     0x03U, 
/* bString */
    (uint8_t)'C', 0U,
    (uint8_t)'D', 0U,
    (uint8_t)'C', 0U,
    (uint8_t)' ', 0U,
    (uint8_t)'D', 0U,
    (uint8_t)'a', 0U,
    (uint8_t)'t', 0U,
    (uint8_t)'a', 0U,
    (uint8_t)' ', 0U,
    (uint8_t)'I', 0U,
    (uint8_t)'n', 0U,
    (uint8_t)'t', 0U,
    (uint8_t)'e', 0U,
    (uint8_t)'r', 0U,
    (uint8_t)'f', 0U,
    (uint8_t)'a', 0U,
    (uint8_t)'c', 0U,
    (uint8_t)'e', 0U,
};

static uint8_t const iLangID_descriptor[] = 
{
/******************************************************************************/
/* Language ID Descriptor                                                     */
/******************************************************************************/
/* bLength */                                                             0x04U, 
/* bDescriptorType */                                                     0x03U, 
/* wLANGID */                                                      0x09U, 0x04U, 
};

static uint8_t const stringDescriptors[] = 
{

/******************************************************************************/
/* String Descriptor                                                          */
/******************************************************************************/
/* bLength */                                                             0x2CU, 
/* bDescriptorType */                                                     0x03U, 
/* bString */
    (uint8_t)'P', 0U,
    (uint8_t)'r', 0U, 
    (uint8_t)'a', 0U, 
    (uint8_t)'n', 0U,
    (uint8_t)' ', 0U,
    (uint8_t)'L', 0U, 
    (uint8_t)'a', 0U,
    (uint8_t)'b', 0U,
    (uint8_t)'s', 0U,
    (uint8_t)' ', 0U,
    (uint8_t)'C', 0U,
    (uint8_t)'D', 0U,
    (uint8_t)'C', 0U,
    (uint8_t)' ', 0U,
    (uint8_t)'D', 0U,
    (uint8_t)'e', 0U,
    (uint8_t)'v', 0U,
    (uint8_t)'i', 0U,
    (uint8_t)'c', 0U,
    (uint8_t)'e', 0U,
    (uint8_t)'s', 0U,
};


/* Pointers to Strings array initialization */
static const uint8_t* stringPtr[] = 
{
    iLangID_descriptor,
    iMan,
    iProduct,
    stringDescriptors,
    idevice,
};

static uint8_t DeviceDescriptor[] = 
{
/******************************************************************************/
/*Device Descriptor                                                           */    
/******************************************************************************/
/* bLength */                                                             0x12U,  
/* bDescriptorType */                                                     0x01U,
/* bcdUSB */                                                        0x10U,0x01U,
/* bDeviceClass */                                                        0x02U,
/* bDeviceSubClass */                                                     0x00U,			
/* bDeviceProtocol */                                                     0x00U,
/* bMaxPacketSize (EP0) */                                                0x40U,
/* idVendor */                                                      0xB4U,0x04U,
/* idProduct */                                                     0x02U,0x00U,
/* bcdDevice */                                                     0x00U,0x01U,
/* iManufacturer */                                                       0x01U,
/* iProduct */                                                            0x02U,

/* iSerialNumber */                                                       0x03U,
/* bNumConfigurations */                                                  0x01U,
}; 


static uint8_t configurationDescriptors[] = 
{
/******************************************************************************/
/* Configuration Descriptor                                                   */
/******************************************************************************/
/* bLength */                                                             0x09U, 
/* bDescriptorType */                                                     0x02U, 
/* wTotalLength */                                                 0x43U, 0x00U, 
/* bNumInterfaces */                                                      0x02U, 
/* bConfigurationValue */                                                 0x01U, 
/* iConfiguration */                                                      0x00U, 
/* bmAttributes */                                                        0x80U, 
/* bMaxPower */                                                           0x19U, 

/******************************************************************************/
/* Communication Alternate Settings                                           */
/******************************************************************************/
/* bLength */                                                             0x09U, 
/* bDescriptorType */                                                     0x04U, 
/* bInterfaceNumber */                                                    0x00U, 
/* bAlternateSetting */                                                   0x00U, 
/* bNumEndpoints */                                                       0x01U, 
/* bInterfaceClass */                                                     0x02U, 
/* bInterfaceSubClass */                                                  0x02U, 
/* bInterfaceProtocol */                                                  0x01U, 
/* iInterface */                                                          0x03U, 

/******************************************************************************/
/* Header                                                                     */
/******************************************************************************/
/* bFunctionLength */                                                     0x05U, 
/* bDescriptorType */                                                     0x24U, 
/* bDescriptorSubtype */                                                  0x00U, 
/* bcdADC */                                                       0x10U, 0x01U, 

/******************************************************************************/
/* Abstract Control Management                                                */
/******************************************************************************/
/* bFunctionLength */                                                     0x04U, 
/* bDescriptorType */                                                     0x24U, 
/* bDescriptorSubtype */                                                  0x02U, 
/* bmCapabilities */                                                      0x02U, 

/******************************************************************************/
/* Union                                                                      */
/******************************************************************************/
/* bFunctionLength */                                                     0x05U, 
/* bDescriptorType */                                                     0x24U, 
/* bDescriptorSubtype */                                                  0x06U, 
/* bControlInterface */                                                   0x00U, 
/* bSubordinateInterface */                                               0x01U, 

/******************************************************************************/
/* Call Management                                                            */
/******************************************************************************/
/* bFunctionLength */                                                     0x05U, 
/* bDescriptorType */                                                     0x24U, 
/* bDescriptorSubtype */                                                  0x01U, 
/* bmCapabilities */                                                      0x00U, 
/* bDataInterface */                                                      0x01U, 

/******************************************************************************/
/* Endpoint Descriptor                                                        */
/******************************************************************************/
/* bLength */                                                             0x07U, 
/* bDescriptorType */                                                     0x05U, 
/* bEndpointAddress */                                                    0x81U, 
/* bmAttributes */                                                        0x03U, 
/* wMaxPacketSize */                                               0x08U, 0x00U, 
/* bInterval */                                                           0x0AU, 

/******************************************************************************/
/* Data Alternate Settings                                                    */
/******************************************************************************/
/* bLength */                                                             0x09U, 
/* bDescriptorType */                                                     0x04U, 
/* bInterfaceNumber */                                                    0x01U, 
/* bAlternateSetting */                                                   0x00U, 
/* bNumEndpoints */                                                       0x02U, 
/* bInterfaceClass */                                                     0x0AU, 
/* bInterfaceSubClass */                                                  0x00U, 
/* bInterfaceProtocol */                                                  0x00U, 
/* iInterface */                                                          0x04U, 

/******************************************************************************/
/* Endpoint Descriptor                                                        */
/******************************************************************************/
/* bLength */                                                             0x07U, 
/* bDescriptorType */                                                     0x05U, 
/* bEndpointAddress */                                                    0x82U, 
/* bmAttributes */                                                        0x02U, 
/* wMaxPacketSize */                                               0x40U, 0x00U, 
/* bInterval */                                                           0x00U, 

/******************************************************************************/
/* Endpoint Descriptor                                                        */
/******************************************************************************/
/* bLength */                                                             0x07U, 
/* bDescriptorType */                                                     0x05U, 
/* bEndpointAddress */                                                    0x03U, 
/* bmAttributes */                                                        0x02U, 
/* wMaxPacketSize */                                               0x40U, 0x00U, 
/* bInterval */                                                           0x00U, 
};

#endif /* _usb_descriptors_h */
