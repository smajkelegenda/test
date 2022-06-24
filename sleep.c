#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    printf("I am a child and I am going to sleep for 3 seconds\n");
	sleep(1);
	printf("1...\n");
	sleep(1);
	printf("2...\n");
	sleep(1);
	printf("3...\n");

    return 0;
}