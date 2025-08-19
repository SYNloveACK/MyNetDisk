#include <QCoreApplication>
#include"./kernel/tcpkernel.h"
#include "Logger.h"



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 初始化日志系统
    Logger::initialize();

    LOG_INFO("应用程序启动");

    IKernel *p=TCPKernel::getKernel();
    if(p->open())
    {
        printf("server is running\n");

    }
    else
    {
        printf("server err\n");
    }
    //TCPKernel* tcp;


    return a.exec();
}
