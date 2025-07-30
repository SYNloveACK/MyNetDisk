#ifndef SEARCHFILE_H
#define SEARCHFILE_H

#include <QWidget>
#include "./kernel/tcpkernel.h"
namespace Ui {
class SearchFile;
}

class SearchFile : public QWidget
{
    Q_OBJECT

public:
    explicit SearchFile(QWidget *parent = nullptr);
    ~SearchFile();
    void setKernel(IKernel* Kernel)
    {
        pKernel=Kernel;
    }

private slots:
    void on_pushButton_clicked();

private:
    Ui::SearchFile *ui;
    IKernel* pKernel;
public:
    long long userid;

};

#endif // SEARCHFILE_H
