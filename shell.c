#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>    
#include <pwd.h>
#include <ctype.h>             
#include <fcntl.h>
#include <dirent.h>


#define MAX_LEN_LINE    10
#define LEN_HOSTNAME	30      

int ls(void){
        char *cwd = (char *)malloc(sizeof(char)*1024);
        DIR *dir = NULL;
        struct dirent *list = NULL;
        getcwd(cwd, 1024);

        if((dir = opendir(cwd)) == NULL){
           printf("error\n");
           exit(1);
        }
        while((list = readdir(dir)) != NULL){
           printf("%s, ", list->d_name);
        }
        free(cwd);
        closedir(dir);
        return 0;
        }

      
int main(void)
{
    
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    pid_t pid, cpid;
    char buffer[20]; 
    



    


    while (true) {
        char *s;
        int len;
        char *ptr;
        char *commandArr[MAX_LEN_LINE] = {NULL};
        int i = 0;

        char hostname[LEN_HOSTNAME + 1];
        memset(hostname, 0x00, sizeof(hostname));

        gethostname(hostname, LEN_HOSTNAME);
        printf("%s@%s $",getpwuid(getuid())->pw_name,hostname);
	/*username@hostname$*/

        printf("MyShell $ ");


        s = fgets(command, MAX_LEN_LINE, stdin);
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }
        
 
        else if(strcmp(command,"exit\n") == 0 ){
                    printf("exit\n");
                    exit(0);
        }
        /*exit*/ 



 	else if(strcmp(command,"printdir\n") == 0 ){
	    char cwd[1024]; 
            getcwd(cwd, sizeof(cwd)); 
            printf("\nDir: %s", cwd); 
	}
	/*print current directory*/ 
        
        /*else if(strcmp(command,"mkdir\n") == 0) {mkdir(args[1], 0775);}
        make directory*/ 

        else if(strcmp(command,"ls\n") == 0 ){
            ls();
        }
        /*print file or folder in current directory*/

        ptr = strtok(s,";");
        while(ptr != NULL){
        commandArr[i] = ptr;
        i++;
        ptr = strtok(NULL," ;");
        }

      

        for(int j = 0; j < i; j++)  {
           

           s = commandArr[j];
           strcpy(command,s);
 
	   
           /* for (int i = 0; buffer[i]; i++){
             if (isspace(buffer[i]))
               strcpy(commandArr[i], &buffer[i + 1]);
	   } */ 
           /*remove null space*/

           len = strlen(command);
           printf("%d\n", len);
           if (command[len - 1] == '\n') {
              command[len - 1] = '\0'; 
           }
           printf("[%s]\n", command);
      
        
           pid = fork();
           if (pid < 0) {
              fprintf(stderr, "fork failed\n");
              exit(1);
           }

           if (pid != 0) {  /* parent */
               cpid = waitpid(pid, &status, 0);
               if (cpid != pid) {
                   fprintf(stderr, "waitpid failed\n");        
               }
               printf("Child process terminated\n");
               if (WIFEXITED(status)) {
                   printf("Exit status is %d\n", WEXITSTATUS(status)); 
               }
           }
           else {  /* child */
               ret = execve(args[0], args, NULL);
               if (ret < 0) {
                   fprintf(stderr, "execve failed\n");   
                   return 1;
                }
           } 
       
        }

    }
    return 0;
}
