#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/wait.h"

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
        	perror("pipe");
        	return -1;	
    	}
    	pid_t pid = create_process();
	if (0 == pid){
		int num;
		int cnt;
		close(pipe_fd[0]);
		FILE * file = fopen(file_name, "rt");
		while(fscanf(file, "%d", &num) != -1){
			double res = 0;
			res = num;
			char c;
			fscanf(file, "%c", &c);
			if(c == '\n'){
				printf("%lf\n",res);
				continue;
			}
			while(fscanf(file, "%d", &num) != -1){
				if (num == 0){
					return 0;
				}
				char c;
				fscanf(file, "%c", &c);
				res = res / num;
				if(c == '\n'){
					break;
				}
			}
			write(pipe_fd[1], &res, sizeof(res));
			cnt++;
		}
		close(pipe_fd[1]);
		fclose(file);
	}else{
		wait(NULL);
		double res;
		close(pipe_fd[1]); 
		while(read(pipe_fd[0], &res , sizeof(res)) != 0){
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
        perror("fork");
        exit(-1);
    }
    return pid;
}
