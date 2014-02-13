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

    if ((sockFd = socket(AF_INET, SOCK_DGRAM, 0)) == ERROR) {
        SmartDashboard::PutString("Data Recvd", "socket error");
        return(1);
    }

    if (bind(sockFd, (struct sockaddr *) &serverAddr, sockAddrSize) == ERROR) {
       SmartDashboard::PutString("Data Recvd", "bind error"); 
       return(1);
    }

    SmartDashboard::PutString("Data Recvd", "waiting...");
    int bytesRecvd = 0;
    char buf[256];
    while (buf[0] != '2') {
        memset(buf, 0, sizeof(buf));
        if ((bytesRecvd = recvfrom(sockFd, buf, 255, 0, (struct sockaddr *) &clientAddr, &sockAddrSize)) == ERROR) {
           SmartDashboard::PutString("Data Recvd", "recv error");
           return(1);
        }
    }

    return(0);

}
