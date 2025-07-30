/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label;
    QLineEdit *lineEdit_rtel;
    QLineEdit *lineEdit_ruser;
    QLineEdit *lineEdit_rpassword;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *pushButton;
    QWidget *tab_2;
    QLabel *label_7;
    QLineEdit *lineEdit_luser;
    QLineEdit *lineEdit_lpassword;
    QLabel *label_8;
    QPushButton *pushButton_3;

    void setupUi(QWidget *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName(QString::fromUtf8("login"));
        login->resize(501, 330);
        tabWidget = new QTabWidget(login);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 0, 481, 321));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 20, 49, 14));
        lineEdit_rtel = new QLineEdit(tab);
        lineEdit_rtel->setObjectName(QString::fromUtf8("lineEdit_rtel"));
        lineEdit_rtel->setGeometry(QRect(70, 20, 113, 22));
        lineEdit_ruser = new QLineEdit(tab);
        lineEdit_ruser->setObjectName(QString::fromUtf8("lineEdit_ruser"));
        lineEdit_ruser->setGeometry(QRect(70, 60, 113, 22));
        lineEdit_rpassword = new QLineEdit(tab);
        lineEdit_rpassword->setObjectName(QString::fromUtf8("lineEdit_rpassword"));
        lineEdit_rpassword->setGeometry(QRect(70, 100, 113, 22));
        lineEdit_rpassword->setEchoMode(QLineEdit::Password);
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 60, 49, 14));
        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 100, 49, 14));
        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(60, 150, 80, 20));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        label_7 = new QLabel(tab_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 50, 49, 14));
        lineEdit_luser = new QLineEdit(tab_2);
        lineEdit_luser->setObjectName(QString::fromUtf8("lineEdit_luser"));
        lineEdit_luser->setGeometry(QRect(70, 50, 251, 31));
        lineEdit_lpassword = new QLineEdit(tab_2);
        lineEdit_lpassword->setObjectName(QString::fromUtf8("lineEdit_lpassword"));
        lineEdit_lpassword->setGeometry(QRect(70, 90, 251, 31));
        lineEdit_lpassword->setEchoMode(QLineEdit::Password);
        label_8 = new QLabel(tab_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 90, 49, 14));
        pushButton_3 = new QPushButton(tab_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(70, 150, 131, 41));
        tabWidget->addTab(tab_2, QString());

        retranslateUi(login);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QWidget *login)
    {
        login->setWindowTitle(QCoreApplication::translate("login", "Form", nullptr));
        label->setText(QCoreApplication::translate("login", "tel", nullptr));
        lineEdit_rtel->setPlaceholderText(QCoreApplication::translate("login", "\350\257\267\350\276\223\345\205\245\347\224\265\350\257\235\345\217\267\347\240\201", nullptr));
        lineEdit_ruser->setPlaceholderText(QCoreApplication::translate("login", "\350\257\267\350\276\223\345\205\245\347\224\250\346\210\267\345\220\215", nullptr));
        lineEdit_rpassword->setPlaceholderText(QCoreApplication::translate("login", "\350\257\267\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        label_2->setText(QCoreApplication::translate("login", "user", nullptr));
        label_3->setText(QCoreApplication::translate("login", "password", nullptr));
        pushButton->setText(QCoreApplication::translate("login", "\346\263\250\345\206\214", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("login", "\346\263\250\345\206\214", nullptr));
        label_7->setText(QCoreApplication::translate("login", "user", nullptr));
        lineEdit_luser->setPlaceholderText(QCoreApplication::translate("login", "\350\257\267\350\276\223\345\205\245\347\224\250\346\210\267\345\220\215", nullptr));
        lineEdit_lpassword->setPlaceholderText(QCoreApplication::translate("login", "\350\257\267\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        label_8->setText(QCoreApplication::translate("login", "password", nullptr));
        pushButton_3->setText(QCoreApplication::translate("login", "\347\231\273\345\275\225", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("login", "\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
