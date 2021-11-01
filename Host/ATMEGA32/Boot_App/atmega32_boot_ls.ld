/**
 * @file Linker Script File
 * @author AbdelRahman Ali AbdelRazek
 *
 */

ENTRY(RESET)

MEMORY
{
    BOOT_FLASH(rx): ORIGIN =0x7000, LENGTH =4096
    SRAM(rwx): ORIGIN =0x0060, LENGTH =2048
}

SECTIONS
{
    .text : 
    {
        *(.boot_ivt)
        *(.text)
        *(.rodata)
        *(.trampolines)
        _etext = .;
    }> BOOT_FLASH

    .data : 
    {
        _sdata = .;
        *(.data)
        *(COMMON)
        _edata = .;
    }> SRAM AT> BOOT_FLASH

    .bss : 
    {
        _sbss = .;
        *(.bss)
        _ebss = .;
    }> SRAM
    
}