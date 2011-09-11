#include <iostream>
#include "connection_manager.h"
#include <signal.h>

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
    ConnectionManager& rCM = ConnectionManager::instance();
    rCM.run();
    cout << "Program exiting..." << endl;
}
