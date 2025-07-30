#ifndef PACKDEF_H
#define PACKDEF_H

#define _default_protocol_base   10
//申请账号
#define _default_protocol_register_rq   _default_protocol_base +1
#define _default_protocol_register_rs   _default_protocol_base +2
//登陆
#define _default_protocol_login_rq   _default_protocol_base +3
#define _default_protocol_login_rs   _default_protocol_base +4
//获取文件列表
#define _default_protocol_getfilelist_rq   _default_protocol_base +5
#define _default_protocol_getfilelist_rs   _default_protocol_base +6
//上传文件---正常传文件、断点续传、秒传
#define _default_protocol_uploadfileinfo_rq   _default_protocol_base +7
#define _default_protocol_uploadfileinfo_rs   _default_protocol_base +8
#define _default_protocol_uploadfileblock_rq   _default_protocol_base +9
#define _default_protocol_uploadfileblock_rs   _default_protocol_base +10

//搜索文件
#define _default_protocol_searchfile_rq   _default_protocol_base +11
#define _default_protocol_searchfile_rs   _default_protocol_base +12

//删除文件
#define _default_protocol_deletefile_rq   _default_protocol_base +13
#define _default_protocol_deletefile_rs   _default_protocol_base +14

#define _default_protocol_sharelink_rq   _default_protocol_base +15
#define _default_protocol_sharelink_rs   _default_protocol_base +16
//提取
#define _default_protocol_getlink_rq   _default_protocol_base +17
#define _default_protocol_getlink_rs   _default_protocol_base +18

#define _default_protocol_break_rq      _default_protocol_base+19
#define _default_protocol_break_rs      _default_protocol_base+20

//心跳
#define _default_protocol_heartbeat_rq      _default_protocol_base+21


//下载文件
#define _default_protocol_downloadfile_rq  _default_protocol_base+22
#define _default_protocol_downloadfile_rs  _default_protocol_base+23
//添加好友
//搜索好友
//删除好友
//聊天

//协议包

#define MAXSIZE  45
#define FILENUM  15
#define SQLLEN 300
#define ONE_PAGE 4096

#define _regist_err 0
#define _regist_success 1

#define _login_usernoexist  0
#define _login_passwderr    1
#define _login_success      2

#define _uploadinfo_isuploaded 0
#define _uploadinfo_continue 1//断点续传
#define _uploadinfo_speedtranfer 2//秒传
#define _uploadinfo_normal 3
#define _heartbeattime 10

#pragma pack(push, 1)

struct STRU_BASE{
    char m_nType;
};

//申请账号
struct STRU_REGISTER_RQ:public STRU_BASE{

    STRU_REGISTER_RQ(){
        m_nType = _default_protocol_register_rq;
    }
    char m_szName[MAXSIZE];
    char m_szPassword[MAXSIZE];
    long long m_tel;
};
struct STRU_REGISTER_RS:public STRU_BASE{
    STRU_REGISTER_RS(){
        m_nType = _default_protocol_register_rs;
    }
    char m_szResult;
};
//登陆
//申请账号
struct STRU_LOGIN_RQ:public STRU_BASE{

    STRU_LOGIN_RQ(){
        m_nType = _default_protocol_login_rq;
    }
    char m_szName[MAXSIZE];
    char m_szPassword[MAXSIZE];

};
struct STRU_LOGIN_RS:public STRU_BASE{
    STRU_LOGIN_RS(){
        m_nType = _default_protocol_login_rs;
    }
    long long m_userId;
    char m_szResult;
};
//获取文件列表
struct STRU_GETFILELIST_RQ:public STRU_BASE{

    STRU_GETFILELIST_RQ(){
        m_nType = _default_protocol_getfilelist_rq;
    }
    long long m_userId;
};

struct FileInfo{
    char m_szFileName[MAXSIZE];
    char m_szFileDateTime[MAXSIZE];
    long long m_fileSize;
};

struct STRU_GETFILELIST_RS:public STRU_BASE{

    STRU_GETFILELIST_RS(){
        m_nType = _default_protocol_getfilelist_rs;
    }
    FileInfo  m_aryInfo[FILENUM];
    long  m_FileNum;
};
struct STRU_UPLOADFILEINFO_RQ:public STRU_BASE
{
    STRU_UPLOADFILEINFO_RQ()
    {
        m_nType=_default_protocol_uploadfileinfo_rq;
    }
    long long m_userid;
    char m_szFileName[MAXSIZE];
    long long m_filesize;
    char m_szFileMD5[MAXSIZE];

};
struct STRU_UPLOADFILEINFO_RS:public STRU_BASE
{
    STRU_UPLOADFILEINFO_RS()
    {
        m_nType=_default_protocol_uploadfileinfo_rs;
    }
    char m_szFileName[MAXSIZE];
    long long m_fileId;
    long long m_pos;
    char m_szResult;
};
struct STRU_UPLOADFILEBLOCK_RQ:public STRU_BASE
{
    STRU_UPLOADFILEBLOCK_RQ()
    {
        m_nType=_default_protocol_uploadfileblock_rq;
    }
    long long m_fileId;
    char m_szFileContent[ONE_PAGE];
    long m_fileNum;
};

//分享
struct STRU_SHARELINK_RQ:public STRU_BASE{
    STRU_SHARELINK_RQ(){
        m_nType = _default_protocol_sharelink_rq;
    }

    long long m_userId;
    char m_szFileName[MAXSIZE];

};
struct STRU_SHARELLINK_RS:public STRU_BASE{
    STRU_SHARELLINK_RS(){
        m_nType = _default_protocol_sharelink_rs;
    }
    char m_szFileName[MAXSIZE];
    char m_szLink[MAXSIZE];
    char m_szCode[MAXSIZE];

};

//提取
struct STRU_GETLINK_RQ:public STRU_BASE{
    STRU_GETLINK_RQ(){
        m_nType = _default_protocol_getlink_rq;
    }

    long long m_userId;
    char m_szCode[MAXSIZE];
};
struct STRU_GETLINK_RS:public STRU_BASE{
    STRU_GETLINK_RS(){
        m_nType = _default_protocol_getlink_rs;
    }

    long long m_userId;
    char m_szFileName[MAXSIZE];
    long long m_FileSize;
    char m_szFileUploadTime[MAXSIZE];
};

struct STRU_BREAK_RQ:public STRU_BASE{
    STRU_BREAK_RQ(){
        m_nType = _default_protocol_break_rq;
    }

};
struct STRU_BREAK_RS:public STRU_BASE{
    STRU_BREAK_RS(){
        m_nType = _default_protocol_break_rs;
    }
};

struct STRU_HEARTBEAT_RQ:public STRU_BASE{
    STRU_HEARTBEAT_RQ(){
        m_nType = _default_protocol_heartbeat_rq;
    }
};

struct STRU_SEARCHFILE_RQ:public STRU_BASE{
    STRU_SEARCHFILE_RQ(){
        m_nType=_default_protocol_searchfile_rq;
    }
    char filename[MAXSIZE];
        long long m_userId;
};

#include<string>
#include<list>
using namespace std;
struct STRU_SEARCHFILE_RS:public STRU_BASE{
        STRU_SEARCHFILE_RS(){
        m_nType=_default_protocol_searchfile_rs;
        }
        bool result;
    //list<string> fileinfo;
        FileInfo  m_aryInfo[FILENUM];
        long  m_FileNum;
};


struct STRU_DOWNLOADFILE_RQ:public STRU_BASE
{
        STRU_DOWNLOADFILE_RQ()
        {
        m_nType = _default_protocol_downloadfile_rq;
        }
        long long m_userid;
        char filename[MAXSIZE];
        long long file_size;
        char downloadpath[MAXSIZE] ;
};

struct STRU_DOWNLOADFILE_RS:public STRU_BASE
{
        STRU_DOWNLOADFILE_RS()
        {
        m_nType=_default_protocol_downloadfile_rs;
        }
        char m_filecontent[ONE_PAGE];
        int pagesize;
        int totalfilesize;
        int fileid;
        char downloadpath[MAXSIZE] ;
        char filename[MAXSIZE];
};
#pragma pack(pop)
/*
 用户表user(u_id,u_name,u_password,u_tel)
  文件信息表file(f_id,f_name,f_uploadtime,f_path,f_count,f_md5)
user_file(num,u_id,f_id)
 *
 *用户分享文件表
 *用户朋友表
*/


#endif // PACKDEF_H
