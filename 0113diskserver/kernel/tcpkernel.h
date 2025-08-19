#ifndef TCPKERNEL_H
#define TCPKERNEL_H
#include"IKernel.h"
#include"../server/tcpnet.h"
#include"../sql/CMySql.h"
#include"Packdef.h"
#include<time.h>
#include<QTimer>
#include<QDateTime>
#include<thread>
#include<map>
#include <fstream>
#include <windows.h>
#include<QDebug>
#include"./json.hpp"
#include <QFileInfo>
#include "Logger.h"
#define json nlohmann::json
struct UploadFileInfo
{
    FILE* m_pfile;
    long long m_fileSize;
    long long m_pos;
    long long m_userId;
};
class TCPKernel : public IKernel
{
public:

private:
    TCPKernel();
     ~TCPKernel();
    public:
    virtual bool open();
    virtual void close();
    virtual void deelData(SOCKET sock,char*szbuf);
    virtual void dealJsonData(SOCKET sock,char* szbuf);
    void registerrq(SOCKET sock,char* szbuf);
    void loginrq(SOCKET sock,char* szbuf);
    void getfilelistrq(SOCKET sock,char* szbuf);
    void uploadfileinforq(SOCKET sock,char* szbuf);
    void uploadfileblockrq(SOCKET sock,char* szbuf);
    void sharelinkrq(SOCKET sock,char* szbuf);
    void breakrq(SOCKET sock,char*szbuf);
    void heartbeatrq(SOCKET sock);
    void searchfilerq(SOCKET sock,char* szbuf);
    void downloadfile(SOCKET sock,char* szbuf);
    void getlinkfile(SOCKET sock,json js);
    static IKernel* getKernel()
    {
        return m_pKernel;
    }
    //饿汉模式 支持线程安全，高效
private:
    INet* m_pTCPNet;
    CMySql* m_pSQL;
    static TCPKernel * m_pKernel;
    char m_szSystemPath[MAX_PATH];
    std::map<long long,UploadFileInfo*> m_mapFileIdtoFileInfo;
    std::map<SOCKET,QDateTime> m_mapSocktoQDateTime;
};

#endif // TCPKERNEL_H
