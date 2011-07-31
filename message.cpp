#include "message.h"
#include <iostream>

using namespace std;

Message::Message(const char* qName, const char* message)
{
    m_queueName = static_cast<char *>(malloc(strlen(qName) + 1));
    bzero(m_queueName, strlen(qName) + 1);
    strncpy(m_queueName, qName, strlen(qName));
    m_body = static_cast<char *>(malloc(strlen(message) + 1));
    bzero(m_body, strlen(message) + 1);
    strncpy(m_body, message, strlen(message));
    m_seqId = atol(m_body);
}

Message::~Message()
{
    //dtor
}

Message::Message(const Message& other)
{
    //copy ctor
}

Message& Message::operator=(const Message& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

void Message::print()
{
    printf("QUEUE: %s\nMESAG: %s\n", m_queueName, m_body);
}
