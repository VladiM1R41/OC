#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/wait.h"


int main(){
	double res;
	int x;
	char c;
	int i = 0;
	while(scanf("%d%c", &x, &c) != -1){
		if(i == 0){
			res = x;
			i = i + 1;
		}else{
			if (x == 0){
				perror("Деление на 0");
				exit(-1);
			}else{
				res /= x;
				if (c == '\n'){
					printf("%lf\n", res);
					i = 0;
				}
			}
		}
	
	}
}

