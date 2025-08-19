#include "Logger.h"

std::shared_ptr<spdlog::logger> Logger::logger_ = nullptr;

void Logger::initialize() {
    try {
        // 确保日志目录存在
        QString logDir = "C:/Users/Lenovo/Desktop/netdisk/0113diskserver/logs";

        QDir().mkpath(logDir);

        // 创建每日滚动的日志文件
        auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
            logDir.toStdString() + "/app_", 0, 0);

        logger_ = std::make_shared<spdlog::logger>("logger", daily_sink);
        logger_->set_level(spdlog::level::debug);

        // 关键：显式设置日志格式，%v 表示日志内容（包含你的格式化字符串）
        logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v");

        spdlog::set_default_logger(logger_);

        LOG_INFO("日志系统初始化完成");
        // 初始化时添加测试
//        logger_->info("测试格式化：id=%d, 大小=%zu", 100, 2048U);
//        logger_->error("测试 fmt 风格格式化：id={}, 大小={}", 123, 456U);
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
