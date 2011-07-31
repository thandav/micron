#include <iostream>
#include "connection_manager.h"
#include <signal.h>
#include "queue_manager.h"
#include "thread_manager.h"
#include "message.h"


using namespace std;

void terminate(int param)
{
    cout << "SIGINT called. bye!" << endl;
    exit(0);
}

int main()
{
    signal(SIGINT, terminate);
    cout << "Program Starting..." << endl;
    QueueManager& QM = QueueManager::instance();
    string queueName("QUEUE");
    QM.createQueue(queueName);
    ThreadManager& TM = ThreadManager::instance();
    //ConnectionManager& rCM = ConnectionManager::instance();
    //rCM.run();
    char buf[1024];
    for (int i = 0; i < 999999; i++) {
        bzero(buf, 1024);
        snprintf(buf, 1024, "%d", i);
        TM.addJobToQueue(new Message(queueName.c_str(), buf));
    }
    sleep(60);
    QM.destroyQueue(queueName);
    cout << "Program exiting..." << endl;
}
