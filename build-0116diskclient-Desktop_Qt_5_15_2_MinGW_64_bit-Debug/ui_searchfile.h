/********************************************************************************
** Form generated from reading UI file 'searchfile.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHFILE_H
#define UI_SEARCHFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SearchFile
{
public:
    QLineEdit *lineEdit;
    QLabel *label;
    QPushButton *pushButton;

    void setupUi(QWidget *SearchFile)
    {
        if (SearchFile->objectName().isEmpty())
            SearchFile->setObjectName(QString::fromUtf8("SearchFile"));
        SearchFile->resize(400, 300);
        lineEdit = new QLineEdit(SearchFile);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(130, 100, 191, 41));
        label = new QLabel(SearchFile);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 100, 41, 41));
        pushButton = new QPushButton(SearchFile);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(130, 200, 81, 31));

        retranslateUi(SearchFile);

        QMetaObject::connectSlotsByName(SearchFile);
    } // setupUi

    void retranslateUi(QWidget *SearchFile)
    {
        SearchFile->setWindowTitle(QCoreApplication::translate("SearchFile", "Form", nullptr));
        label->setText(QCoreApplication::translate("SearchFile", "\346\226\207\344\273\266\345\220\215\357\274\232", nullptr));
        pushButton->setText(QCoreApplication::translate("SearchFile", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SearchFile: public Ui_SearchFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHFILE_H
