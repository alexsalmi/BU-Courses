#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	unsigned int decimal = atoi(argv[1]);
	char binary[32];
	int i;

	for(i=31; i>=0; i--){
		binary[i] = decimal%2 + '0';
		decimal /= 2;
	}

	printf("%.32s\n", binary);

	return 0;
}