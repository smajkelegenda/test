
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <linux/kernel.h>
#include <sys/sysinfo.h>
#include <libgen.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

int loop = 1;

/*
*	\033 is an escape sequence followed by the color code which changes the color of the text
*/
void red(){
	printf("\033[1;31m");
}

void yellow(){
  printf("\033[1;33m");
}

void green(){
	printf("\033[0;32m");
}

void purple(){
	printf("\033[0;35m");
}

void cyan(){
	printf("\033[0;36m");
}

void reset(){
	printf("\033[0m"); // color code "[0m" resets the text color to the default color
}

void prompt(){

	char *username;
	char hostname[HOST_NAME_MAX + 1]; // host names are limited to (HOST_NAME_MAX) bytes
	
	username = getlogin(); // included in unistd.h, it returns the login name of the user
	gethostname(hostname, HOST_NAME_MAX + 1); // returns the standard host name for the current machine

	char cwd[PATH_MAX]; // PATH_MAX - defines the longest path a directory can be

	/*
	*	settings for the color and the overall look of the prompt
	*/
	red();
	printf("[");

	yellow();
	printf("%s", username);

	green();
	printf("@");

	purple();
	printf("%s", hostname);

    if (getcwd(cwd, sizeof(cwd)) != NULL) { // getcwd() returns an absolute pathname of the current working directory
	cyan();
	printf(" %s", basename(cwd)); // basename() returns the final component of the pathname, deleting any trailing '/' characters
	}
	else{
		perror("getcwd() error");
        return;
	}

	red();
	printf("]");

	reset();

	printf("$ ");

}



void date() {
	time_t T = time(NULL);
    struct tm tm = *localtime(&T);

    printf("System Date is: %02d/%02d/%04d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    printf("System Time is: %02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

/*

void rev(char file_name[128]) {
	FILE *fp;
    char ch, newch[500];
    int i=0, j, COUNT=0;
     
    fp = fopen(file_name, "r");
    if(!fp)
    {
        printf("Error in opening the file...\nExiting...");
        getch();
        return 0;
    }
    printf("\nThe original content is:\n\n");
    ch = getc(fp);
    while(ch != EOF)
    {
        COUNT++;
        putchar(ch);
        newch[i] = ch;
        i++;
        ch = getc(fp);
    }
    printf("\n\n\n");
    printf("The content in reverse order is:\n\n");
    for(j=(COUNT-1); j>=0; j--)
    {
        ch = newch[j];
        printf("%c", ch);
    }
    printf("\n");
    getch();
}

void mv(char file_name1[128], char file_name2[128])
{
	int i,fd1,fd2;
	char buf[2];
	file1=file_name1;
	file2=file_name2;
	printf("file1=%s file2=%s",file1,file2);
	fd1=open(file1,O_RDONLY,0777);
	fd2=creat(file2,0777);
	while(i=read(fd1,buf,1)>0)
	write(fd2,buf,1);
	remove(file1);
	close(fd1);
	close(fd2);
}

void waitpid() {
	int pid;
    int status;
    pid =fork();
    if(pid<0){
        printf("Error");
    }
    else if(pid==0){
         // Child Process
         for(int i=0;i<100;i++){
            value-=1;
            printf("%d \n",value);
         }
        exit(EXIT_SUCCESS);
    }
	
   waitpid(pid,&status,0); // WAIT till child gets over
	// Parent process
   for(int i=0;i<100;i++){
            value-=1;
            printf("%d \n",value);
         }
   printingFunction();
    if (WIFSIGNALED(status)){
        printf("Error\n");
    }
    else if (WEXITSTATUS(status)){
        printf("Exited Normally\n");
    }
    return 0;
    
	}
}

*/

void fork_c(){

	int rc = fork();
	
	if(rc != 0 && rc != -1){
		printf("I am a parent waiting for my child to wake up\n");
		wait(NULL);
		printf("I am a parent and my child has woken up\n");
	}
	if(rc == 0){

		execl("./sleep", "./sleep", NULL );

		kill(getpid(), SIGINT); 
	}
}

void execvpe() {
	char* command = "ls";
    char* argument_list[] = {"ls", "-l", NULL};
 
    printf("Before calling execvp()\n");
 
    printf("Creating another process using fork()...\n");
 
    if (fork() == 0) {
        // Newly spawned child Process. This will be taken over by "ls -l"
        int status_code = execvp(command, argument_list);
 
        printf("ls -l has taken control of this child process. This won't execute unless it terminates abnormally!\n");
 
        if (status_code == -1) {
            printf("Terminated Incorrectly\n");
            return 1;
        }
    }
    else {
        // Old Parent process. The C program will come here
        printf("This line will be printed\n");
    }
 
    return 0;
}



void forkbomb(){
	/**
	 * Since the forkbomb command is very dangerous a simple CLI is created in order
	 * to protect the user from accidently crashing its computer.
	 */
	red();
	printf("<Danger!>");
	reset();
	printf("This function will crash your computer if you continue!\n");
	printf("[Y/N] : ");

	char input[1024] = "";
	fgets(input, 1024, stdin);

	if(input[0] == 'Y' || input[0] == 'y'){
		
		printf("Goodbye!\n");
		/**
		 * The forkbomb command is simple in nature, all we need is a infinite loop and the fork command
		 * We create an "infinite" number of processes, in a certain amount of time the computer will be overloaded
		 * with new child processes, it will start to freeze and in the end -> shutdown
		 * 
		 * Also notice that the forking is grows fast 1 child creates 2 processes, these 2 create 4..
		 * 
		 */
		while(1){
			fork();
		}

	}
	
	else if( input[0] == 'N' || input[0] == 'n'){
		printf("Smart choice!\n");
	}
	
	else{
		printf("Incorrect input, aborting executing!\n");
	}

}
/**
 * Function used for routing user input, it can be though as the "heart" of the shell, since it
 * reads the user input, intreprets it and calls the wanted function.
*/
void router(char input[1024]){

	char function[10] = "";
	char flags[10] = "";
	int output = 0;
	char file_name[128] = "";
	char file_name2[128] = "";
	int fun_counter = 0;
	int flag_counter = 0;
	int name_counter = 0;
	// first we need to detect the command, we know we reached the end of the command when we stumble upon a space or a new line char = enter key
	for(int i = 0; i < strlen(input); i++){
		if(input[i] != ' ' || (int)input[i] == 0 ){
			function[fun_counter] = input[i];
			fun_counter++;
		}
		else{
			break;
		}
	}

	// now we check if the user have provided the shell with flags
	// we detect '-' and take the next char from the string which is the actual flag

	for(int i = fun_counter; i < strlen(input); i++){
		if(input[i] == '-'){
			flags[flag_counter] = input[i + 1];
			flag_counter++;
		}
	}
	
	// the last thing we do is detect if the user have specified redirecting to an output
	// we detect '>' which means the user wants file output, we set the output variable to 1
	// this will serve as a flag when we check if the user wants file output in uptime

	// notice that we start from fun_counter + 2 * flag_counter + 1
	// fun_counter (the lenght of command) + 2 * flag_counter(number of flags, times 2 because of '-') 
	// + 1 since we want to start the input reading from the space

	for(int i = fun_counter + 2*flag_counter + 1; i < strlen(input); i++){
		if(input[i] == '>'){
			output = 1;
		}
		if(input[i] != '>' && (input[i] != ' ' || (int)input[i] == 0)){
			file_name[name_counter] = input[i];
			name_counter++;
		}
	}

	// removes the '\n' char that fgets adds
	function[strcspn(function, "\n")] = 0;

	if(strcmp(function, "date") == 0){
		date();
	}
    
    /*

	else if(strcmp(function, "rev") == 0){
		rev(file_name);
	}

	else if(strcmp(function, "mv") == 0){
		mv(file_name, file_name2);
	}
    
    */
    
    else if(strcmp(function, "fork") == 0){
		fork_c();
	}


	else if(strcmp(function, "clear") == 0){
		system("clear");
	}

	else if(strcmp(function, "exit") == 0){
		loop = 0;

	} 
	
	
	else if(strcmp(function,"execvpe") == 0){
		execvpe();
	}

	else if(strcmp(function,"forkbomb") == 0){
		forkbomb();
	}
	else if((int)function[0] != 0){ // tests if the first entered char is not a new line 
		printf("%s: command not found\n", function);
	}

}
int main(void){


	system("clear");
	/*
	* The input char array for storing user input
	*/
	char input[1024];

	while(loop){


	prompt();
	/*
	* Even if user outputs is larger than the array size, fgets will handle the overflow properly
	*/
	fgets(input, 1024, stdin);

	router(input);
	}

	return 0;
}Task 1.5:

/*
The purpose of fork() is to create a new process, which becomes the child process of the caller. After a new child process is created, both processes will execute the next instruction following the fork() system call.
int i = 1; 
while (i < 100) { i++; } 
printf("%d ", i); 
while (i > 0) { i--; }
printf("%d ", i);
In this code, process goes from executing 1st while loop and then terminating it. During this period 2nd while loop is ready and is waiting to start executing, when first loop is terminated then second one begins until condition is fulfilled. During this time first loop is terminated, after second loop is done it’s also terminated.

*/
