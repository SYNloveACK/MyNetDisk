#ifndef LOGIN_H
#define LOGIN_H
#include"Packdef.h"
#include"./kernel/IKernel.h"
#include <QWidget>
#include<QMessageBox.h>
namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    void setKernel(IKernel* pKernel)
    {
        m_pKernel=pKernel;
    }

private slots:
   void on_pushButton_clicked();

    void on_pushButton_3_clicked();

public slots:
    void slot_register(STRU_REGISTER_RS* psrr);

private:
    Ui::login *ui;
    IKernel *m_pKernel;
};

#endif // LOGIN_H
