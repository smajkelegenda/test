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
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ustat.h>
#include <time.h>
#include <utime.h>
#include <grp.h>
#include <sys/utsname.h>
#include <limits.h>


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

void mylink(char *argv[],int argc)
{	if(strcmp(argv[1],"-s")==0)
         		{  	if(!symlink(argv[2],argv[3]))
                 			printf("soft link created\n");
             			else
                			printf("error creatng symlink\n");

          		}
          		else if(argc==3)
			{	 if(!link(argv[1],argv[2]))
                    			printf("hard link created\n");
                    		else
                   			printf("error creating hard link\n");
			}
          		else
				printf("syntax error\n");

}

void mv(char *argv[],int argc)
{
	int fd,fd1,r;
	char buf1[50],buf2[50];
	switch(argc)
	{
		case 1:
		printf("Error:No argv!");
		break;
		case 2:
		printf("Error:Too few argv for move command!");
		break;
		case 3:
		mylink(argv,argc);
		unlink(argv[1]);
		printf("Contents of file %s moved to file %s\n",argv[1],argv[2]);
		break;
		default:
		printf("Error:Excess argv supplied!**!");
		break;
	}

}

void mkdir_c(char flags[10], int f_size, char name[128], int n_size){

	char dir[PATH_MAX]; // PATH_MAX - defines the longest path a direcotry can be
	getcwd(dir, sizeof(dir)); // getcwd - gives us the absolute pathname of the current working directory

	char slash[129] = "/"; // creates a new string slighty bigger than name
	strcat(slash,name); // in order to concatenate res -> /name_of_our_new_dir
	slash[strcspn(slash, "\n")] = 0;

	if(name[0] != '\n'){ // prevents creating a '' named directory

		mkdir(strcat(dir,slash), 0777); // make the dir with mkdir function
		// 0777 -> -rwxrwxrwx read,write & execute for owner, group and others

	}

	/**
	 * The user can choose to input many flags, so we go throught the flags array
	 * detecting the user flags and executing them.
	 */

	for( int i = 0;i < f_size;i++){
		if(flags[i] == 'v'){
			name[strlen(name)-1] = '\'';
			printf("mkdir: created directory '%s\n", name);
		}
		if(flags[i] == 'h'){
			printf("Create the DIRECTORY, if they do not already exist\n");
			printf("Usage mkdir [OPTION] ... DIRECTORY...\n");
			printf("-v print a message for a created directory\n");
			printf("-V print the current version\n");
			printf("-h show the help page\n");
		}
		if(flags[i] == 'V'){
			printf("current version mkdir : v0.0.1\n");
		}
	}
}

void ls(char flags[10], int f_size){

	// opendir() is included in dirent.h and opens a directory stream specified in the brackets
	// in this case the "." which represents the current directory
	DIR* dir = opendir(".");

	// checking if the directory exists
	if(dir == NULL){
		return;
	}
	
	struct dirent* entity;
	entity = readdir(dir); // reading the contents of the directory
	
	while(entity != NULL){
		if(f_size == 0){ // if no flags have been entered
		printf("%s\n", entity->d_name); // printing the names (d_name) of files and directories in the current directory 
		entity = readdir(dir); // reading the next file/directory 
		}

		/*
		* -c (color) flag
		* checks if the entity type is directory or file and changes the output color accordingly
		* directories are purple, files are green
		*/
		for(int i = 0; i < f_size; i++){
			if(flags[i] == 'c'){
				// checking if entity type (d_type) is a directory (DT_DIR)
				if(entity->d_type == DT_DIR){ 
					purple(); // changes color to purple
					printf("%s\n", entity->d_name); // prints the entity
					reset(); // resets text to default color
				}

				else if(entity->d_type == DT_REG){ // checking if entity type (d_type) is a file (DT_REG)
					green();
					printf("%s\n", entity->d_name);
					reset();	
				}
			}
		entity = readdir(dir); // reading the contents of the directory
		}
	}
	closedir(dir); // closes the directory stream

	// more flags
	for(int i = 0; i < f_size; i++){
		if(flags[i] == 'h'){
			printf("List FILEs in current directory\n");
			printf("Usage ls [OPTION]...\n");
			printf("-c add color to the output (directories - purple, files - green\n");
			printf("-V print the current version\n");
			printf("-h show the help page\n");
		}
		if(flags[i] == 'V'){
			printf("current version ls : v0.0.1\n");
		}
	}
}

void uptime(char flags[10], int f_size, char file_name[128], int output){
	
	struct sysinfo s_info; // sysinfo() returns information on overall system statistics
    int error = sysinfo(&s_info);

	// on success sysinfo() returns 0 
	// on error, -1 is returned
	if(error != 0)
    {
        printf("code error = %d\n", error);
    }

	/* s_info.uptime is system uptime in seconds
	* then some formulas are used to convert seconds into hours, minutes and seconds
	*/
    int secs = s_info.uptime;
    int hours = (int)(secs/3600);
    int minutes = ((int)secs/60) % 60;
	int seconds = (int)(secs%60);

	
	if(output == 0 && f_size == 0){ // if redirecting to file and flags are not used
		printf("uptime: %d:%d:%d\n", hours, minutes, seconds);
	}

	if(f_size == 0){
		if(output == 1){ // if we detected a '>' char in input we need to redirect the output
			// first we create a pointer to FILE type
			FILE * file;
			// we need to remove the end line char from the file_name string
			file_name[strcspn(file_name, "\n")] = 0;
			// we open/create a new file and set the mode to write - 'w'
			file = fopen(file_name,"w");

			if(file == NULL){ // if the file was not created successfully we exit the function
				return;
			}
			// we output the following string to file
			fprintf(file,"uptime: %d:%d:%d\n", hours, minutes, seconds);
			// we close the file
			fclose(file);
			// provide user with confirmation message
			printf("Uptime written to a file : %s\n", file_name);
		}

	}

	// flags
	for(int i = 0; i < f_size; i++){

		// -p (pretty) flag changes the color of the uptime command and makes the output easily readable
		if(flags[i] == 'p'){
			red();
			printf("uptime: ");
			yellow();
			printf("%d", hours);
			printf(" hours, ");
			green();
			printf("%d", minutes);
			printf(" minutes, ");
			purple();
			printf("%d", seconds);
			printf(" seconds\n");
		}

		if(flags[i] == 'V'){
			printf("current version uptime : v0.0.1\n");
		}
		if(flags[i] == 'h'){
			printf("Tell how long the system has been running\n");
			printf("Usage uptime [OPTION]...\n");
			printf("-p show uptime in pretty format\n");
			printf("-V print the current version\n");
			printf("-h show the help page\n");
		}
	}
}

void printline(char *line, int number){
        for(int i = 0; i < number; i++){
                printf(" "); // prints an empty space based on the value of number (space)
        }
        printf("%s\n", line); // prints a "line" of the car
}

// int spaces determines how far the car moves
void sl(int spaces){ 
	green();

	// car is seperated in four lines
	char *line1 = "       .--------.";
	char *line2 = " ____/_____|___ \\___";
	char *line3 = " O    _   - |   _   ,*";
	char *line4 = " '--(_)-------(_)--'";

	// prints all four segments of the car using printline() 
	printline(line1, spaces);
	printline(line2, spaces);
	printline(line3, spaces);
	printline(line4, spaces);
	reset();
}

void fork_c(){

	int rc = fork();
	
	if(rc != 0 && rc != -1){
		printf("I am a parent waiting for my child to wake up\n");
		wait(NULL);
		printf("I am a parent and my child has woken up\n");
	}
	if(rc == 0){
		/*
		* The first argument is the location
		* The second argument is the actual arguments which go to the main function of sleep.c
		*/
		/*
		 * Usually the exec family of functions, overwrites the currently executing program, and does not return
		 * to the caller program. But because we are executing the execl function on top of a child process
		 * the child gets overwriten and we return to our parent (init) process and continue the execution of our shell
		 */

		execl("./sleep", "./sleep", NULL );

		kill(getpid(), SIGINT); 
		/*
		* We have to make the child process kill itself, because if we don't, the child process will never end
		* And the parent will wait forever. 
		* if we dont kill the child process it will finish this function, finish in the router function
		* and at last it will finish in the while(1) in the main function
		* effectively having two processes, where one does not do anything and eats up memory.
		*/
	}
}

void vfork_c(){

	int shared_variable = 100;
	
	/***
	 * In vfork the child process halts the parent process until the child is done executing
	 * In vfork the child process has the same memory scope as the parent process
	 * The changes done in the child process will reflect the parent's memory
	 * Here we have declared the variable shared_variable = 100
	 * The child process increments it by one, then the child process is killed same reason as for fork_c()
	 * The changes are reflected in the parent process memory space.
	 */
	int pid_id = vfork();

	if(pid_id == 0){

		printf("I am a child and I share parent's variables\n");
		printf("Our variable before modifying in child process : %d\n", shared_variable);
		shared_variable++;
		printf("Our variable after modifying in child process : %d\n", shared_variable);

		kill(getpid(), SIGINT);

	}else{
		printf("I am a parent and the variable is now : %d\n", shared_variable);
	}
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

	// simple if statements which calls the function to its desired input
	
	if(strcmp(function, "mkdir") ==  0){
		mkdir_c(flags, flag_counter, file_name, name_counter);
	}

	else if(strcmp(function, "ls") == 0){
		ls(flags, flag_counter);
	}

	else if(strcmp(function, "uptime") == 0){
		uptime(flags, flag_counter, file_name, output);
	}

	else if(strcmp(function, "sl") == 0){
		for (int i = 0; i < 110; i++){ 
		system("sleep 0.01"); // movement speed of the car
		system("clear"); // clears the screen before each instance of car appears
		sl(i); // the car moves 110 spaces
		}
		system("clear");
	}

	else if(strcmp(function, "clear") == 0){
		system("clear");
	}

	else if(strcmp(function, "exit") == 0){
		loop = 0;

	}
    else if(strcmp(argv[0],"mv")==0){
        mv(argv,argc);
    }
	
	else if(strcmp(function,"fork") == 0){
		fork_c();
	}

	else if(strcmp(function,"vfork") == 0){
		vfork_c();
	}

	else if(strcmp(function,"forkbomb") == 0){
		forkbomb();
	}
	else if((int)function[0] != 0){ // tests if the first entered char is not a new line 
		printf("%s: command not found\n", function);
	}

}
int main(void){

	printf("%c]0;%s%c", '\033', "Medo", '\007');
	/*
	* The system("clear") is included in stdlib.h and works only in Linux based systems
	*/

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
}