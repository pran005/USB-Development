#include "usb.h"
#include "usb_descriptors.h"

extern volatile usb_en_ep0_state_t Ep0State;
volatile uint32_t c = 0;  
volatile uint32_t gl_control_ep_out_buffer[64]; 

 
static volatile USB_Request_t* request;
volatile uint16_t data; 

void USBSetDevStackContext(usb_drv_context_t *driverContext, void* devStackCtx)
{
    driverContext->usb_stack_ctx = devStackCtx; 
}

/* TODO : Rename and add handling for all standard requests */
void USBCommunicate(usb_drv_context_t *context)
{
    bool dir = IN ; 
    bool isStd = false ; 
	  uint16_t size=0; 

    /* 
     * Check if a packet was received from the Host. The IP 
     * on Tiva-C doesn't give info about OUT or IN packet.
     * Hence the following piece of code figures that. 
     */ 

     /* TODO: Read EP0 rename */ 
     fetch_out_packet() ; 
     request = (volatile USB_Request_t*)gl_control_ep_out_buffer ;
        
        
     /* Check the direction of the Data stage */
     dir = (request->bmRequestType & 0x80) ? IN : OUT ; 
    
    /* Check if this is a standard request */ 
    isStd = (request->bmRequestType & 0x60) ? false : true ; 
		
			
    /* Default to invalid requests */
    /* Check for invalid standard request */
    switch(request->bRequest)    
    {
        /* Set address */ 
        case 0x05 : 
            {
                                 
                /* Clear RXRDY (ACK) and set DATAEND bit since no Data Stage for SET_ADDR */ 
                USB0->CSRL0 = 0x48; 

                context->devAddress = request->wValue ; 
									
								  /* Pend changing the device address & write it after status stage is completed */ 
                context->devAddress |= (1u<<31); 	/* Using the same variable as address is 7-bit */ 

                break ; 
            }
							
		/* GET_DESCRIPTOR */ 
        case 0x06 : 
            {
                USB0->CSRL0 = 0x40 ; 
                /* TODO Figure out request and handle accordingly */ 
								
                /* Figure out descriptor type (wValue 0xFF00) >> 8 */ 
                
                if((request->wValue>>8) == 0x1) 
                {
                    while(USB0->CSRL0 & 0x02) ;
											
					size = (request->wLength > DeviceDescriptor[0]) ? request->wLength : DeviceDescriptor[0]; 
													
					FillFIFO(DeviceDescriptor,DeviceDescriptor[0]) ; 
                    
                    while(USB0->CSRL0 & 0x02) ;
                    
                    USB0->CSRL0 = 0x0A ; 
                    
                    Ep0State = EP0_STATE_STATUS; 
                }

                else if((request->wValue>>8) == 0x02)
                {
                    while(USB0->CSRL0 & 0x02) ;
             										
					if(request->wLength>64)		
		    		{
                        size = 64; 
                        FillFIFO(configurationDescriptors,size) ;
                        data = sizeof(configurationDescriptors) - 64; 
                        Ep0State = EP0_STATE_TX; 
                        USB0->CSRL0 = 0x02 ;
                    }
							
                    else	
                    {
                        FillFIFO(configurationDescriptors,request->wLength) ;
                        USB0->CSRL0 = 0x0A ;
                    }
					
                    
                    while(USB0->CSRL0 & 0x02) ; 
                }
                
              
                else if(((request->wValue>>8 == 0x03)))
                    
                {
                    while(USB0->CSRL0 & 0x02) ;
											
											uint8_t strindex = (request->wValue & 0xFF);
										
                    FillFIFO(stringPtr[strindex],stringPtr[strindex][0]) ; 
                    
                    while(USB0->CSRL0 & 0x02) ;
                    
                    USB0->CSRL0 = 0x0A ; 
                    
                    Ep0State = EP0_STATE_STATUS;
                }
              

                break ; 
            }
    }
}



