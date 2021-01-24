# USB-Development

### Overview

A playground to learn USB 2.0 Protocol by trying to implement a USB Stack for Microcontrollers from scratch. 

The approach would be modularized, i.e. driver-library (hardware-specific), middleware and class-specific implementations will form the stack to ease porting to different hardware. 

### Progress 
The current implementation is built over Texas Instuments' TM4C123GH6PM (Tiva C) MCU based on Arm-Cortex M4. The MCU enumerates successfully as a USB Device of the CDC Class. The device doesn't handle the SET_CONFIGURATION request as for now. 

### Learn USB 
The following are a few good resources to learn about USB: 

* [USB in a Nutshell](https://www.beyondlogic.org/usbnutshell/usb1.shtml)
* [Jan Axelson's Book](https://www.amazon.in/Usb-Complete-5th-Edn-Guides/dp/1931448280)
* [A wonderful Application Note by Cypress Semiconductor](https://www.cypress.com/documentation/application-notes/an57294-usb-101-introduction-universal-serial-bus-20)
* [The Holy Specification](https://www.usb.org/document-library/usb-20-specification)
* [The CDC Class Specification](https://www.usb.org/document-library/class-definitions-communication-devices-12)

### Contribute
If you are interested to contribute on USB for your own learning or otherwise, please drop an e-mail @ - pranshri0506@gmail.com , we'd love a healthy collaboration. 
We'd love to collaborate on USB classes or USB Device/Host implementations be it baremetal or on some OS (Windows/Linux). 

If you are willing to contribute-

1. Find the suitable contribution
2. Fork the project.
3. Clone.
4. Create a branch.
5. Make your additions/changes.
6. Commit & push your changes.
7. Create a pull request.

### Note
This project uses a few descriptors as samples to depict functioning. If you are planning to re-use the same descriptors, please change the Vendor & Product IDs. We are not responsible for any legal actions in case of any misuse. The project's main aim is learning and contributing freely & legally to USB. 
