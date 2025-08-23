#include <QCoreApplication>
#include"./kernel/tcpkernel.h"
#include "Logger.h"
#include "redismanager.h"

// 信号处理函数：收到退出信号后，通知 Qt 事件循环退出
void handleExitSignal(int signal) {
    qDebug() << "收到退出信号：" << signal;
    // 获取 QCoreApplication 实例，调用 quit() 退出事件循环
    QCoreApplication* app = QCoreApplication::instance();
    if (app) {
        app->quit();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 关键：注册信号处理函数，捕获 Ctrl+C（SIGINT）和 kill 命令（SIGTERM）
    signal(SIGINT, handleExitSignal);   // 捕获 Ctrl+C
    signal(SIGTERM, handleExitSignal);  // 捕获系统 kill 信号

    // 初始化日志系统
    Logger::initialize();

    LOG_INFO("应用程序启动");

    // 获取单例实例
    RedisManager& redis = RedisManager::instance();

    // 连接Redis
    if (redis.connect()) {
        LOG_INFO("redis 连接成功");
    }

    IKernel *p=TCPKernel::getKernel();
    if(p->open())
    {
        printf("server is running\n");

    }
    else
    {
        printf("server err\n");
    }

    // 等待事件循环退出（此时若收到信号，会触发 handleExitSignal 调用 a.quit()）
    int exitCode = a.exec();

    // 事件循环退出后，执行“清理操作”
    LOG_INFO("应用程序正常退出，退出码：{}", exitCode);

    // 关键：手动刷新 spdlog 日志缓冲，确保所有日志写入磁盘
    spdlog::shutdown();  // spdlog 关闭时会自动刷新所有 sink 的缓冲

    return exitCode;
}
