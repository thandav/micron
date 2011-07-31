#include <iostream>
#include "connection_manager.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "thread_manager.h"
#include "message.h"
#include "queue_manager.h"
#include <sys/select.h>

using namespace std;

ConnectionManager::ConnectionManager()
{
    pthread_mutex_init(&m_mutex, NULL);
}

ConnectionManager::~ConnectionManager()
{
    //dtor
}

ConnectionManager::ConnectionManager(const ConnectionManager& other)
{

}


void ConnectionManager::run()
{
    pthread_mutex_lock(&m_mutex);

    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;
    int i, n, maxfd, maxi, nready, listenfd, connfd, sockfd, client[FD_SETSIZE];
    fd_set rset, allset;
    char buf[MAXLINE];
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13345);

    if (bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
    {
        cerr << "bind error" << endl;
        exit(1);
    }

    if (listen(listenfd, 10) < 0)
    {
        cerr << "listen error" << endl;
        exit(1);
    }
    QueueManager& pQM = QueueManager::instance();
    ThreadManager& pTM = ThreadManager::instance();

    maxfd = listenfd;            /* initialize */
    maxi = -1;                   /* index into client[] array */
    for (i = 0; i < FD_SETSIZE;  i++)
        client[i] = -1;          /* -1 indicates available entry */

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

     for ( ; ; ) {
        rset = allset;          /* structure assignment */
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(listenfd, &rset)) {       /* new client connection */
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
            for (i = 0; i < FD_SETSIZE; i++)
                if (client[i] < 0)
                {
                    client[i] = connfd; /* save descriptor */
                    pQM.createQueue(getClientName(connfd));
                    break;
                }
            if (i == FD_SETSIZE)
            {
                cerr << "too many clients" << endl;
            }

            FD_SET(connfd, &allset);       /* add new descriptor to set */

            if (connfd > maxfd)
                maxfd = connfd; /* for select */
            if (i > maxi)
                maxi = i;          /* max index in client[] array */
            if (--nready <= 0)
                continue;          /* no more readable descriptors */
        }
        for (i = 0; i <= maxi; i++) {       /* check all clients for data */
            if ( (sockfd = client[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset)) {
                bzero(buf, MAXLINE);
                if ( (n = read(sockfd, buf, MAXLINE)) == 0) {
                    /* connection closed by client */
                    pQM.destroyQueue(getClientName(sockfd));
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else {
                    Message* msg = new Message(getClientName(sockfd).c_str(), buf);
                    pTM.addJobToQueue(msg);
                }

                if (--nready <= 0)
                    break;         /* no more readable descriptors */
            }
        }
    }
    close(listenfd);

    pthread_mutex_unlock(&m_mutex);
}

string ConnectionManager::getClientName(int sockfd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    //bzero(&addr, sizeof(struct sockaddr_in));
    int err = getpeername(sockfd,(struct sockaddr*) &addr, &len);
    if (err < 0) {
        cerr << "peername not found" << endl;
        exit(1);
    }
    char cbuf[40];
    char buff[32];
    bzero(cbuf, 40);
    bzero(buff, 32);
    const char* ip = inet_ntop(AF_INET, &addr.sin_addr, buff, 32);
    int port = ntohs(addr.sin_port);
    snprintf(cbuf, 40, "%s:%d", ip, port);
    string retVal((const char*) cbuf);
    return retVal;
}

ConnectionManager& ConnectionManager::instance()
{
    static ConnectionManager instance;
    return instance;
}

