#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

char flag[50];

void init(){
	srand(time(NULL));
    	strcpy( flag, "cat flag" );
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
}

int main()
{
	init();

	int a = rand()%10;
	while(a == 0){
		a = rand()%10;
	}
	int b = rand()%100000;
	
	printf("a is %d and b is %d \n",a,b);
	printf("For x > %d, solve x * %d = %d \n",b,a,b);

	int x;
	printf("Enter the solution: ");
	scanf("%d", &x);

	int g = x*a;

	if(x > b && g >= b-20 && g <= b+20){
		printf("", system(flag));
		return 0;
	}else{
		printf("Incorrect, your x was %d and x * a was %d",x,g);
		return 0;
	}
}
