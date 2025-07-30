#include "searchfile.h"
#include "ui_searchfile.h"

SearchFile::SearchFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchFile)
{
    ui->setupUi(this);
}

SearchFile::~SearchFile()
{
    delete ui;
    //delete pKernel;
}

void SearchFile::on_pushButton_clicked()
{
    QString qs=ui->lineEdit->text();
    STRU_SEARCHFILE_RQ ssq;
    ssq.m_userId=this->userid;
    strcpy(ssq.filename,qs.toStdString().c_str());
    pKernel->sendData((char*)&ssq,sizeof(ssq));
}

