ENTRY(RESET)

MEMORY
{
    APP_FLASH(rx): ORIGIN = 0x0000, LENGTH = 0x7000
    SRAM(rwx): ORIGIN = 0x0060, LENGTH = 2048
}

SECTIONS
{
    .text : 
    {
        *(.app_ivt)
        *(.text)
        *(.rodata)
        *(.trampolines)
        _etext = .;
    }> APP_FLASH

    .data : 
    {
        _sdata = .;
        *(.data)
        *(COMMON)
        _edata = .;
    }> SRAM AT> APP_FLASH

    .bss : 
    {
        _sbss = .;
        *(.bss)
        _ebss = .;
    }> SRAM
    
}