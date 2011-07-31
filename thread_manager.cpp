#include "thread_manager.h"
#include <sstream>
#include <string>
#include "queue_manager.h"

ThreadManager* ThreadManager::pInstance = NULL;

void* run(void* ptr)
{
    QueueManager& pQM = QueueManager::instance();
    ostringstream buf;
    //buf << "Thread started. THREAD_ID=" << pthread_self() << endl;
    cout << buf.str();
    ThreadManager* pTM = static_cast<ThreadManager*>(ptr);
    for (;;)
    {
        Message* message = NULL;
        pthread_mutex_lock(&pTM->mutex);

        if (pTM->m_pJobQueue.empty())
        {
            //cout << "Going into WAIT state. THREAD_ID=" << pthread_self() << endl;
            pthread_cond_wait(&pTM->cond, &pTM->mutex);
        }
        else
        {
            message = pTM->m_pJobQueue.front();
            pTM->m_pJobQueue.pop();
            //cout << "Message recvd from JobQueue: " << " THREAD_ID=" << pthread_self() << endl;
            pQM.putInQueue(string(message->getQueueName()), message);
        }

        pthread_mutex_unlock(&pTM->mutex);
    }
}

ThreadManager& ThreadManager::instance()
{
    if (pInstance == NULL)
    {
        pInstance = new ThreadManager();
    }
    return *pInstance;
}

ThreadManager::ThreadManager()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t newThread;
    for (int i = 0; i < 4; i++)
    {
        pthread_create(&newThread, NULL, run, this);
    }
}

ThreadManager::~ThreadManager()
{
    //dtor
}

void ThreadManager::addJobToQueue(Message* msg)
{
    pthread_mutex_lock(&mutex);
    m_pJobQueue.push(msg);

    if (m_pJobQueue.size() > 0)
    {
        //cout << "SIGNALLING! JOB_COUNT=" << m_pJobQueue.size() << endl;
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutex);
}
