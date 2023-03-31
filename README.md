# **OTA Programmer (Presonal Project for Learning Purpose)**
****
## **Project Description**
    The aim of this project to load new images wirelessly to a specific host using NodeMCU for:
            1. Load the image.
            2. Burn the image to the host.
            3. Check the image integrity.

    Using ATMEGA32 as a host.
    
    Demonstration Videos:
    1- https://youtu.be/OfSwXuUjenA
    2- https://youtu.be/HIVw4h3W39A

## **The Aims of this Project**
    1. How to write Make file.
    2. How to write Linker script.
    3. How to write Startup code.
    4. How relocatable object files are linked together.
    5. The purpose of each section in the code file.
    6. Bare metal embedded systems programming using Visual studio code and gcc tools.

## **Boot Loader Logic**
    1. First it checks if APP request available and there is Valid flashed app in flash.
    2. It begins to receive frame after frame and change its state accordingly:
        2.1- It begins at wait flash request state.
        2.2- If flash request frame received it enters to wait tx data state.
        2.3- after flashing all tx pages data it enters to wait crc check state
        (Note: the logic of this state isn't implemented yet).
        2.4- after crc check it jumps to flashed application.

### **The Frame and Its Construction**
    We communicate with the boot loader using frames of different types.

    1. First byte is the frame length.
    2. Second byte is the frame type:
        2.1- Idx 2 -> Flash Request Frame.
        2.2- Idx 3 -> Tx Pages DATA Frame.
        2.3- Idx 4 -> CRC Check Frame.
    3. Data of specific frame:
        3.1- for Flash Request Frame: the data is the numbers of pages to be flashed.
        3.2- for Tx Pages Frame: First byte is the Page number and 128 Bytes page data.
        3.3- for CRC Check Frame: 2 bytes for generated CRC of the sent Image.

# **Note**
    In Interrupt Vector Table (IVT) construction I used a naked function instead of array of pointers to functions,
    because ATMEGA32 logic is to run the first instruction in this table which is to be a jump to the required ISR
    not the address of the ISR itself.

    naked attribute indicate that the specified function does not need prologue/epilogue sequences generated by the compiler.

    

