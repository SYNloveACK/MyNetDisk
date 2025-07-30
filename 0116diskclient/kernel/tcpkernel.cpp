#include "tcpkernel.h"

TCPKernel::TCPKernel(QObject *parent)
    : QObject{parent}
{
    m_pTCPNet=new TCPNet(this);
}

TCPKernel::~TCPKernel()
{
    delete m_pTCPNet;
}

bool TCPKernel::connect(const char *szip, short sport)
{
    if(!m_pTCPNet->connectServer(szip,sport))
    {
        return false;
    }
    return true;
}

void TCPKernel::disconnect(const char *szerr)
{
    m_pTCPNet->disconnectServer(szerr);
}

bool TCPKernel::sendData(const char *szbuf, int nlen)
{
    if(!m_pTCPNet->sendData(szbuf,nlen))
        return false;
    return true;
}

void TCPKernel::dealData(const char *szbuf)
{
    //todo
    switch(*szbuf)
    {
    case _default_protocol_register_rs:
    {
        emit signal_register((STRU_REGISTER_RS*)szbuf);
    }
    break;
    case _default_protocol_login_rs:
        emit signal_login((STRU_LOGIN_RS*)szbuf);
        break;
    case _default_protocol_getfilelist_rs:
        emit signal_getfilelist((STRU_GETFILELIST_RS*)szbuf);
        break;
    case _default_protocol_uploadfileinfo_rs:
        emit signal_uploadfileinfo((STRU_UPLOADFILEINFO_RS*)szbuf);
        break;
    case _default_protocol_sharelink_rs:
        emit signal_sharelink((STRU_SHARELLINK_RS*)szbuf);
        break;
    case _default_protocol_break_rs:
        emit signal_break((STRU_BREAK_RS*)szbuf);
        break;
    case _default_protocol_searchfile_rs:
        emit signal_searchfile((STRU_SEARCHFILE_RS*)szbuf);
        break;
    case _default_protocol_downloadfile_rs:

        STRU_DOWNLOADFILE_RS *p=(STRU_DOWNLOADFILE_RS*)szbuf;
        qDebug()<<"收到downloadrs"<<p->totalfilesize;
        emit signal_downlowad((STRU_DOWNLOADFILE_RS*)szbuf);
        break;
    }
}

