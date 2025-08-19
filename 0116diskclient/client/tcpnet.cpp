#include "tcpnet.h"
#include<iostream>
using namespace std;
TCPNet::TCPNet(IKernel* pKernel)
{
    m_sockclient =0;
    m_hThread  = 0;
    m_bflagQuit = true;
    m_pKernel=pKernel;
}
TCPNet::~TCPNet()
{

}

bool TCPNet::connectServer(const char* szip ,short sport)
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

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

    m_sockclient = socket(AF_INET,SOCK_STREAM,0);
    if(INVALID_SOCKET == m_sockclient){
        disconnectServer("socket err\n");

    }

    sockaddr_in addrserver;
    addrserver.sin_family =AF_INET;
    addrserver.sin_addr.S_un.S_addr = inet_addr(szip);
    addrserver.sin_port = htons(sport);
    if(SOCKET_ERROR == connect(m_sockclient,(const sockaddr*)&addrserver,sizeof(addrserver))){
        disconnectServer("connect err\n");
        return false;
    }

    //创建线程--接收数据
    m_hThread = CreateThread(0,0,&ThreadProc,this,0,0);
    return true;
}

DWORD WINAPI TCPNet::ThreadProc(LPVOID lpvoid)
{
    TCPNet *pthis = (TCPNet*)lpvoid;
    pthis->recvData();
    return 0;
}
void TCPNet::disconnectServer(const char* szerr)
{
    m_bflagQuit =false;
    if(m_hThread){
        if(WAIT_TIMEOUT == WaitForSingleObject(m_hThread,100))
            TerminateThread(m_hThread,-1);

        CloseHandle(m_hThread);
        m_hThread = NULL;
    }

    printf(szerr);
    if(m_sockclient){
        closesocket(m_sockclient);
        m_sockclient =0;
    }
    WSACleanup();

}
///*
bool TCPNet::sendData(const char* szbuf,int nlen)
{
    if(!szbuf ||nlen <=0 )
        return false;
    //包大小
    if(send(m_sockclient,(char*)&nlen,sizeof(int),0)<=0)
        return false;

    //包内容
    if(send(m_sockclient,szbuf,nlen,0)<=0)
        return false;

    //序列化和反序列化--protobuf
    return true;
}
//bool isProbablyJson(const std::string& str) {
//    if (str.empty()) return false;

//    // 去除首尾空白字符
//    auto start = str.find_first_not_of(" \t\n\r");
//    auto end = str.find_last_not_of(" \t\n\r");
//    if (start == std::string::npos || end == std::string::npos) {
//        return false;  // 全是空白字符
//    }

//    // 检查第一个和最后一个非空字符
//    char firstChar = str[start];
//    char lastChar = str[end];

//    // 合法 JSON 应该以 { 或 [ 开头，以 } 或 ] 结尾
//    bool isObject = (firstChar == '{' && lastChar == '}');
//    bool isArray = (firstChar == '[' && lastChar == ']');

//    if (!isObject && !isArray) {
//        return false;  // 明显不符合 JSON 结构
//    }

//    // 可选：简单长度检查（避免 {} 或 []）
//    if (end - start + 1 <= 2) {
//        return false;  // 太短，可能是空对象或空数组
//    }

//    return true;  // 可能是 JSON
//}

#include <string>
#include <cctype>

bool isProbablyJson(const std::string& str) {
    // 检查空字符串
    if (str.empty()) return false;

    // 跳过首尾空白字符
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    if (start == std::string::npos || end == std::string::npos) {
        return false;  // 全是空白
    }

    // 检查首尾字符
    const char first = str[start];
    const char last = str[end];
    const bool isObject = (first == '{' && last == '}');
    const bool isArray = (first == '[' && last == ']');
    if (!isObject && !isArray) return false;

    // 允许空对象/空数组
    if (end - start + 1 == 2) return true;  // "{}" 或 "[]"

    // === 关键增强 ===
    // 1. 检查引号平衡（忽略转义引号）
    int quoteCount = 0;
    bool inEscape = false;

    for (size_t i = start; i <= end; ++i) {
        const char c = str[i];

        if (c == '\\' && !inEscape) {
            inEscape = true;  // 标记转义开始
            continue;
        }

        if (c == '"' && !inEscape) {
            quoteCount++;  // 只计数非转义引号
        }

        inEscape = false;  // 重置转义状态
    }

    // 引号必须成对出现
    if (quoteCount % 2 != 0) return false;

    // 2. 对象必须包含键值对特征
    if (isObject) {
        // 要求存在冒号且不在首尾位置
        size_t colonPos = str.find(':', start + 1);
        if (colonPos == std::string::npos ||   // 无冒号
            colonPos >= end - 1) {             // 冒号太靠尾
            return false;
        }

        // 冒号前应有键的特征（双引号或字母）
        const char prev = str[colonPos - 1];
        if (prev != '"' && !std::isalpha(prev)) {
            return false;
        }
    }

    // 3. 数组必须包含有效分隔符
    if (isArray) {
        // 检查是否存在数组元素分隔符或有效值
        bool hasValidContent = false;
        for (size_t i = start + 1; i < end; ++i) {
            const char c = str[i];
            // 合法值特征：数字/字母/引号/结构符
            if (c == ',' || c == '"' || std::isdigit(c) ||
                c == '{' || c == '[' || std::isalpha(c)) {
                hasValidContent = true;
                break;
            }
        }
        if (!hasValidContent) return false;
    }

    return true;  // 通过所有基础检查
}
void TCPNet::recvData()
{

    int nPackSize;
    int nReadNum;
    char *pszbuf = NULL;
    while(m_bflagQuit){
       // 接收数据
       // 接收包大小
        nReadNum = recv(m_sockclient,(char*)&nPackSize,sizeof(int),0);
        if(nReadNum <=0){

            //服务器异常
            break;
        }
        if(nPackSize <=0)
            continue;

        pszbuf = new char[nPackSize+1];
        int len=nPackSize;
        //接收包内容
        int offset = 0;
        while(nPackSize>0){
            nReadNum = recv(m_sockclient,pszbuf+offset,nPackSize,0);
            offset += nReadNum;
            nPackSize -= nReadNum;

        }
        pszbuf[offset]='\0';
        if(isProbablyJson(pszbuf))
        {
            m_pKernel->dealDatabyJson(pszbuf);
        }
        else
        {
            m_pKernel->dealData(pszbuf);
        }


        delete []pszbuf;
        pszbuf = NULL;

    }
}
