#include <stdio.h>
#include <pthread.h>
#define NUM_THREAD	100

void * thread_inc(void * arg);
void * thread_des(void * arg);
long long num=0;

int main(int argc, char *argv[]) 
{
	pthread_t thread_id[NUM_THREAD];
	int i;

	printf("sizeof long long: %d \n", sizeof(long long));
	for(i=0; i<NUM_THREAD; i++)
	{
		if(i%2) // 쓰레드 ID 가 짝수일때 실행
			pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		else // 쓰레드 ID 가 홀수 일때 실행
			pthread_create(&(thread_id[i]), NULL, thread_des, NULL);	
	}	

	for(i=0; i<NUM_THREAD; i++)
		pthread_join(thread_id[i], NULL); // 총 100개 쓰레드의 동작 종료때까지 대기

	printf("result: %lld \n", num);
	return 0;
}

void * thread_inc(void * arg) 
{
	int i;
	for(i=0; i<50000000; i++)
		num+=1; // Critical Area
	return NULL;
}
void * thread_des(void * arg)
{
	int i;
	for(i=0; i<50000000; i++)
		num-=1; // Critical Area
	return NULL;
}
