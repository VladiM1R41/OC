#include <stdio.h>
#include <pthread.h>
#include <malloc.h>

pthread_mutex_t mutex;
typedef struct someArgs_tag {
	int data[2];
} someArgs_t;


int res[2] = {0,0};

void* add_array(void *args) {
	someArgs_t *arg = (someArgs_t*) args;
	pthread_mutex_lock(&mutex);
	for (int q = 0;q < 2;q++){
		res[q] += arg->data[q];
	}
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}


int main(){
	int *a;
	int n, m;
	int i;
	printf("Введите кол-во массивов: ");
	scanf("%d", &n);
	printf("Введите длину массивов: ");
	scanf("%d", &m);
	a = (int*)malloc(n*m*sizeof(int));
	pthread_t threads[n];
	for (i =0;i < n;i++){
		for(int j = 0; j < m; j++){
			scanf("%d", a + i*m + j);
		}
	}
	someArgs_t args[n];
	//int data[3][2] = {{6,6},{-2,-2},{6,6}};

	for (i = 0; i < n; i++) {
		for(int j = 0; j < m;j++){
			args[i].data[j] = *(a + i*m+j);
		} 
	}

	for (i = 0; i < n; i++) {
	    pthread_create(&threads[i], NULL, add_array, (void*) &args[i]);
	}

	for (i = 0; i < n; i++) {
	        pthread_join(threads[i], NULL);

	}
	for (i = 0; i < m; i++) {
	        printf("%d ", res[i]);

	}
}
