#include "WPILib.h"

int NetTest() {
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;

    int sockAddrSize;
    int sockFd;


    SmartDashboard::PutString("Data Recvd", "initializing");
    sockAddrSize = sizeof(struct sockaddr_in);
    bzero((char *) &serverAddr, sockAddrSize);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_len = (u_char) sockAddrSize;
    serverAddr.sin_port = htons(1140);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
}
