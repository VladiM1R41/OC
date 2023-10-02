#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/wait.h"
#include <fcntl.h>

#define _R_            "\x1b[31m" // red color for child process output color
#define _G_            "\x1b[32m" // green color for parent process output color
#define RESET_COLOR    "\x1b[0m"  // reset color
int create_process();

int main(){
	int cnt;
	char file_name[30];
	scanf("%s", file_name);
    	int pipe_fd[2];
    	int err = pipe(pipe_fd); 
    	if (err == -1) 
    	{
        	perror("Ошибка при создании пайпа\n");
        	return -1;	
    	}
    	pid_t pid = create_process();
    	
    	int fd = open(file_name, O_CREAT | O_RDONLY);
	if (0 == pid){
		close(pipe_fd[0]);
		
		err = dup2(fd, fileno(stdin));
		
		if(err == -1){
			perror("Ошибка при перенаправлении ввода 1\n");
			exit(-1);
		}
		
		err = dup2(pipe_fd[1], fileno(stdout));
		
		if(err == -1){
			perror("Ошибка при перенаправлении ввода 2\n");
			exit(-1);
		}
		
		err = execl("./child_process", "./child_process", NULL);
		if (err == -1){
			perror("Ошибка при запуске программы из дочернего процесса\n");
			exit(-1);
		}
	}else{
		close(pipe_fd[1]); 
		err = dup2(pipe_fd[0], fileno(stdin));
		
		if(err == -1){
			perror("Ошибка при перенаправлении ввода 3\n");
			exit(-1);
		}
		
		wait(NULL);
		double res;
		while(scanf("%lf", &res) != -1){
			printf(_G_"res = %lf\n", res);
		}
		close(pipe_fd[0]);
	}
	return 0;


}

int create_process() {
    pid_t pid = fork();
    if (-1 == pid)
    {
        perror("Ошибка при создании дочернего роцесса\n");
        exit(-1);
    }
    return pid;
}
