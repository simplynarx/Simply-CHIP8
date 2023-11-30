#include "rom.h"
#include "cpu.h"

int main(int argc, char *argv[]){
	//open_rom(argv[1]);
	uint16_t a;
	scanf("%hx", &a);
	decode_execute_opcode(a);

	return 0;
}
