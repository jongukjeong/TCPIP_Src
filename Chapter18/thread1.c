#include <stdio.h>
#include <pthread.h>
void* thread_main(void *arg);

int main(int argc, char *argv[]) 
{
	pthread_t t_id;
	int thread_param=5; // 쓰레드 동작함수로 전달할 인자값 결정
	//쓰레드의 생성 및 thread_main 함수로 쓰레드의 동작함수 결정
	if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param)!=0)
	{
		puts("pthread_create() error");
		return -1;
	}; 	
	sleep(3);  puts("end of main");
	return 0;
}

void* thread_main(void *arg) 
{
	int i;
	int cnt=*((int*)arg); // 전달받은 인자값을 (int로) 형변환
	for(i=0; i<cnt; i++)
	{
		sleep(1);  puts("running thread");	 
	}
	return NULL;
}
