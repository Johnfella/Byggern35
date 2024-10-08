#include <stdlib.h>
#include <stdio.h>
#define FOSC 4915200UL
/*#include <avr/io.h>
#include <avr/interrupt.h>*/
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void SRAM_test (void);
void main( void ) {
    SRAM_test();
}

void SRAM_test (void) {
    volatile char *ext_ram = (char *) 0x1800;
    uint16_t ext_ram_size = 0x800;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;
    printf("Startind SRAM test...\n");

    uint16_t seed = rand();
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; i++){
        uint8_t some_value = rand();
        ext_ram[i] = some_value;
        uint8_t retreived_value = ext_ram[i];
        if(retreived_value != some_value){
            printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
            write_errors++;
        }
    }

    srand(seed);
    for(uint16_t i = 0; i < ext_ram_size; i++){
        uint8_t some_value = rand();
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value){
            printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
            retrieval_errors++;
        }
    }
    printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors); 
}