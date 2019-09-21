#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	int n = atoi(argv[1]);
	int fib = 0, minus_one=1, minus_two=1, i;

	for(i=1; i<=n; i++){
		if((i==1)||(i==2))
			fib = 1;
		else{
			fib = minus_one + minus_two;
			minus_two = minus_one;
			minus_one = fib;
		}
	}

	printf("%d\n", fib);

	return 0;
}
