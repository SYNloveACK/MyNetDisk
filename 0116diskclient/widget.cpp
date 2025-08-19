#include "widget.h"
#include "ui_widget.h"
#include<QIcon>
#include<QFile>
#include<QFileDialog>
#include<QInputDialog>
#include<thread>
#include<QUiLoader>
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
    resize(1400,1000);
    this->setLayout(ui->horizontalLayout);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
    connect((TCPKernel*)m_pKernel,&TCPKernel::signal_downloadbyjson,this,&Widget::slot_downloadbyjson,Qt::BlockingQueuedConnection);

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

void Widget::resizeEvent(QResizeEvent *event)
{

    //QWidget::resizeEvent(event);
    //ui->horizontalLayout->setGeometry(rect());
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


std::wstring utf8ToWide(const std::string& utf8Str) {
    if (utf8Str.empty()) {
        return L"";
    }

    // 计算需要的缓冲区大小
    int wideCharCount = MultiByteToWideChar(
        CP_UTF8,            // 源字符串是 UTF-8
        0,                 // 无特殊标志
        utf8Str.c_str(),    // 源字符串
        -1,                // 自动计算长度（包括 NULL 终止符）
        nullptr,           // 不接收输出，仅计算长度
        0                  // 输出缓冲区大小为 0，仅计算
        );

    if (wideCharCount == 0) {
        return L"";
    }

    // 分配足够大的缓冲区
    std::wstring wideStr;
    wideStr.resize(wideCharCount);

    // 执行转换
    int result = MultiByteToWideChar(
        CP_UTF8,
        0,
        utf8Str.c_str(),
        -1,
        &wideStr[0],       // 直接写入 std::wstring 的缓冲区
        wideCharCount
        );

    if (result == 0) {
        return L"";
    }

    // 移除多余的 NULL 终止符（因为 std::wstring 会自动处理）
    if (!wideStr.empty() && wideStr.back() == L'\0') {
        wideStr.pop_back();
    }

    return wideStr;
}


// 处理HEAD消息
void  Widget::handleHeadMessage(json &js, int fid) {
    // 1. 必需字段校验
    if (!js.contains("fileName") || !js.contains("totalFileSize") || !js.contains("downloadPath")) {
        qDebug() << "HEAD消息缺少必要字段";
        LOG_ERROR("handleHeadMessage:HEAD消息缺少必要字段");
        return;
    }

    // 2. 路径处理
    std::string downloadpath = js["downloadPath"].get<std::string>();
    if (!downloadpath.empty() && downloadpath.back() != '/' && downloadpath.back() != '\\') {
        downloadpath += '/';
    }
    downloadpath += js["fileName"].get<std::string>();
    // 3. 检查是否已存在相同文件ID
    if (m_mapFidtoDownloadfileInfoJson.find(fid) != m_mapFidtoDownloadfileInfoJson.end()) {
        qDebug() << "文件ID已存在，覆盖之前的下载:" << fid;
        LOG_INFO("handleHeadMessage:文件ID已存在，覆盖之前的下载");
        delete m_mapFidtoDownloadfileInfoJson[fid];
    }

    // 4. 创建新的下载信息
    DownloadFileInfoJson* pinfo = new DownloadFileInfoJson;
    pinfo->m_filesize = js["totalFileSize"].get<size_t>();
    pinfo->downloadpath = downloadpath;
    pinfo->receivedSize = 0;  // 初始化接收字节数

    // 5. 创建文件并预分配空间
    FILE* f = _wfopen(utf8ToWide(downloadpath).c_str(), L"wb");
    if (f) {
        if (pinfo->m_filesize > 0) {
            // 设置文件大小
            if (_chsize_s(_fileno(f), static_cast<long>(pinfo->m_filesize)) != 0) {
                LOG_ERROR("文件空间预分配失败");
            }
        }
        fclose(f);
    }

    // 6. 以读写模式重新打开文件
    pinfo->pfile = _wfopen(utf8ToWide(downloadpath).c_str(), L"r+b");
    if (!pinfo->pfile) {
        LOG_ERROR("文件创建失败");
        delete pinfo;
        return;
    }

    // 7. 保存下载信息
    m_mapFidtoDownloadfileInfoJson[fid] = pinfo;

    LOG_INFO("handleHeadMessage:文件初始化完成，大小{}",pinfo->m_filesize);
}

// 处理BODY消息
void  Widget::handleBodyMessage(json &js, int fid) {
    // 1. 必需字段校验
    if (!js.contains("offset") || !js.contains("chunkSize") || !js.contains("fileContent")) {
        LOG_ERROR("handleBodyMessage:BODY消息缺少必要字段");
        return;
    }

    // 2. 获取下载信息
    auto it = m_mapFidtoDownloadfileInfoJson.find(fid);
    if (it == m_mapFidtoDownloadfileInfoJson.end() || !it->second) {
        LOG_ERROR("handleBodyMessage:未找到文件ID对应的下载信息");
        return;
    }
    DownloadFileInfoJson* pinfo = it->second;

    // 3. 获取分块信息
    size_t offset = js["offset"].get<size_t>();
    size_t chunkSize = js["chunkSize"].get<size_t>();

    // 4. 处理Base64编码数据
    std::vector<uint8_t> fileData;
    try {
        std::string base64Str = js["fileContent"].get<std::string>();
        QByteArray decoded = QByteArray::fromBase64(base64Str.c_str());

        // 检查解码后数据大小是否匹配
        if (static_cast<size_t>(decoded.size()) != chunkSize) {
            LOG_ERROR("handleBodyMessage:分块大小不匹配, 预期:{} 实际:{}",chunkSize,decoded.size());
            return;
        }

        fileData.assign(decoded.begin(), decoded.end());
    } catch (const json::exception& e) {
        LOG_ERROR("handleBodyMessage:JSON解析错误");
        return;
    }

    // 5. 定位文件指针
    if (_fseeki64(pinfo->pfile, static_cast<long long>(offset), SEEK_SET) != 0) {
        LOG_ERROR("handleBodyMessage:文件定位失败, 偏移:",offset);
        return;
    }

    // 6. 写入数据
    size_t written = fwrite(fileData.data(), 1, fileData.size(), pinfo->pfile);
    if (written != fileData.size()) {
        LOG_ERROR("handleBodyMessage:写入不完整, 预期:{} 实际{}",fileData.size(),written);
        return;
    }

    // 7. 更新状态
    pinfo->receivedSize += written;
    fflush(pinfo->pfile);  // 确保数据写入磁盘

    // 8. 输出进度信息
    if (js.contains("chunkId")) {
        LOG_INFO("handleBodyMessage:接收分块ID={} 大小={} 进度={}/{}", js["chunkId"].get<int>(),written,pinfo->receivedSize,pinfo->m_filesize);
    }
}

// 处理TAIL消息
void  Widget::handleTailMessage(int fid) {
    // 1. 获取下载信息
    auto it = m_mapFidtoDownloadfileInfoJson.find(fid);
    if (it == m_mapFidtoDownloadfileInfoJson.end() || !it->second) {
        LOG_ERROR("handleTailMessage:未找到文件ID对应的下载信息id={}",fid);
        return;
    }
    DownloadFileInfoJson* pinfo = it->second;
    // 2. 验证文件完整性
    if (pinfo->receivedSize != pinfo->m_filesize) {
        LOG_ERROR("handleTailMessage:文件传输不完整");
    } else {
        LOG_INFO("handleTailMessage:文件传输完成");
    }

    // 3. 清理资源
    if (pinfo->pfile) {
        fclose(pinfo->pfile);
        pinfo->pfile = nullptr;
    }
    delete pinfo;
    m_mapFidtoDownloadfileInfoJson.erase(fid);
}


void Widget::slot_downloadbyjson(json &js) {
    std::lock_guard<std::mutex> lock(file_mtx); // 文件操作锁

    // 1. 必需字段校验
    if (!js.contains("type") || !js.contains("fileId")) {
        LOG_ERROR("slot_downloadbyjson:无效的JSON格式");
        return;
    }

    // 2. 获取消息类型和文件ID
    int type = js["type"].get<int>();
    int fid = js["fileId"].get<int>();

    // 3. 根据消息类型处理
    if (type == _default_protocol_getlink_head_rs) {
        handleHeadMessage(js, fid);
    } else if (type == _default_protocol_getlink_body_rs) {
        handleBodyMessage(js, fid);
    } else if (type == _default_protocol_getlink_tail_rs) {
        handleTailMessage(fid);
    } else {
        LOG_ERROR("slot_downloadbyjson 未知的消息类型");
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
    if(filePath.isEmpty())
    {
        return;
    }
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



void Widget::on_pushButton_5_clicked()//发送分享链接
{
    //获取分享文件
    int nRow=ui->tableWidget->currentRow();
    int totalCount=ui->tableWidget->rowCount();
    if(nRow==-1||nRow>=totalCount)
    {
        return;
    }
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

void Widget::on_tableWidget_cellDoubleClicked(int row, int column)
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
        if(dir.isEmpty())
        {
            return;
        }
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


void Widget::on_pushButton_pickLink_clicked()
{
    // 1. 创建 UI 加载器
    QUiLoader loader;

    // 2. 打开 UI 文件
    QFile uiFile(":/pickLink.ui"); // 如果放在资源文件中
    if (!uiFile.open(QIODevice::ReadOnly)) {
        qDebug() << "open file fail！";
        return;
    }

    // 3. 加载 UI 文件并生成界面
    QWidget *pickLinckWindow = loader.load(&uiFile);
    uiFile.close();

    if (!pickLinckWindow) {
        qDebug() << "load ui file fail！";
        return;
    }

    QPushButton *btnConfirm = pickLinckWindow->findChild<QPushButton*>("pushButton_Confirm");
    QPushButton *btnCancel = pickLinckWindow->findChild<QPushButton*>("pushButton_Cancle");
    QLineEdit *lineEdit = pickLinckWindow->findChild<QLineEdit*>("lineEdit");

    if (btnConfirm && btnCancel && lineEdit) {
        // 确认按钮点击
        connect(btnConfirm, &QPushButton::clicked, [lineEdit,this,pickLinckWindow]() {
            json js=json::object();
            js["type"]=_default_protocol_getlink_rq;
            js["uid"]=std::to_string(this->m_userId);
            js["link"]=lineEdit->text().toStdString();;
            QString dir = QFileDialog::getExistingDirectory(
                nullptr,                 // 父窗口
                "选择下载目录",               // 对话框标题
                QDir::homePath(),        // 默认打开的目录（这里是用户主目录）
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                );
            if(dir.isEmpty())
            {

                return;
            }
            string sdir=dir.toUtf8().toStdString();
            js["path"]=sdir;
            string jsstr=js.dump();
            this->m_pKernel->sendData(jsstr.c_str(),jsstr.size()+1);
            pickLinckWindow->close();
        });

        // 取消按钮点击
        connect(btnCancel, &QPushButton::clicked,pickLinckWindow,&Widget::close);
    }
    else
    {
        qDebug() << "else:cant find !";
    }
    // 4. 显示窗口
    pickLinckWindow->show();

}

