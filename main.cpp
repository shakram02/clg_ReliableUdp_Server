
#include <sys/socket.h>
#include "timecalc.h"
#include "../libs/data_packet.h"

#include "WelcomingSocket.h"
#include "ClientObserver.h"
#include "WorkerClasses/FileFragmenter.h"

//#define SERVER_IP "192.168.1.7"
#define SERVER_IP "127.0.0.1"
#define PORT_NUM 62135

void log_error(const char *);

void *get_in_addr(struct sockaddr *);

void TestFileFragmenter()
{
//    FileFragmenter ff("mizo.txt", 5);
//    void *fragm;
//
//    while (!ff.EndOfFile()) {
//        ff.NextFragment(&fragm);
//        cout << "frag: " << (char *) fragm << endl;
//    }
//
//    free(fragm);
}

int main()
{
    ClientObserver obs(SERVER_IP, PORT_NUM);
    obs.StartListening();

    string d;
    cout << "Hit enter to terminate..." << endl;
    cin >> d;

    obs.StopListening();

//    TestFileFragmenter();
    return 0;
}
