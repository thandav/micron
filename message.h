#ifndef MESSAGE_H
#define MESSAGE_H
#include <sys/socket.h>
#include <arpa/inet.h>

class Message
{
    public:
        Message(const char* addr, const char* message);
        ~Message();
        Message(const Message& other);
        Message& operator=(const Message& other);
        char* getQueueName()
        {
            return m_queueName;
        }
        long getSeqId()
        {
            return m_seqId;
        }

        void print();
    protected:
    private:
        char* m_queueName;
        char* m_body;
        long m_seqId;
};

#endif // MESSAGE_H
