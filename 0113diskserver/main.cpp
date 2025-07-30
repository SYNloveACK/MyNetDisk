#include <QCoreApplication>
#include"./kernel/tcpkernel.h"
void cheakheart(){

}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
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
