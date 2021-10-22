#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[])
{
	char floor[50];

	int lvl_1 = 0xdeadbeef;
	int lvl_2 = 0xcafebabe;
	int lvl_3 = 0xff;

	printf("Which floor you wanna climb to?: ");
	gets(floor);

	if(lvl_1 == lvl_2 == lvl_3){
		printf("Welcome to the flag floor");
		system("cat flag");
	}
	else{
		printf("Weird, the tower has no such floor");
	}

	return 0;
}	
