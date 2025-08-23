#include "logger.h"

std::shared_ptr<spdlog::logger> Logger::logger_ = nullptr;

void Logger::initialize() {
    try {
        // 确保日志目录存在
        QString logDir ="C:/Users/Lenovo/Desktop/netdisk/0113diskserver/logs";
        QDir().mkpath(logDir);

        // 创建每日滚动的日志文件
        auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
            logDir.toStdString() + "/app_", 0, 0);

        logger_ = std::make_shared<spdlog::logger>("multi_file_logger", daily_sink);
        logger_->set_level(spdlog::level::debug);
        spdlog::set_default_logger(logger_);

        LOG_INFO("日志系统初始化完成");
        printf("log init success\n");
    } catch (const spdlog::spdlog_ex& ex) {
        qCritical("日志初始化失败: %s", ex.what());
    }
}

std::shared_ptr<spdlog::logger> Logger::getLogger() {
    if (!logger_) {
        initialize();
    }
    return logger_;
}
