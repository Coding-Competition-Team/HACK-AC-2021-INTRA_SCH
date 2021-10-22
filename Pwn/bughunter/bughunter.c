#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void debug(int key){
	char pass[32];
	int lock = 0xdeadbeef;

	printf("Enter password to view bug: ");

	gets(pass);

	if(key != lock){
		system("cat flag");
	}
	else{
		printf("Invalid");
	}
}

int main(int argc, char* argv[])
{
	int choice;
	printf("Welcome to AI Debugger v3.0 by Elon Tusk\n");
	printf("Debugging... \n1 bug was found!\n");

	debug(0xdeadbeef);
	return 0;
}
