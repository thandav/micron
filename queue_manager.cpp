#include "queue_manager.h"
#include <queue>

QueueManager* QueueManager::pInstance = NULL;

QueueManager& QueueManager::instance()
{
    if (pInstance == NULL)
    {
        pInstance = new QueueManager();
    }
    return *pInstance;
}

QueueManager::QueueManager() : m_id(0)
{
    pthread_mutex_init(&mutex, NULL);
}

QueueManager::~QueueManager()
{
    map<string, void*>::iterator it = m_queues.begin();
    for (;it != m_queues.end(); it++)
    {
        destroyQueue(it->first);
    }
}

void QueueManager::createQueue(const string& addr)
{
    void* _queue = new std::queue<Message*>();
    m_queues[addr] = _queue;
    printf("Created new queue for: %s\n", addr.c_str());
}

void QueueManager::putInQueue(const string& addr, Message* msg)
{
    pthread_mutex_lock(&mutex);
    std::map<string, void*>::iterator it = m_queues.find(addr);
    if (it != m_queues.end())
    {
        std::queue<Message*>* pQ = static_cast<std::queue<Message*>*>(it->second);
        if (!pQ->empty())
        {
            Message* lastMsg = pQ->back();
            if (lastMsg->getSeqId() != (msg->getSeqId() - 1))
            {
                printf("OUT OF SEQUENCE. IN_QUEUE: %ld\tENTERING: %ld\n",
                   lastMsg->getSeqId(), msg->getSeqId());
                exit(1);
            }
        }
        pQ->push(msg);
        //msg->print();
    }
    pthread_mutex_unlock(&mutex);
}

Message* QueueManager::getFromQueue(const string& addr)
{
    pthread_mutex_lock(&mutex);
    Message* retVal = NULL;
    std::map<string, void*>::iterator it = m_queues.find(addr);
    if (it != m_queues.end())
    {
        std::queue<Message*>* pQ = static_cast<std::queue<Message*>*>(it->second);
        retVal = pQ->front();
        pQ->pop();
        retVal->print();
    }
    if (retVal == NULL)
        printf("Cannot get message from queue: %s\n", addr.c_str());
    pthread_mutex_unlock(&mutex);

    return retVal;
}

void QueueManager::destroyQueue(const string& addr)
{
    std::map<string, void*>::iterator it = m_queues.find(addr);
    if (it != m_queues.end())
    {
        delete static_cast<std::queue<Message*>*>(it->second);
        m_queues.erase(it);
        printf("Queue destroyed: %s\n", addr.c_str());
    }
}
