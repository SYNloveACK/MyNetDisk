#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include"./kernel/tcpkernel.h"
#include<QDebug>
#include<QMessageBox>
#include"login.h"
#include"md5.h"
#include<map>
#include<QDateTime>
#include<QTime>
#include<threadpool.h>
#include"searchfile.h"
#include"showsearchfile.h"
#include <windows.h>
struct UploadFileInfo
{
    char m_szFilePath[MAX_PATH];
    long long m_fileSize;
    long long m_fileId;
};
struct DownloadFileInfo
{
    string downloadpath;
    int m_pos;
    int m_filesize;
    FILE* pfile;
};

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    static void heartbeat(TCPKernel* p);
public slots:
    void slot_login(STRU_LOGIN_RS*);
    void slot_getfilelist(STRU_GETFILELIST_RS* );
    void slot_uploadfileinfo(STRU_UPLOADFILEINFO_RS*);
    void slot_sharelink(STRU_SHARELLINK_RS*);
    void slot_break(STRU_BREAK_RS*);
    void slot_download(STRU_DOWNLOADFILE_RS*);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_11_clicked();

    void on_tableWidget_cellClicked(int row, int column);//下载

public:
    Ui::Widget *ui;
    SearchFile* psf;
    ShowSearchFile* showsf;
    TCPKernel *m_pKernel;
    login *m_plogin;
    long long m_userId;
    std::map<QString,UploadFileInfo*> m_mapFileNametoFileInfo;
    int m_FileNum;
    Threadpool m_tp;
    std::map<int,DownloadFileInfo*> m_mapFidtoDownloadfileInfo;
};

class SendFileItask:public task
{
public:
    SendFileItask(UploadFileInfo* pInfo,long long pos,long long fileId,Widget *pWidget)
    {
        m_pInfo=pInfo;
        m_pos=pos;
        m_pWidget=pWidget;
        m_fileId=fileId;
    }
    ~SendFileItask()
    {

    }
public:
    void run();
private:
    UploadFileInfo* m_pInfo;
    long long m_pos;
    Widget* m_pWidget;
    long long m_fileId;

};

#endif // WIDGET_H
