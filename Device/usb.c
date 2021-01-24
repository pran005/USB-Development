#include "usb.h"
#include "usb_descriptors.h"

volatile static int state = 0 ;
volatile static uint32_t c = 0 ;  
volatile static uint32_t gl_control_ep_out_buffer[64]; 

 
static volatile USB_Request_t* request ;
static volatile uint16_t data ; 
static volatile uint32_t dev_address;  

/* TODO: Define clear states, use typedef enum for state variable */
void USBEnum_Handler(void)
{
    switch(state)
    {
        /* Idle State */ 
        case 0 : 
        { 
            USBCommunicate() ; 
            break ;
        }
				
        /* Status Stage */ 
        case 1: 
        { 
            if (dev_address & (1u<<31))
            {
                USB0->FADDR = (uint8_t)dev_address; 
                dev_address &= ~(1u<<31); 
            }
            USBCommunicate() ; 
            break ;
        }
				
        /* The "still transmitting" Stage (bMaxPacketSize < data length) */
        case 2:
        {
            if(data)
            {
                fetch_out_packet(); 
                while(USB0->CSRL0 & 0x02) ;

				/* TODO: Remove numeric constant */ 
                FillFIFO(&configurationDescriptors[64],data) ;
                data = 0;
                state = 1;
                
                while(USB0->CSRL0 & 0x02) ;
                USB0->CSRL0 = 0x0A ; 
            }	
            break;
        }
    }
}

/* TODO : Rename and add handling for all standard requests */
void USBCommunicate(void)
{
    bool dir = IN ; 
    bool isStd = false ; 
	uint16_t size=0; 

    /* Check if a packet was received from the Host */ 
    if(USB0->CSRL0 & 0x01)
    {
        fetch_out_packet() ; 
        request = (volatile USB_Request_t*)gl_control_ep_out_buffer ;
        
        
        /* Check the direction of the Data stage */
        dir = (request->bmRequestType & 0x80) ? IN : OUT ; 
        
        /* Check if this is a standard request */ 
        isStd = (request->bmRequestType & 0x60) ? false : true ; 
			
				

        /* Check for invalid standard request */
        switch(request->bRequest)    
        {
            /* Set address */ 
            case 0x05 :
            {                                  
                /* Clear RXRDY (ACK) and set DATAEND bit since no Data Stage for SET_ADDR */ 
                USB0->CSRL0 = 0x41; 
                dev_address = request->wValue ; 			
				/* Pend changing the device address & write it after status stage is completed */ 
    	        dev_address |= (1u<<31); 	/* Using the same variable as address is 7-bit */ 

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
                    state = 1; 
                }

                else if((request->wValue>>8) == 0x02)
                {
                    while(USB0->CSRL0 & 0x02) ;

                    if(request->wLength>64)		
                    {
                        size = 64; 
                        FillFIFO(configurationDescriptors,size) ;
                        data = sizeof(configurationDescriptors) - 64; 
                        state = 2; 
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
                    state = 1; 
                }            

                break ; 
                
            }       
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
