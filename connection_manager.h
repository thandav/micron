#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

//ConnectionManager is the class which is called from main().
//It runs in the main thread infinitely till the end of the application
using namespace std;

class ConnectionManager
{
    public:
        void run();
        void stopListening();
        static ConnectionManager& instance();
        static const int MAXLINE = 1024;
    protected:
    private:
        ConnectionManager();
        ~ConnectionManager();
        ConnectionManager(const ConnectionManager& other);
        ConnectionManager& operator=(const ConnectionManager& other);

        string getClientName(int sockfd);
        pthread_mutex_t m_mutex;
};

#endif // CONNECTIONMANAGER_H
