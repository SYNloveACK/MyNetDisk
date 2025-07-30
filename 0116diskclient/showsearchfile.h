#ifndef SHOWSEARCHFILE_H
#define SHOWSEARCHFILE_H

#include <QWidget>
#include"Packdef.h"
namespace Ui {
class ShowSearchFile;
}

class ShowSearchFile : public QWidget
{
    Q_OBJECT

public:
    explicit ShowSearchFile(QWidget *parent = nullptr);
    ~ShowSearchFile();
public slots:
    void slot_showsearchfile(STRU_SEARCHFILE_RS* szbuf);

private:
    Ui::ShowSearchFile *ui;
};

#endif // SHOWSEARCHFILE_H
