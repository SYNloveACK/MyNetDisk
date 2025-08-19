#include "widget.h"
#include "logger.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    Logger::initialize();
    LOG_INFO("应用程序启动");
    Widget w;
    //w.show();

    w.hide();//主窗口隐藏
    return a.exec();
}
