#include <stdio.h>
#include <stdint.h>

int main(void)
{
	uint8_t port = 0x5A;
	uint8_t result_8 = ~port >> 4;
printf("0x%02x\n", result_8); //0xFA
	result_8 = (uint8_t) ~port >> 4;
printf("0x%02x\n", result_8); //0x0A
return 0;	
}
