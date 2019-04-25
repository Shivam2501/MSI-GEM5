#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int x = 0;
int max= 0;
pthread_mutex_t lock =PTHREAD_MUTEX_INITIALIZER;


void* producer()
{
	while(1)
	{
		pthread_mutex_lock(&lock);
		x++;
		if(x==max)
		{
			pthread_mutex_unlock(&lock);
			return NULL;
		}
		pthread_mutex_unlock(&lock);

	}
}

void* consumer()
{
	int y;
	while(1)
	{
		pthread_mutex_lock(&lock);
		y = x;
		if(y==max)
		{
			pthread_mutex_unlock(&lock);
			return NULL;
		}
		pthread_mutex_unlock(&lock);

	}
}

int main(int argc, char* argv[])
{
	if(argc !=2)
	{
		printf("Usage: %s maxnumber\n",argv[0]);
		return 0;
	}

	max = atoi(argv[1]);
	pthread_t threads[8];
	int i=0;

	pthread_create(&threads[0],NULL,producer,NULL);
	for(i=1;i<8;i++)
		pthread_create(&threads[i],NULL,consumer,NULL);

	pthread_join(threads[0],NULL);
	for(i=0;i<8;i++)
		pthread_join(threads[i],NULL);
	printf("max number reached: %d\n",x);

	return 0;
}
