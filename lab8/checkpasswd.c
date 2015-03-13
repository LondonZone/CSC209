#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Read a user id and password from standard input, 
   - create a new process to run the validate program
   - use exec (probably execlp) to load the validate program.
   - send 'validate' the user id and password on a pipe, 
   - print a message 
        "Password verified" if the user id and password matched, 
        "Invalid password", or 
        "No such user"
     depending on the return value of 'validate'.

Setting the character arrays to have a capacity of 256 when we are only
expecting to get 10 bytes in each is a cheesy way of preventing most
overflow problems.
*/

#define MAXLINE 256
#define MAXPASSWD 10

void strip(char *str, int capacity) {
    char *ptr;
    if((ptr = strchr(str, '\n')) == NULL) {
        str[capacity - 1] = '\0';
    } else {
        *ptr = '\0';
    }
}


int main(void) {
    char userid[MAXLINE];
    char password[MAXLINE];

    /* Read a user id and password from stdin */
    printf("User id:\n");
    if((fgets(userid, MAXLINE, stdin)) == NULL) {
        fprintf(stderr, "Could not read from stdin\n"); 
        exit(1);
    }
    strip(userid, MAXPASSWD);

    printf("Password:\n");
    if((fgets(password, MAXLINE, stdin)) == NULL) {
        fprintf(stderr, "Could not read from stdin\n"); 
        exit(1);
    }
    strip(password, MAXPASSWD);

    int pipeEnd[2];// array of size 2 for password and username
    pipe(pipeEnd); // init pipes 

    pid_t pid;
    pid = fork();
    if (pid == 0){  //child process
        close(pipeEnd[1]);// close read end 
        dup2(pipeEnd[0],STDIN_FILENO);
        close(pipeEnd[0]);

        execlp("./validate",NULL);
        
    }else{
    //send 'validate' the user id and password on a pipe, 
        // set stdin to stdout of pipes 
        
        int status;
        close(pipeEnd[0]);//close write end
        write(pipeEnd[1],userid,10);
        write(pipeEnd[1],password,10); //read in
        wait(&status);
        int temp = WEXITSTATUS(status);
        close(pipeEnd[1]);// close read end 

       printf("%d\n",temp);
        switch(temp){
            case 2:
                printf("Invalid password");

            case 3:
                printf("No such user");

            case 0:
                printf("Password verified");



        }

    }

    
    
   
    
    
    return 0;
}








