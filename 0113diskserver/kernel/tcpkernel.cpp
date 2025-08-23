#include "tcpkernel.h"
#include<iostream>
using namespace std;
TCPKernel* TCPKernel::m_pKernel=new TCPKernel;
#include <string>
#include <vector>

// Base64编码表
static const char base64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

std::string base64_encode(const uint8_t* data, size_t len) {
    std::string encoded;
    encoded.reserve(((len + 2) / 3) * 4); // 预留足够空间

    int i = 0;
    int j = 0;
    uint8_t char_array_3[3];
    uint8_t char_array_4[4];

    while (len--) {
        char_array_3[i++] = *(data++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; i < 4; i++) {
                encoded += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }

    // 处理剩余字节
    if (i) {
        for (j = i; j < 3; j++) {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; j < i + 1; j++) {
            encoded += base64_chars[char_array_4[j]];
        }

        // 添加填充字符
        while (i++ < 3) {
            encoded += '=';
        }
    }

    return encoded;
}
void heartbeatcheck(std::map<SOCKET,QDateTime> * mp)
{
    while(1){
        auto ite=mp->begin();
        while(ite!=mp->end()){
            printf("time tick\n");
            if(ite->second<QDateTime::currentDateTime()){
                closesocket(ite->first);
                printf("disconnect\n");
                ite=mp->erase(ite);
                continue;
            }
            ite++;
        }
        this_thread::sleep_for(std::chrono::seconds(_heartbeattime));
    }
}

TCPKernel::TCPKernel()
{
    m_pTCPNet=new TCPNet;
    m_pSQL=new CMySql;
    strcpy(m_szSystemPath,"C:/mydisk/");
    std::thread check(heartbeatcheck,&m_mapSocktoQDateTime);
    check.detach();
}

TCPKernel::~TCPKernel()
{
    delete m_pTCPNet;
    delete m_pSQL;
}

bool TCPKernel::open()
{
    if(!m_pTCPNet->initNetWork())
    {
        printf("init network err\n");
        return false;
    }
    if(!m_pSQL->ConnectMySql("localhost","root","Zx040322","0113disk"))
    {
        printf("connect mysql err\n");
        return false;
    }
    return true;
}

void TCPKernel::close()
{
    m_pTCPNet->unInitNetWork("over");
    m_pSQL->DisConnect();
}

void TCPKernel::deelData(SOCKET sock, char *szbuf)
{
    //todo
    switch(*szbuf)
    {
    case _default_protocol_register_rq:
    {
        LOG_INFO("sock:{}  _default_protocol_register_rq",sock);
        registerrq(sock,szbuf);
    }
    break;
    case _default_protocol_login_rq:{
        LOG_INFO("sock:{}  _default_protocol_login_rq",sock);
        loginrq(sock,szbuf);
    }
    break;
    case _default_protocol_getfilelist_rq:
    {
        LOG_INFO("sock:{}  _default_protocol_getfilelist_rq",sock);
        getfilelistrq(sock,szbuf);
    }
    break;
    case _default_protocol_uploadfileinfo_rq:
    {
                LOG_INFO("sock:{}  _default_protocol_uploadfileinfo_rq",sock);
        uploadfileinforq(sock,szbuf);
    }
    break;
    case _default_protocol_uploadfileblock_rq:
    {
        LOG_INFO("sock:{}  _default_protocol_uploadfileblock_rq",sock);
        uploadfileblockrq(sock,szbuf);
    }
    break;
    case _default_protocol_sharelink_rq:
    {
        LOG_INFO("sock:{}  _default_protocol_sharelink_rq",sock);
        sharelinkrq(sock,szbuf);
    }
    break;
    case _default_protocol_break_rq:
    {
        LOG_INFO("sock:{}  _default_protocol_break_rq",sock);
        breakrq(sock,szbuf);
    }
    break;
    case _default_protocol_heartbeat_rq:
    {
        LOG_INFO("sock:{}  _default_protocol_heartbeat_rq",sock);
        heartbeatrq(sock);
    }
    break;
    case _default_protocol_searchfile_rq:
    {
        LOG_INFO("sock:{}  _default_protocol_searchfile_rq",sock);
        searchfilerq(sock,szbuf);
    }
    break;
    case _default_protocol_downloadfile_rq:
    {
        LOG_INFO("sock:{}  _default_protocol_downloadfile_rq",sock);
        downloadfile(sock,szbuf);
    }
    break;

    }

}

void TCPKernel::dealJsonData(SOCKET sock, char *szbuf)
{
    json js=json::parse(szbuf);
    int type =js["type"];
    switch(type)
    {
    case _default_protocol_getlink_rq:
    {
        LOG_INFO("sock:{}  Jsondata :_default_protocol_getlink_rq",sock);
        getlinkfile(sock,js);
        break;
    }
    }
}

void TCPKernel::registerrq(SOCKET sock, char *szbuf)
{
    STRU_REGISTER_RQ *psrr=(STRU_REGISTER_RQ*)szbuf;
    char szsql[SQLLEN]={0};
    char szPath[MAX_PATH]={0};
    list<string> lststr;
    STRU_REGISTER_RS srr;
    srr.m_szResult=_regist_err;
    sprintf(szsql,"insert into user (u_name,u_password,u_tel) values('%s','%s','%lld')",
            psrr->m_szName,psrr->m_szPassword,psrr->m_tel);
    if(m_pSQL->UpdateMySql(szsql))
    {//更新成功
        srr.m_szResult=_regist_success;
        //获取用户ID
        sprintf(szsql,"select u_id from user where u_tel=%lld",psrr->m_tel);
        m_pSQL->SelectMySql(szsql,1,lststr);
        if(lststr.size()>0)
        {
            string strUserId =lststr.front();
            lststr.pop_front();
            //为当前用户创建目录
            sprintf(szPath,"%s%s",m_szSystemPath,strUserId.c_str());

            CreateDirectoryA(szPath,0);
        }

    }
    //发送回复--序列化 反序列化(省略了)
    m_pTCPNet->sendData(sock,(char*)&srr,sizeof(srr));

}

void TCPKernel::loginrq(SOCKET sock, char *szbuf)
{
    STRU_LOGIN_RQ *pslr =(STRU_LOGIN_RQ*)szbuf;
    char szsql[SQLLEN]={0};

    list<string> lststr;
    STRU_LOGIN_RS slr;
    slr.m_szResult=_login_usernoexist;
    sprintf(szsql,"select u_id,u_password from user where u_name ='%s';",pslr->m_szName);
    m_pSQL->SelectMySql(szsql,2,lststr);
    if(lststr.size()>0)
    {
        slr.m_szResult=_login_passwderr;
        string strUserId=lststr.front();
        lststr.pop_front();
        string strPassword=lststr.front();
        if(0==strcmp(pslr->m_szPassword,strPassword.c_str()))
        {//密码正确
            slr.m_szResult=_login_success;
            slr.m_userId=atoll(strUserId.c_str());
            m_mapSocktoQDateTime.emplace(sock, QDateTime::currentDateTime().addSecs(_heartbeattime+4));
        }
    }
    //发回复
    m_pTCPNet->sendData(sock,(char*)&slr,sizeof(slr));

}

void TCPKernel::getfilelistrq(SOCKET sock, char *szbuf)
{
    STRU_GETFILELIST_RQ *psgr=(STRU_GETFILELIST_RQ*)szbuf;
    char szsql[SQLLEN]={0};
    list<string>lststr;
    STRU_GETFILELIST_RS sgr;
    sprintf(szsql,"select f_name,f_size,f_uploadtime from myview where u_id=%lld;",psgr->m_userId);
    m_pSQL->SelectMySql(szsql,3,lststr);
    int i=0;
    while(lststr.size()>0)
    {
        string strfileName=lststr.front();
        lststr.pop_front();
        string strFileSize=lststr.front();
        lststr.pop_front();
        string strFileUploadTime=lststr.front();
        lststr.pop_front();
        strcpy(sgr.m_aryInfo[i].m_szFileName,strfileName.c_str());
        strcpy(sgr.m_aryInfo[i].m_szFileDateTime,strFileUploadTime.c_str());
        sgr.m_aryInfo[i].m_fileSize=atoll(strFileSize.c_str());
        i++;
        if(i==FILENUM||lststr.size()==0)
        {
            sgr.m_FileNum=i;
            m_pTCPNet->sendData(sock,(const char*)&sgr,sizeof(sgr));
            ZeroMemory(sgr.m_aryInfo,sizeof(sgr.m_aryInfo));
            i++;
        }

    }


}

void TCPKernel::uploadfileinforq(SOCKET sock, char *szbuf)
{
    STRU_UPLOADFILEINFO_RQ *psur=(STRU_UPLOADFILEINFO_RQ*)szbuf;
    char szsql[SQLLEN]={0};
    list<string> lststr;
    STRU_UPLOADFILEINFO_RS sur;
    strcpy(sur.m_szFileName,psur->m_szFileName);
    sur.m_pos=0;
    sprintf(szsql,"select u_id,f_id,f_count from myview where f_md5 = '%s';",psur->m_szFileMD5);
    m_pSQL->SelectMySql(szsql,3,lststr);
    //1 判断数据库是否存在热门文件
    if(lststr.size()>0)
    {
        string strUserId=lststr.front();
        lststr.pop_front();
        string strFileId=lststr.front();
        lststr.pop_front();
        string strFilecount=lststr.front();
        lststr.pop_front();
        long long userId=atoll(strUserId.c_str());
        long long FileId=atoll(strFileId.c_str());
        sur.m_fileId=FileId;
        //1.1判断这个文件是不是自己传的，1如果没有传完--断点续传 2如果传完了--重复上传
        if(psur->m_userid==userId)
        {
            sur.m_szResult=_uploadinfo_isuploaded;
            UploadFileInfo*pInfo =m_mapFileIdtoFileInfo[FileId];
            if(pInfo)
            {
                sur.m_szResult=_uploadinfo_continue;
                printf("continue_transfer\n");
                sur.m_pos=pInfo->m_pos;
            }
            else
            {
                printf("istransfered!\n");
            }

        }
        else
        {
            sur.m_szResult=_uploadinfo_speedtranfer;
            //1.2如果这个文件是别人传的，存在则秒传成功 文件引用计数+1
            sprintf(szsql,"update file set f_count=f_count+1 where f_id=%lld;",FileId);
            m_pSQL->UpdateMySql(szsql);
            sprintf(szsql,"insert into user_file(u_id,f_id) values(%lld,%lld);",psur->m_userid,FileId);
            m_pSQL->UpdateMySql(szsql);
            printf("others transfered!count+1\n");
        }
    }
    else
    {//2 数据库不存在--正常传
        printf("normal transfer!\n");
        sur.m_szResult=_uploadinfo_normal;
        char szFilePath[MAX_PATH]={0};
        //C:\mydisk/+ userid/+ filename
        QString fileName = QString::fromUtf8(psur->m_szFileName);
        sprintf(szFilePath,"%s%lld/%s",m_szSystemPath,psur->m_userid,fileName.toStdString().c_str());
        sprintf(szsql,"insert into file(f_name,f_size,f_path,f_md5) values('%s',%lld,'%s','%s')",
                psur->m_szFileName,psur->m_filesize,szFilePath,psur->m_szFileMD5);


        //2.1传文件信息到数据库
        m_pSQL->UpdateMySql(szsql);
            //2.2将文件与用户信息映射到user_file
        sprintf(szsql,"select f_id from file where f_md5 = '%s'",psur->m_szFileMD5);
        m_pSQL->SelectMySql(szsql,1,lststr);
        if(lststr.size()>0)
        {
            string strFileId=lststr.front();
            lststr.pop_front();
            sprintf(szsql,"insert into user_file(u_id,f_id) values(%lld,%lld);",psur->m_userid,atoll(strFileId.c_str()));
            sur.m_fileId=atoll(strFileId.c_str());
            m_pSQL->UpdateMySql(szsql);
        }

        //2.3创建新文件

        //FILE* pfile=fopen(szFilePath,"wb");
        //记录文件信息 fileid--(文件指针 大小 位置)
        QString fullPath=szFilePath;
        std::wstring wpath = fullPath.toStdWString();
        FILE* pfile = _wfopen(wpath.c_str(), L"wb");
        if(!pfile)
        {
            printf("create file fail\n");
        }
        UploadFileInfo* p=new UploadFileInfo;
        p->m_pfile=pfile;
        p->m_fileSize=psur->m_filesize;
        p->m_pos=0;
        p->m_userId=psur->m_userid;
        m_mapFileIdtoFileInfo[sur.m_fileId]=p;
    }
    m_pTCPNet->sendData(sock,(char*)&sur,sizeof(sur));
}

void TCPKernel::uploadfileblockrq(SOCKET sock, char *szbuf)
{
    //将文件内容写入到对应文件
    STRU_UPLOADFILEBLOCK_RQ *psur=(STRU_UPLOADFILEBLOCK_RQ*)szbuf;
    UploadFileInfo*p=m_mapFileIdtoFileInfo[psur->m_fileId];
    if(!p)
    {
        std::cout<<"TCPKernel::uploadfileblockrq的 UploadFileInfo 为空"<<endl;
            return;
    }
    //将文件信息写入到对应的文件中
    //printf("m_pos:%d  filesize:%d\n",p->m_pos,p->m_fileSize);
    ssize_t nWritNum=fwrite(psur->m_szFileContent,sizeof(char),psur->m_fileNum,p->m_pfile);
    if(nWritNum>0)
    {
        p->m_pos+=nWritNum;
        if(p->m_pos==p->m_fileSize)
        {
            fclose(p->m_pfile);
            auto ite=m_mapFileIdtoFileInfo.begin();
            while(ite!=m_mapFileIdtoFileInfo.end())
            {
                if(ite->first==psur->m_fileId)
                {
                    auto p=ite->second;
                    delete p;
                    p=nullptr;
                    m_mapFileIdtoFileInfo.erase(ite);
                    break;
                }
                ite++;
            }
        }
    }

}

void TCPKernel::sharelinkrq(SOCKET sock, char *szbuf)
{
    STRU_SHARELINK_RQ *pssr=(STRU_SHARELINK_RQ*)szbuf;
    STRU_SHARELLINK_RS ssr;
    strcpy(ssr.m_szFileName,pssr->m_szFileName);
    //生成码4位数字
    srand((unsigned int)time(0));
    char c;

    char szsql[SQLLEN]={0};
    list<string> lststr;
    char szCode[5]={0};
    while(1)
    {
        for (int i = 0; i < 4; i++) {
            int randNum = rand() % 36;
            if (randNum < 10){
                c = randNum + '0'; // 生成数字 0-9 对应的 ASCII 字符
            }
            else {
                c = randNum - 10 + 'A'; // 生成字母 A-Z 对应的 ASCII 字符
            }
            szCode[i]=c;
        }

        sprintf(szsql,"select num from 0113disk.user_share where code = '%s'",szCode);
        m_pSQL->SelectMySql(szsql,1,lststr);
        if(lststr.empty())
        {
        break;
        }
        else
        {
        lststr.pop_back();
        }
    }


    sprintf(szsql,"select f_id,f_path from myview where u_id=%lld and f_name ='%s'",pssr->m_userId,pssr->m_szFileName);
    m_pSQL->SelectMySql(szsql,2,lststr);
    if(lststr.size()>0)
    {
        string strFileId=lststr.front();
        lststr.pop_front();
        string f_path=lststr.front();
        lststr.pop_front();
        RedisManager& redis=RedisManager::instance();
        !redis.storeFileInfoJson(strFileId,f_path,szCode);
        long long FileId=atoll(strFileId.c_str());
            //将分享的文件信息存储到数据库中
        sprintf(szsql,"insert into user_share(u_id,f_id,code) values(%lld,%lld,'%s')",pssr->m_userId,FileId,szCode);
        m_pSQL->UpdateMySql(szsql);
    }
    //发回复
    strcpy(ssr.m_szCode,szCode);
    m_pTCPNet->sendData(sock,(char*)&ssr,sizeof(ssr));
}

void TCPKernel::breakrq(SOCKET sock, char *szbuf)
{
    STRU_BREAK_RS sbs;
    m_pTCPNet->sendData(sock,(char*)&sbs,sizeof(sbs));
    if(sock){
        closesocket(sock);
    }
}

void TCPKernel::heartbeatrq(SOCKET sock)
{
    m_mapSocktoQDateTime[sock]=QDateTime::currentDateTime().addSecs(_heartbeattime+2);
}

void TCPKernel::searchfilerq(SOCKET sock, char *szbuf)
{
    STRU_SEARCHFILE_RQ *sfq=(STRU_SEARCHFILE_RQ *)szbuf;
    STRU_SEARCHFILE_RS sfs;
    char szsql[SQLLEN]={0};
    list<string> lststr;
    sfs.result=false;
    sprintf(szsql, "SELECT file.f_name, file.f_size, file.f_uploadtime "
                   "FROM file, user_file "
                   "WHERE file.f_id = user_file.f_id "
                   "AND user_file.u_id = %d "
                   "AND file.f_name LIKE CONCAT('%%', '%s', '%%');",
            sfq->m_userId, sfq->filename);

    m_pSQL->SelectMySql(szsql,3,lststr);
    if(!lststr.empty()){
        sfs.result=true;
        sfs.m_FileNum=lststr.size()/3;
        int i=0;
        while(!lststr.empty()){
            string filename=lststr.front();
            lststr.pop_front();
            string filesize=lststr.front();
            lststr.pop_front();
            string filedate=lststr.front();
            lststr.pop_front();
            strcpy(sfs.m_aryInfo[i].m_szFileName,filename.c_str());
            strcpy(sfs.m_aryInfo[i].m_szFileDateTime,filedate.c_str());
            sfs.m_aryInfo[i].m_fileSize=atoll(filesize.c_str());
            i++;
        }
    }
    m_pTCPNet->sendData(sock,(char*)&sfs,sizeof(sfs));
}

std::wstring utf8ToWide(const char* utf8Str) {
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len];
    MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, wstr, len);
    std::wstring result(wstr);
    delete[] wstr;
    return result;
}

// 获取文件大小
long getFileSize(FILE* file) {
    if (!file) return -1;

    // 保存当前位置
    long currentPos = ftell(file);

    // 移动到文件末尾
    fseek(file, 0, SEEK_END);
    long size = ftell(file);

    // 恢复原位置
    fseek(file, currentPos, SEEK_SET);

    return size;
}


void download(INet* pTCPNet,string fpath,SOCKET sock,int fid,string downloadpath)
{
    STRU_DOWNLOADFILE_RS sds;
    strcpy(sds.downloadpath,downloadpath.c_str());
    sds.fileid=fid;
    QString qfpath=QString::fromUtf8(fpath.c_str());
    QString fileName =qfpath.section('/',-1);
    strcpy(sds.filename,fileName.toStdString().c_str());
    FILE* pfile = _wfopen(utf8ToWide(fpath.c_str()).c_str(), L"rb");
    if(!pfile)
    {
        qDebug()<<"无法打开文件";
        return;
    }
    int filesize=getFileSize(pfile);
    printf("filesize:%d\n",filesize);
    int totalread=0;
    sds.totalfilesize=filesize;
    while(totalread<filesize)
    {
        int read=fread(sds.m_filecontent,1,sizeof(sds.m_filecontent),pfile);
        if(read>0)
        {
            totalread+=read;
            qDebug()<<"读取字节数："<<read;
            //printf("读取%d字节\n",read);
            sds.pagesize=read;
            pTCPNet->sendData(sock,(char*)&sds,sizeof(sds));
        }
    }
    fclose(pfile);
    //printf("文件读取完毕\n");
    qDebug()<<"读取完毕";
}
void TCPKernel::downloadfile(SOCKET sock, char *szbuf)
{
    STRU_DOWNLOADFILE_RQ *sdq=(STRU_DOWNLOADFILE_RQ *)szbuf;
    char szsql[SQLLEN]={};
    sprintf(szsql,"select 0113disk.file.f_id,f_path from 0113disk.file join 0113disk.user_file on 0113disk.file.f_id=0113disk.user_file.f_id and 0113disk.user_file.u_id=%d and f_name='%s';",sdq->m_userid,sdq->filename);
    list<string> lst;
    m_pSQL->SelectMySql(szsql,2,lst);
    int fid=std::stoi(lst.front());
    lst.pop_front();
    string fpath=lst.front();
    //int fsize=sdq->file_size;
    string strdownloadpath=sdq->downloadpath;
    std::thread downloadthread(download,m_pTCPNet,fpath,sock,fid,strdownloadpath);
    downloadthread.detach();

}

int min(int a,int b)
{
    return a<b?a:b;
}



void downloadbyjson(INet* pTCPNet, string fpath, SOCKET sock, int fid, string downloadpath)
{
    // 第一步：发送HEAD
    json head = json::object();
    head["type"] = _default_protocol_getlink_head_rs;
    head["fileId"] = fid;
    head["downloadPath"] = downloadpath;
    QString qfpath = QString::fromUtf8(fpath.c_str());
    QString fileName = qfpath.section('/', -1);
    head["fileName"] = fileName.toStdString();

    FILE* pfile = _wfopen(utf8ToWide(fpath.c_str()).c_str(), L"rb");
    if (!pfile) {
        LOG_ERROR("downloadbyjson open file fail! fpath:{}",fpath);
        return;
    }

    size_t filesize = getFileSize(pfile);
    head["totalFileSize"] = filesize;

    // 发送HEAD
    string headstr = head.dump();
    LOG_INFO("downloadbyjson: head:{}",headstr.c_str());
    pTCPNet->sendData(sock, headstr.c_str(), headstr.size());

    // 第二步：发送BODY（分块）
    const size_t CHUNK_SIZE = 4 * 1024; // 4KB
    std::vector<uint8_t> buffer(CHUNK_SIZE);
    size_t totalread = 0;
    int chunkId = 0;

    while (totalread < filesize) {
        size_t currentChunkSize = min(CHUNK_SIZE, filesize - totalread);
        buffer.resize(currentChunkSize);
        size_t read = fread(buffer.data(), 1, currentChunkSize, pfile);
        if (read > 0) {
            json body = json::object();
            body["type"] = _default_protocol_getlink_body_rs;
            body["fileId"] = fid;
            body["offset"] = totalread;
            body["chunkSize"] = read;
            body["chunkId"] = chunkId;
            body["fileContent"] = base64_encode(buffer.data(), read);

            string bodystr = body.dump();
            pTCPNet->sendData(sock, bodystr.c_str(), bodystr.size());

            totalread += read;
            chunkId++;
            qDebug() << "发送分块: ID=" << chunkId << "大小=" << read << "偏移=" << totalread - read;
            LOG_INFO("downloadbyjson:body 发送分块id={}  大小=%{}  偏移={}",chunkId,read,totalread - read);
        }
    }
    fclose(pfile);

    // 第三步：发送TAIL
    json tail = json::object();
    tail["type"] = _default_protocol_getlink_tail_rs;
    tail["fileId"] = fid;
    string tailstr = tail.dump();
    pTCPNet->sendData(sock, tailstr.c_str(), tailstr.size());
    LOG_INFO("downloadbyjson:tail 文件id={}",fid);
    qDebug() << "文件传输完成";
}
//void TCPKernel::getlinkfile(SOCKET sock, json js)
//{
//    char szsql[SQLLEN]={};
//    if(!js["link"].is_string())
//    {
//        LOG_ERROR("js[\"link\"] is not a string");
//        return;
//    }
//    sprintf(szsql,"select f_id,f_path from 0113disk.file where f_id=(select f_id from 0113disk.user_share where 0113disk.user_share.code='%s');",js["link"].get<std::string>().c_str());
//    list<string> lst;
//    m_pSQL->SelectMySql(szsql,2,lst);
//    if(lst.empty())
//    {
//        LOG_ERROR("无法找到连接文件！");
//        return;
//    }
//    int fid=stoi(lst.front());
//    lst.pop_front();
//    string fpath=lst.front();
//    lst.pop_front();
//    string downloadpath=js["path"].get<std::string>();
//    std::thread downloadthread(downloadbyjson,m_pTCPNet,fpath,sock,fid,downloadpath);
//    downloadthread.detach();
//}

void TCPKernel::getlinkfile(SOCKET sock, json js)
{
    if(!js["link"].is_string())
    {
        LOG_ERROR("js[\"link\"] is not a string");
        return;
    }
    string link=js["link"].get<std::string>();
    json res=json::object();
    RedisManager& redis=RedisManager::instance();

    if(!redis.getFileInfoJson(link,res))
    {
        LOG_ERROR("TCPKernel::getlinkfile  getFileInfoJson fail");
        char szsql[SQLLEN]={};
        if(!js["link"].is_string())
        {
            LOG_ERROR("js[\"link\"] is not a string");
            return;
        }
        sprintf(szsql,"select f_id,f_path from 0113disk.file where f_id=(select f_id from 0113disk.user_share where 0113disk.user_share.code='%s');",js["link"].get<std::string>().c_str());
        list<string> lst;
        m_pSQL->SelectMySql(szsql,2,lst);
        if(lst.empty())
        {
            LOG_ERROR("无法找到连接文件！");
            return;
        }
        int fid=stoi(lst.front());
        lst.pop_front();
        string fpath=lst.front();
        lst.pop_front();
        string downloadpath=js["path"].get<std::string>();
        std::thread downloadthread(downloadbyjson,m_pTCPNet,fpath,sock,fid,downloadpath);
        downloadthread.detach();
        return;
    }
    LOG_INFO("TCPKernel::getlinkfile getFileInfoJson success");
    LOG_INFO("TCPKernel::getlinkfile json:%s",res.dump());
    string downloadpath=js["path"].get<std::string>();
    string fpath=res["f_path"];
    int fid=std::stoi(res["f_id"].get<std::string>());
    std::thread downloadthread(downloadbyjson,m_pTCPNet,fpath,sock,fid,downloadpath);
    downloadthread.detach();
}




