#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

#define no_reader 4
#define no_writer 3
sem_t mutex,wrt;

int count = 0;
int data = 0;
pthread_mutex_t dat = PTHREAD_MUTEX_INITIALIZER;

void *reader(void *arg)
{
    while(1)
    {
        int rid = *(int *)arg;
        sem_wait(&mutex);
        count++;

        if(count == 1)
        {
            sem_wait(&wrt);
        }

        sem_post(&mutex);
        pthread_mutex_lock(&dat);
        int c = data;
        pthread_mutex_unlock(&dat);
        printf("reader id %d  data : %d\n",rid,data);
        sem_wait(&mutex);
        count--;
        if(count == 0)
        {
            sem_post(&wrt);
        }
        sem_post(&mutex);
        sleep(1);
    }
}

void *writer(void *arg)
{
    int wid = *(int *)arg;
    while (1)
    {
        sem_wait(&wrt);
        pthread_mutex_lock(&dat);
        data++;
        int c = data;
        pthread_mutex_unlock(&dat);
        printf("Writer id : %d data : %d\n",wid,c);
        sem_post(&wrt);
        sleep(1);
    }
}

int main()
{
    sem_init(&mutex,0,1);
    sem_init(&wrt,0,1);

    pthread_t write[no_writer];
    pthread_t read[no_reader];

    int r[no_reader];
    int w[no_writer];

    for(int i = 0;i<no_writer;i++)
    {
        w[i]=i+1;
        pthread_create(&write[i],NULL,writer,&w[i]);
    }
    for(int i = 0;i<no_reader;i++)
    {
        r[i]=i+1;
        pthread_create(&read[i],NULL,reader,&r[i]);
    }
    for(int i =0;i<no_writer;i++)
    {
        pthread_join(write[i],NULL);
    }
    for(int i=0;i<no_reader;i++)
    {
        pthread_join(read[i],NULL);
    }
    sem_destroy(&mutex);
    sem_destroy(&mutex);
    return 0;
    


}