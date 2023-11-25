#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t mutex;
 
int number_threads;

typedef struct quicksort_p {
	int * arr;
	int low;
	int high;
}quicksort_parameters;

void swap(int * a, int * b) { 
	int t = * a;
	* a = * b;
	* b = t;
}

int partition(int * arr, int low, int high) {
	int pivot = arr[high];
	int s = low;
	for (int i = low; i < high; i++) {
        	if (arr[i] <= pivot) {
      			swap(&arr[i], &arr[s]);
      			s++;
    		}
  	}
  	swap(&arr[s], &arr[high]);
  	return s;
}

void quicksort(int * arr, int low, int high) { 
	if (low < high) {
    		int pivot = partition(arr, low, high);
    		quicksort(arr, low, pivot - 1);
    		quicksort(arr, pivot + 1, high);
  	}
}


void quicksort_parallel(void * initialValues) {
	quicksort_parameters * parameters = initialValues;
	int * arr = parameters -> arr;
	int low = parameters -> low;
	int high = parameters -> high;
	if (low >= high) {
    		return;
  	}

  	int pivot = partition(arr, low, high);

  	pthread_mutex_lock(&mutex);
  	int a = number_threads > 0; 
  	if (a) {
    		number_threads--;
  	}
  	pthread_mutex_unlock(&mutex);

  	if (!a) { 
    		quicksort(arr, low, pivot - 1);
    		quicksort(arr, pivot + 1, high);
    		return;
  	}
  	quicksort_parameters thread_param = {
    		arr,
    		low,
    		pivot - 1
  	};
  	pthread_t thread;
  	pthread_create(&thread, NULL, quicksort_parallel, & thread_param);
  	quicksort_parameters param = {
    		arr,
    		pivot + 1,
    		high
  	};
  	quicksort_parallel(&param);
  	pthread_join(thread, NULL);
}

int main(int argc, char ** argv) {
	if (argc != 2) {
    		perror("Не указано кол-во потоков \n");
    		return 1;
  	}

  	number_threads = strtol(argv[1], NULL, 10);

  	int size; 
  	scanf("%d", &size);

  	int * elements = malloc(size * sizeof(int)); 

  	for (int i = 0; i < size; i++) {
    		//scanf("%d", & elements[i]);
    		elements[i] = rand() % 1000;
  	}
  	quicksort_parameters start_param = {
    		elements,
    		0,
    		size - 1
  	};
	clock_t start, end;
	double cpu_time_used;
	start = clock();
  	quicksort_parallel(&start_param);
  	//for (int i = 0; i < size; i++) {
   	//	printf("%d ", elements[i]);
  	//}
  	end = clock();
  	cpu_time_used = ((double)(end - start));
  	printf("time: %lf\n",cpu_time_used);
  	free(elements);
}
