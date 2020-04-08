#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREAD	100

void * thread_inc(void * arg);
void * thread_des(void * arg);

long long num=0;
pthread_mutex_t mutex;

int main(int argc, char *argv[]) 
{
	pthread_t thread_id[NUM_THREAD];
	int i;
	
	pthread_mutex_init(&mutex, NULL); // 뮤텍스 사용을위한 초기화

	for(i=0; i<NUM_THREAD; i++)
	{
		if(i%2)
			pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		else
			pthread_create(&(thread_id[i]), NULL, thread_des, NULL);	
	}	

	for(i=0; i<NUM_THREAD; i++)
		pthread_join(thread_id[i], NULL); // 쓰레드 종료 대기

	printf("result: %lld \n", num);
	pthread_mutex_destroy(&mutex); // 뮤텍스 사용 종료
	return 0;
}

void * thread_inc(void * arg) 
{
	int i;
	pthread_mutex_lock(&mutex); // 임계영역 사용권한 획득
	for(i=0; i<50000000; i++)
		num+=1;
	pthread_mutex_unlock(&mutex);// 임계영역 사용권한 반환
	return NULL;
}
void * thread_des(void * arg)
{
	int i;
	for(i=0; i<50000000; i++)
	{
		pthread_mutex_lock(&mutex);// 임계영역 사용권한 획득
		num-=1;
		pthread_mutex_unlock(&mutex);// 임계영역 사용권한 반환
	}
	return NULL;
}

/*
swyoon@com:~/tcpip$ gcc mutex.c -D_REENTRANT -o mutex -lpthread
swyoon@com:~/tcpip$ ./mutex
result: 0 

*/
