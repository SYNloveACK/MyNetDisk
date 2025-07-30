#ifndef TCPNET_H
#define TCPNET_H
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include"INet.h"
#define MAXNUM   1024
// szType 0 文件信息   1 文件内容     2 文件回复
#include"kernel/IKernel.h"
class TCPNet :public INet
{
public:
    TCPNet(IKernel* pKernel);
    ~TCPNet();
public:
    bool connectServer(const char* szip = "127.0.0.1",short sport=1234);
    void disconnectServer(const char* szerr);
    bool sendData(const char* szbuf,int nlen);
    void recvData();
    static DWORD WINAPI ThreadProc(LPVOID lpvoid);
private:
    SOCKET m_sockclient;
    HANDLE m_hThread;
    bool   m_bflagQuit;
    IKernel* m_pKernel;
};

#endif // TCPNET_H
