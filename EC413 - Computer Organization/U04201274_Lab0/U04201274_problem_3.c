#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	unsigned int decimal = atoi(argv[1]);
	char hex[8] = "";
	int i=8, hex_char;

	do{
		i--;
		hex_char = decimal%16;
		if(hex_char>9)
			hex[i] = (hex_char-10) + 'A';
		else
			hex[i] = hex_char + '0';
		decimal /= 16;
	}while(decimal!=0);

	for(i; i<8; ++i)
		printf("%c", hex[i]);
	printf("\n");

	return 0;
}
