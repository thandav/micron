#ifndef THREADMANAGER_H
#define THREADMANAGER_H
#include <pthread.h>
#include <queue>
#include <iostream>
#include "message.h"

using namespace std;

class ThreadManager
{
    public:
        static ThreadManager& instance();

        void addJobToQueue(Message* msg);
        friend void* run(void* ptr);
    protected:
    private:
        ThreadManager();
        ~ThreadManager();
        ThreadManager(const ThreadManager& other);
        ThreadManager& operator=(const ThreadManager& other);

        static ThreadManager* pInstance;

        pthread_mutex_t mutex;
        pthread_cond_t cond;
        queue<Message*> m_pJobQueue;
};

#endif // THREADMANAGER_H
