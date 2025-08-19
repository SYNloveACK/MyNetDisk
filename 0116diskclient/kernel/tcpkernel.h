#ifndef TCPKERNEL_H
#define TCPKERNEL_H

#include<QDebug>>
#include <QObject>
#include"../client/tcpnet.h"
#include"IKernel.h"
#include"Packdef.h"
#include"./json.hpp"
#include"logger.h"
using json = nlohmann::json;
class TCPKernel : public QObject,public IKernel
{
    Q_OBJECT
public:
    explicit TCPKernel(QObject *parent = nullptr);
    ~TCPKernel();
public:
     bool connect(const char* szip = "127.0.0.1",short sport=1234);
     void disconnect(const char* szerr);
     bool sendData(const char* szbuf,int nlen);
     void dealData(const char* szbuf);
     void dealDatabyJson(const char*szbuf);
signals:
     void signal_register(STRU_REGISTER_RS* );
    void signal_login(STRU_LOGIN_RS* );
     void signal_getfilelist(STRU_GETFILELIST_RS* );
    void signal_uploadfileinfo(STRU_UPLOADFILEINFO_RS*);
      void signal_sharelink(STRU_SHARELLINK_RS*);
    void signal_break(STRU_BREAK_RS*);
      void signal_searchfile(STRU_SEARCHFILE_RS*);
    void signal_downlowad(STRU_DOWNLOADFILE_RS*);
      void signal_downloadbyjson(json& js);
 private:
    TCPNet * m_pTCPNet;

};

#endif // TCPKERNEL_H
