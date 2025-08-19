#include "tcpnet.h"
#include"../kernel/tcpkernel.h"
#include <algorithm>
#include <cctype>
TCPNet::TCPNet()
{
    m_socklisten = 0;
    m_bflagQuit = true;
}

TCPNet::~TCPNet()
{
    unInitNetWork("TCPNet ����...\n");
}

bool TCPNet::initNetWork(const char *szip, short sport)
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {

        printf("WSAStartup failed with error: %d\n", err);
        return false;
    }


    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {

        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
        return false;
    }
    else
        printf("The Winsock 2.2 dll was found okay\n");

    m_socklisten = socket(AF_INET,SOCK_STREAM,0);
    if(INVALID_SOCKET == m_socklisten){
        unInitNetWork("socket err\n");
        return false;
    }
    sockaddr_in addrserver;
    addrserver.sin_family =AF_INET;
    addrserver.sin_addr.S_un.S_addr =0 /*inet_addr(szip)*/;
    addrserver.sin_port = htons(sport);
    if(SOCKET_ERROR ==bind(m_socklisten,(const sockaddr*)&addrserver,sizeof(addrserver))){
          unInitNetWork("bind err\n");
          return false;
    }

    if(SOCKET_ERROR ==listen(m_socklisten,128)){
         unInitNetWork("listen err\n");
         return false;
    }
    //�����߳�---��������---
    HANDLE hThread = CreateThread(0,0,&ThreadProc,this,0,0);
    if(hThread)
         m_lstThread.push_back(hThread);
    return true;
}


DWORD TCPNet::ThreadProc(LPVOID lpvoid)
{
    TCPNet *pthis = (TCPNet*)lpvoid;
    sockaddr_in addrclient;
    int nsize = sizeof(sockaddr_in);

    DWORD dwThreadId = NULL;
    while(pthis->m_bflagQuit){
         //�ȴ�����
         SOCKET sockwaiter = accept(pthis->m_socklisten,(struct sockaddr*)&addrclient,&nsize);
         printf("client ip:%s,port:%d\n",inet_ntoa(addrclient.sin_addr),ntohs(addrclient.sin_port));

         //�����߳�
         HANDLE hThread =  CreateThread(0,0,&ThreadRecv,(LPVOID)pthis,0,&dwThreadId);
         if(hThread)
         {
             pthis->m_lstThread.push_back(hThread);
             pthis->m_mapThreadIdToSocket[dwThreadId] = sockwaiter;
         }
    }
    return 0;
}

DWORD TCPNet::ThreadRecv(LPVOID lpvoid)
{
    TCPNet *pthis = (TCPNet*)lpvoid;
    pthis->recvData();
    return 0;
}

void TCPNet::unInitNetWork(const char* szerr)
{
    m_bflagQuit = false;
    auto ite = m_lstThread.begin();
    while(ite != m_lstThread.end()){

         //�ж��߳��Ƿ��˳�
         if(WAIT_TIMEOUT == WaitForSingleObject(*ite,100))
             TerminateThread(*ite,-1);

         CloseHandle(*ite);
         *ite= NULL;
         ite++;
    }
    m_lstThread.clear();
    m_mapThreadIdToSocket.clear();
    printf(szerr);
    if(m_socklisten){

       closesocket(m_socklisten);
    }
    WSACleanup();



}

bool TCPNet::sendData(SOCKET sock, const char *szbuf, int nlen)
{
    if(sock == INVALID_SOCKET || !szbuf ||nlen <=0 )
       return false;
    //����С
    if(send(sock,(char*)&nlen,sizeof(int),0)<=0)
       return false;

    //������
    if(send(sock,szbuf,nlen,0)<=0)
       return false;

    //���л��ͷ����л�--protobuf
    return true;
}

/*
bool TCPNet::sendData(SOCKET sock, const char *szbuf, int nlen)
{
    // 1. ����У��
    if (sock == INVALID_SOCKET || !szbuf || nlen <= 0) {
       return false;
    }

    // 2. �������ݳ��ȣ��̶�4�ֽڣ������ֽ���
    uint32_t netLen = htonl(static_cast<uint32_t>(nlen)); // ����ֽ�������
    const char* pLen = reinterpret_cast<const char*>(&netLen);
    int remaining = sizeof(netLen);

    while (remaining > 0) {
       int sent = send(sock, pLen + (sizeof(netLen) - remaining), remaining, 0);
       if (sent <= 0) {
             if (sent == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK) {
                 // ������ģʽ�µȴ���д
                 fd_set writeSet;
                 FD_ZERO(&writeSet);
                 FD_SET(sock, &writeSet);
                 timeval timeout{1, 0}; // 1�볬ʱ
                 if (select(0, NULL, &writeSet, NULL, &timeout) <= 0) {
                     return false; // ��ʱ�����
                 }
                 continue;
             }
             return false; // ��������
       }
       remaining -= sent;
    }

    // 3. ѭ��������������
    remaining = nlen;
    const char* pData = szbuf;
    while (remaining > 0) {
       int sent = send(sock, pData + (nlen - remaining), remaining, 0);
       if (sent <= 0) {
             if (sent == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK) {
                 // ������ģʽ�µȴ���д
                 fd_set writeSet;
                 FD_ZERO(&writeSet);
                 FD_SET(sock, &writeSet);
                 timeval timeout{1, 0};
                 if (select(0, NULL, &writeSet, NULL, &timeout) <= 0) {
                     return false;
                 }
                 continue;
             }
             return false;
       }
       remaining -= sent;
    }

    return true;
}
*/
bool isStringValidJson(const std::string& str) {
    return json::accept(str);  // ֱ�ӷ����Ƿ�Ϸ�
}
bool isProbablyJson(const std::string& str) {
    if (str.empty()) return false;

    // ȥ����β�հ��ַ�
    auto start = str.find_first_not_of(" \t\n\r");
    auto end = str.find_last_not_of(" \t\n\r");
    if (start == std::string::npos || end == std::string::npos) {
       return false;  // ȫ�ǿհ��ַ�
    }

    // ����һ�������һ���ǿ��ַ�
    char firstChar = str[start];
    char lastChar = str[end];

    // �Ϸ� JSON Ӧ���� { �� [ ��ͷ���� } �� ] ��β
    bool isObject = (firstChar == '{' && lastChar == '}');
    bool isArray = (firstChar == '[' && lastChar == ']');

    if (!isObject && !isArray) {
       return false;  // ���Բ����� JSON �ṹ
    }

    // ��ѡ���򵥳��ȼ�飨���� {} �� []��
    if (end - start + 1 <= 2) {
       return false;  // ̫�̣������ǿն���������
    }

    return true;  // ������ JSON
}
void TCPNet::recvData()
{
    SOCKET sockWaiter =  m_mapThreadIdToSocket[GetCurrentThreadId()];
    //��ô�������Ա
    int nPackSize;
    int nReadNum;
    char *pszbuf = NULL;
    while(m_bflagQuit){
       //��������
       //���հ���С
       nReadNum = recv(sockWaiter,(char*)&nPackSize,sizeof(int),0);
       if(nReadNum <=0){
             //����ǿͻ���������
             //���տͻ�����Դ
             break;
       }
       if(nPackSize <=0)
             continue;

       pszbuf = new char[nPackSize];
       //���հ�����
       int offset = 0;
       while(nPackSize){
             nReadNum = recv(sockWaiter,pszbuf+offset,nPackSize,0);
             offset += nReadNum;
             nPackSize -= nReadNum;

       }

      //printf("client say:%s\n",pszbuf);

       if(isProbablyJson(pszbuf))
       {
            TCPKernel::getKernel()->dealJsonData(sockWaiter,pszbuf);
       }
       else
       {
            TCPKernel::getKernel()->deelData(sockWaiter,pszbuf);
       }

       delete []pszbuf;
       pszbuf = NULL;



    }
}

