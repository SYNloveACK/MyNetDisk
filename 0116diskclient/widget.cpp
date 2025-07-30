#include "widget.h"
#include "ui_widget.h"
#include<QIcon>
#include<QFile>
#include<QFileDialog>
#include<QInputDialog>
#include<thread>

std::wstring utf8ToWide(const char* utf8Str) {
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len];
    MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, wstr, len);
    std::wstring result(wstr);
    delete[] wstr;
    return result;
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("disk");
    setWindowIcon(QIcon("./disk.JPG"));
    m_pKernel=new TCPKernel;
    m_plogin=new login;
    m_plogin->setKernel(m_pKernel);
    m_plogin->show();

    if(m_pKernel->connect())
    {
        qDebug()<<"connect success";
    }
    else
    {
        QMessageBox::information(this,"err","connect err");
    }
    m_FileNum=0;
    psf=new SearchFile;
    psf->setKernel(m_pKernel);
    showsf=new ShowSearchFile;
    //创建线程池
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    //m_tp.createThreadpool(2,si.dwNumberOfProcessors/(1-0.8));
    m_tp.createThreadpool(5,8);
    connect((TCPKernel*)m_pKernel,&TCPKernel::signal_register,m_plogin,&login::slot_register,Qt::BlockingQueuedConnection);
    connect((TCPKernel*)m_pKernel,&TCPKernel::signal_login,this,&Widget::slot_login,Qt::BlockingQueuedConnection);
    connect((TCPKernel*)m_pKernel,&TCPKernel::signal_getfilelist,this,&Widget::slot_getfilelist,Qt::BlockingQueuedConnection);
    connect((TCPKernel*)m_pKernel,&TCPKernel::signal_uploadfileinfo,this,&Widget::slot_uploadfileinfo,Qt::BlockingQueuedConnection);
    connect((TCPKernel*)m_pKernel,&TCPKernel::signal_sharelink,this,&Widget::slot_sharelink,Qt::BlockingQueuedConnection);
    connect((TCPKernel*)m_pKernel,&TCPKernel::signal_break,this,&Widget::slot_break,Qt::BlockingQueuedConnection);
    connect((TCPKernel*)m_pKernel,&TCPKernel::signal_searchfile,showsf,&ShowSearchFile::slot_showsearchfile,Qt::BlockingQueuedConnection);
    connect((TCPKernel*)m_pKernel,&TCPKernel::signal_downlowad,this,&Widget::slot_download,Qt::BlockingQueuedConnection);

}

Widget::~Widget()
{
    delete ui;
    delete m_pKernel;
    delete m_plogin;
    m_tp.destroyThreadpool();
    delete psf;
    delete showsf;
}

void Widget::heartbeat(TCPKernel* p)
{
    STRU_HEARTBEAT_RQ shq;
    while(1){
        p->sendData((char*)&shq,sizeof(shq));
        this_thread::sleep_for(std::chrono::seconds(_heartbeattime));
    }
}

void Widget::slot_login(STRU_LOGIN_RS *pslr)
{
    const char* pszResult="user noexist";
    if(pslr->m_szResult==_login_passwderr)
    {
        pszResult="password err";
    }
    else if(pslr->m_szResult==_login_success)
    {
        //关闭登录窗口
        m_plogin->hide();
        //显示主窗口
        this->show();
        m_userId=pslr->m_userId;
        pszResult="login success";
        //登录成功
        //获取文件列表的请求
        STRU_GETFILELIST_RQ sgr;
        sgr.m_userId=pslr->m_userId;
        m_pKernel->sendData((char*)&sgr,sizeof(sgr));

    }
    QMessageBox::information(this,"login",pszResult);
    std::thread threadheart(heartbeat,m_pKernel);
    threadheart.detach();

    return;
}


void Widget::slot_getfilelist(STRU_GETFILELIST_RS *psgr)
{
    m_FileNum=psgr->m_FileNum;
    QTableWidgetItem *pitem;
    for(int i=0;i<psgr->m_FileNum;i++)
    {
        pitem=new QTableWidgetItem(QIcon("./pic.JPG"),psgr->m_aryInfo[i].m_szFileName);
        ui->tableWidget->setItem(i,0,pitem);
        QString strSize=QString::number(psgr->m_aryInfo[i].m_fileSize);
        pitem=new QTableWidgetItem(strSize);
        ui->tableWidget->setItem(i,1,pitem);
        pitem=new QTableWidgetItem(psgr->m_aryInfo[i].m_szFileDateTime);
        ui->tableWidget->setItem(i,2,pitem);
    }
}

void SendFileItask::run()
{
    //FILE *pfile=fopen(m_pInfo->m_szFilePath,"rb");


    // 转换UTF-8路径为宽字符
    // 使用_wfopen打开文件
    FILE* pfile = _wfopen(utf8ToWide(m_pInfo->m_szFilePath).c_str(), L"rb");

    if (!pfile) {
        qDebug() << "Failed to open file:" << m_pInfo->m_szFilePath;
        return; // 直接返回或处理错误
    }



    //如果是断点续传
    if(m_pos)
    {
        //将文件指针移动到指定位置
        fseek(pfile,m_pos,SEEK_SET);
    }
    //正常发送文件内容
    STRU_UPLOADFILEBLOCK_RQ sur;
    sur.m_fileId=m_fileId;
    while(1)
    {
        ssize_t nReadNum=fread(sur.m_szFileContent,sizeof(char),sizeof(sur.m_szFileContent),pfile);
        if(nReadNum>0)
        {
            //qDebug()<<"nReadNum="<<nReadNum<<endl;
            sur.m_fileNum=nReadNum;
            m_pWidget->m_pKernel->sendData((char*)&sur,sizeof(sur));
        }
        else
        {
            //qDebug()<<"break:nReadNum="<<nReadNum<<endl;
            break;
        }
    }
    fclose(pfile);
    QString strFilePath=m_pInfo->m_szFilePath;
    strFilePath=strFilePath.section('/',-1);
    auto ite=m_pWidget->m_mapFileNametoFileInfo.begin();
    while(ite!=m_pWidget->m_mapFileNametoFileInfo.end())
    {
        if(ite->first==strFilePath)
        {
            ite =m_pWidget->m_mapFileNametoFileInfo.erase(ite);
            break;
        }
        ite++;
    }
}

void Widget::slot_uploadfileinfo(STRU_UPLOADFILEINFO_RS *psur)
{
    QString strFileName=psur->m_szFileName;
    UploadFileInfo* pInfo=m_mapFileNametoFileInfo[strFileName];

    if(psur->m_szResult==_uploadinfo_isuploaded)
    {
        QMessageBox::information(this,"uploadfile","file is uploaded");
    }
    else if(_uploadinfo_speedtranfer==psur->m_szResult)
    {
        qDebug()<<"speedtransfer\n";
    }
    else if(psur->m_szResult==_uploadinfo_continue||psur->m_szResult==_uploadinfo_normal)
    {
        //将任务投递到线程池
        task *p=new SendFileItask(pInfo,psur->m_pos,psur->m_fileId,this );
        m_tp.push(p);
    }

    //1将文件信息显示到控件上
    QTableWidgetItem* p=new QTableWidgetItem(QIcon("./pic.JPG"),strFileName);
    ui->tableWidget->setItem(m_FileNum,0,p);
    QString strFileSize=QString::number(pInfo->m_fileSize);
    p=new QTableWidgetItem(strFileSize);
    ui->tableWidget->setItem(m_FileNum,1,p);
    QDateTime datetime = QDateTime::currentDateTime();
    QString strDateTime = datetime.toString("dd.MM.yyyy HH.mm.ss");
    p = new QTableWidgetItem(strDateTime);
    ui->tableWidget->setItem(m_FileNum,2,p);
    ++m_FileNum;
    //2显示文件上传成功
    QMessageBox::information(this,"uploadfile","file upload success");
}

void Widget::slot_sharelink(STRU_SHARELLINK_RS *pssr)
{
    QString strLink=pssr->m_szLink;
    strLink=strLink+"code: "+pssr->m_szCode;
   // QMessageBox::information(this,"share link",strLink);
    QInputDialog::getText(this,tr("share link"),
                          tr("share link address"),QLineEdit::Normal,
                          strLink);
}

void Widget::slot_break(STRU_BREAK_RS *)
{
    QMessageBox::information(this,"标题","您已下线");
}

void Widget::slot_download(STRU_DOWNLOADFILE_RS* psds) {
    // 1. 规范化路径处理
    std::string downloadpath = psds->downloadpath;
    if (!downloadpath.empty() && downloadpath.back() != '/' && downloadpath.back() != '\\') {
        downloadpath += '/';
    }
    downloadpath += psds->filename;


    int fid = psds->fileid;

    // 2. 检查是否已有下载信息
    auto it = m_mapFidtoDownloadfileInfo.find(fid);
    if (it == m_mapFidtoDownloadfileInfo.end()) {
        // 新文件下载
        DownloadFileInfo* pinfo = new DownloadFileInfo;
        pinfo->m_pos = 0;
        pinfo->m_filesize = psds->totalfilesize;
        pinfo->downloadpath = downloadpath;

        // 3. 总是以写入二进制模式创建新文件
        FILE* file = _wfopen(utf8ToWide(downloadpath.c_str()).c_str(), L"wb");
        if (!file) {
            qDebug() << "创建文件失败:" << QString::fromStdString(downloadpath);
            delete pinfo;
            return;
        }

        pinfo->pfile = file;
        m_mapFidtoDownloadfileInfo.emplace(fid, pinfo);
    }

    // 4. 获取下载信息
    DownloadFileInfo* pinfo = m_mapFidtoDownloadfileInfo[fid];
    FILE* pfile = pinfo->pfile;

    // 5. 写入数据前定位到正确位置
    if (_fseeki64(pfile, pinfo->m_pos, SEEK_SET) != 0) {
        qDebug() << "文件定位失败";
        return;
    }

    // 6. 写入数据
    int remaining = psds->pagesize;
    int writenum = 0;
    const char* data = psds->m_filecontent;

    while (remaining > 0) {
        int written = fwrite(data + writenum, 1, remaining, pfile);
        if (written <= 0) {
            qDebug() << "写入失败，错误码:" << ferror(pfile);
            fflush(pfile);
            break;
        }

        writenum += written;
        remaining -= written;
        pinfo->m_pos += written;

        // 定期刷新缓冲区
        if (writenum % 4096 == 0) {
            fflush(pfile);
        }
    }

    // 7. 检查是否完成
    if (pinfo->m_pos >= pinfo->m_filesize) {
        qDebug() << "传输完成:" << QString::fromStdString(downloadpath);
        fclose(pfile);
        delete pinfo;
        m_mapFidtoDownloadfileInfo.erase(fid);
    } else {
        fflush(pfile); // 确保数据写入磁盘
    }
}

string FileDigest(QString filename) {

    QFile file(filename);

    file.open(QIODevice::ReadOnly);
    if (!file.isOpen())
        return "";

    MD5 md5;
    char buffer[1024];
    while (1)
    {
        qint64 length =  file.read(buffer, 1024);

        if (length > 0)
            md5.update(buffer, length);
        else
            break;
    }
    file.close();
    return md5.toString();
}

void Widget::on_pushButton_clicked()//上传文件
{
    //获取上传文件信息（文件名，大小，MD5）
    QString filePath=QFileDialog::getOpenFileName(this,tr("Open file"),
                                                  ".",//打开文件的根目录
                                                    "ALL files (*.*);;Images (*.png *.xpm *.jpg);;Text files(*.txt))");
    QString fileName =filePath.section('/',-1);

    QFile file(filePath);
    long long filesize;
    if(file.open(QIODevice::ReadOnly))
    {
        filesize=file.size();
        file.close();
    }
    else
    {
        printf("file open fail!");
    }
    string strMD5 =FileDigest(filePath);
    //qDebug()<<strMD5.c_str();
    //上传文件信息请求表
    STRU_UPLOADFILEINFO_RQ sur;
    sur.m_filesize=filesize;
    sur.m_userid=m_userId;
    strcpy(sur.m_szFileMD5,strMD5.c_str());
    strcpy(sur.m_szFileName,fileName.toStdString().c_str());

    m_pKernel->sendData((char*)&sur,sizeof(sur));
    //记录当前文件信息 文件名（文件路径、大小 id
    UploadFileInfo* p=new UploadFileInfo;
    strcpy(p->m_szFilePath,filePath.toStdString().c_str());
    p->m_fileId=0;
    p->m_fileSize=filesize;
    m_mapFileNametoFileInfo[fileName]=p;

}



void Widget::on_pushButton_5_clicked()
{
    //获取分享文件
    int nRow=ui->tableWidget->currentRow();
    if(nRow==-1) return;
    QTableWidgetItem *pItem=ui->tableWidget->item(nRow,0);

    //发送分享链接请求
    STRU_SHARELINK_RQ ssr;
    ssr.m_userId=m_userId;
    strcpy(ssr.m_szFileName,pItem->text().toStdString().c_str());

    m_pKernel->sendData((char*)&ssr,sizeof(ssr));
}


void Widget::on_pushButton_3_clicked()
{
    psf->userid=this->m_userId;
    psf->show();
}


void Widget::on_pushButton_11_clicked()
{
    STRU_BREAK_RQ sbq;
    m_pKernel->sendData((char*)&sbq,sizeof(sbq));
}


void Widget::on_tableWidget_cellClicked(int row, int column)
{
QMessageBox msgBox;
msgBox.setWindowTitle("提示");
msgBox.setText("选择你的操作");

// 添加自定义按钮（文本和角色）
QPushButton *saveBtn = msgBox.addButton("下载", QMessageBox::AcceptRole);
QPushButton *discardBtn = msgBox.addButton("删除", QMessageBox::RejectRole);
QPushButton *cancelBtn = msgBox.addButton("取消", QMessageBox::DestructiveRole);

msgBox.exec();

if (msgBox.clickedButton() == saveBtn) {
    QString dir = QFileDialog::getExistingDirectory(
        nullptr,                 // 父窗口
        "选择下载目录",               // 对话框标题
        QDir::homePath(),        // 默认打开的目录（这里是用户主目录）
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
    string sdir=dir.toUtf8().toStdString();

    STRU_DOWNLOADFILE_RQ sdq;
    strcpy(sdq.downloadpath,sdir.c_str());
    sdq.m_userid=m_userId;
    QTableWidgetItem* item=ui->tableWidget->item(row,0);

    QString fname=item?item->text():"";
    item=ui->tableWidget->item(row,1);
    QString fsize=item?item->text():"";
    sdq.file_size=std::stoll(fsize.toStdString());
    strcpy(sdq.filename,fname.toStdString().c_str());
    m_pKernel->sendData((char*)&sdq,sizeof(sdq));

} else if (msgBox.clickedButton() == discardBtn) {
    qDebug() << "用户点击了删除";
} else if (msgBox.clickedButton() == cancelBtn) {
    qDebug() << "用户点击了取消";
}
}

