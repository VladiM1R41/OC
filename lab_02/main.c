
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct index{
	int j_index;
	int i_index;
}index;

typedef struct data_set {
	int start_index;
	int end_index;
	int* data;
}data_set;


void swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}


index quick_sort(int* s_arr, int first, int last)
{
    	int i = first, j = last, pivot = s_arr[(first + last) / 2];
 
    	do {
        	while (s_arr[i] < pivot) i++;
        	while (s_arr[j] > pivot) j--;
 
        	if(i <= j) {
            		swap(&s_arr[i], &s_arr[j]);
            		i++;
            		j--;
        	}
    	} while (i <= j);
	index lr;
	lr.j_index = j;
	lr.i_index = i;
	return lr;
}


void* sorted(void* data)
{

	data_set* info = (data_set*)data;


	int left_index, right_index;
	index q;

	left_index = info->start_index;
	right_index = info->end_index;

	if (left_index < right_index) {

		pthread_attr_t attr;
		pthread_t first_thread;
		pthread_t second_thread;

		data_set info1;
		data_set info2;

		info1.data = info->data;
		info2.data = info->data;


		pthread_attr_init(&attr);


		pthread_attr_setdetachstate(
			&attr, PTHREAD_CREATE_JOINABLE);


		q = quick_sort(info->data,left_index,right_index);
		int i = q.i_index;
		int j = q.j_index;
		info1.start_index = left_index;
		info1.end_index = j;


		if (pthread_create(&first_thread,&attr, sorted,&info1)) {
			printf("222Error in creating thread\n");


			exit(-1);
		}

		info2.start_index = i;
		info2.end_index = right_index;


		if (pthread_create(&second_thread,&attr, sorted,&info2)) {
			printf("333Error in creating thread\n");


			exit(-1);
		}

		pthread_join(first_thread, NULL);
		pthread_join(second_thread, NULL);
	}

	return NULL;
}

int main()
{

	int N;

	data_set info;

	printf("Enter number of elements in the array: \n");
	scanf("%d",&N);


	int A[N];

	printf("Enter the array: \n");
	for (int i = 0; i < N; i++) {
		scanf("%d",&A[i]);
	}


	info.data = A;
	info.start_index = 0;
	info.end_index = N - 1;


	pthread_t thread_id;


	if (pthread_create(&thread_id, NULL,sorted,&info)) {
		printf("!!Error in creating thread\n");

		exit(-1);
	}

	int r1 = pthread_join(thread_id, NULL);


	if (r1) {
		printf("123Error in joining thread\n");

		exit(-1);
	}

	printf("Sorted Array is: \n");

	for (int i = 0; i < N; i++) {
		printf("%d ",A[i]);
	}

	pthread_exit(NULL);

	return 0;
}

