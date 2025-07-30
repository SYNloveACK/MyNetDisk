#include "showsearchfile.h"
#include "ui_showsearchfile.h"
#include<QMessageBox>
ShowSearchFile::ShowSearchFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowSearchFile)
{
    ui->setupUi(this);
}

ShowSearchFile::~ShowSearchFile()
{
    delete ui;
}

void ShowSearchFile::slot_showsearchfile(STRU_SEARCHFILE_RS *szbuf)
{

    if(szbuf->result==false){
            QMessageBox::information(this,"","未找到对应文件");
        return;
        }
    QTableWidgetItem *pitem;
    for(int i=0;i<szbuf->m_FileNum;i++){
        pitem=new QTableWidgetItem(szbuf->m_aryInfo[i].m_szFileName);
        ui->tableWidget->setItem(i,0,pitem);
        pitem=new QTableWidgetItem(std::to_string(szbuf->m_aryInfo[i].m_fileSize).c_str());
        ui->tableWidget->setItem(i,1,pitem);
        pitem=new QTableWidgetItem(szbuf->m_aryInfo[i].m_szFileDateTime);
        ui->tableWidget->setItem(i,2,pitem);
    }
    this->show();
    //ui->tableWidget->show();
}
