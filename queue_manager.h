#ifndef QUEUEMANAGER_H
#define QUEUEMANAGER_H
#include <iostream>
#include <map>
#include "message.h"
#include <pthread.h>

using namespace std;

class QueueManager
{
    public:
        static QueueManager& instance();
        void putInQueue(const string& addr, Message* msg);
        Message* getFromQueue(const string& addr);
        void createQueue(const string& addr);
        void destroyQueue(const string& addr);
    protected:
    private:
        QueueManager();
        ~QueueManager();
        QueueManager(const QueueManager& other);
        QueueManager& operator=(const QueueManager& other);

        static QueueManager* pInstance;
        map<string, void*> m_queues;
        int m_id;
        pthread_mutex_t mutex;
};

#endif // QUEUEMANAGER_H
