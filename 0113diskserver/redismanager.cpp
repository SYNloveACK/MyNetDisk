#include "redismanager.h"
#include <QThread>
#include <QDebug>

RedisManager& RedisManager::instance()
{
    static RedisManager instance;
    return instance;
}

RedisManager::RedisManager(QObject* parent)
    : QObject(parent), m_context(nullptr), m_connected(false)
{
    m_heartbeatTimer = new QTimer(this);
    QObject::connect(m_heartbeatTimer, &QTimer::timeout, this, &RedisManager::checkConnection);
}

RedisManager::~RedisManager()
{
    disconnect();
}

bool RedisManager::connect(const QString& host, int port)
{
    QMutexLocker locker(&m_mutex);

    if (m_context) {
        redisFree(m_context);
        m_context = nullptr;
    }

    // 连接Redis服务器
    m_context = redisConnect(host.toUtf8().constData(), port);
    if (m_context == nullptr || m_context->err) {
        QString error = m_context ? QString::fromUtf8(m_context->errstr) : "Cannot allocate Redis context";
        LOG_ERROR("Redis连接失败");
        emit errorOccurred(QString("Redis连接失败: %1").arg(error));

        if (m_context) {
            redisFree(m_context);
            m_context = nullptr;
        }

        m_connected = false;
        emit connectionStatusChanged(false);
        return false;
    }

    m_connected = true;
    emit connectionStatusChanged(true);

    // 启动心跳检测
    m_heartbeatTimer->start(30000); // 30秒检测一次
    return true;
}

void RedisManager::disconnect()
{
    QMutexLocker locker(&m_mutex);

    m_heartbeatTimer->stop();

    if (m_context) {
        redisFree(m_context);
        m_context = nullptr;
    }

    m_connected = false;
    emit connectionStatusChanged(false);
}

bool RedisManager::isConnected() const
{
    return m_connected;
}

bool RedisManager::selectDatabase(Database db)
{
    if (!m_context) return false;

    redisReply* reply = (redisReply*)redisCommand(m_context, "SELECT %d", db);
    if (reply == nullptr || reply->type == REDIS_REPLY_ERROR) {
        if (reply) freeReplyObject(reply);
        return false;
    }

    freeReplyObject(reply);
    return true;
}

bool RedisManager::set(const QString& key, const QString& value, int expireSeconds)
{
    QMutexLocker locker(&m_mutex);
    if (!m_context) return false;

    redisReply* reply = nullptr;
    if (expireSeconds > 0) {
        reply = (redisReply*)redisCommand(m_context, "SET %s %s EX %d",
                                        key.toUtf8().constData(),
                                        value.toUtf8().constData(),
                                        expireSeconds);
    } else {
        reply = (redisReply*)redisCommand(m_context, "SET %s %s",
                                        key.toUtf8().constData(),
                                        value.toUtf8().constData());
    }

    if (reply == nullptr || reply->type == REDIS_REPLY_ERROR) {
        if (reply) freeReplyObject(reply);
        return false;
    }

    freeReplyObject(reply);
    return true;
}

QString RedisManager::get(const QString& key)
{
    QMutexLocker locker(&m_mutex);
    if (!m_context) return QString();

    redisReply* reply = (redisReply*)redisCommand(m_context, "GET %s",
                                                 key.toUtf8().constData());
    if (reply == nullptr || reply->type != REDIS_REPLY_STRING) {
        if (reply) freeReplyObject(reply);
        return QString();
    }

    QString result = QString::fromUtf8(reply->str);
    freeReplyObject(reply);
    return result;
}

bool RedisManager::storeFileInfoJson(string f_id, string f_path, string link)
{
    selectDatabase(SHARE_LINK_DB);
    json js=json::object();
    js["f_id"]=f_id;
    js["f_path"]=f_path;
    js["link"]=link;
    string key="file:"+link;
    if(set(key.c_str(),js.dump().c_str(),60*60*24))
    {
        return true;
    }
    else
    {
        LOG_ERROR("storeFileInfoJson insert fail！");
        return false;
    }
}

bool RedisManager::getFileInfoJson(string link,json& js)
{
    selectDatabase(SHARE_LINK_DB);
    QString str=("file:"+link).c_str();
    LOG_INFO("link key:{}",str.toStdString());
    QString res=get(str);
    if(res.isEmpty())
    {
        LOG_ERROR("未查询到文件信息！");
        return false;
    }
    // 解析JSON
    try {
        js = json::parse(res.toStdString());
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR("JSON解析失败: {}, 原始数据: {}", e.what(), res.toStdString());
        return false;
    }
}

void RedisManager::checkConnection()
{
    QMutexLocker locker(&m_mutex);
    if (!m_context) {
        m_connected = false;
        emit connectionStatusChanged(false);
        return;
    }

    // 发送PING命令检查连接
    redisReply* reply = (redisReply*)redisCommand(m_context, "PING");
    if (reply == nullptr || reply->type == REDIS_REPLY_ERROR) {
        m_connected = false;
        emit connectionStatusChanged(false);
        emit errorOccurred("Redis连接已断开");
    } else {
        freeReplyObject(reply);
        m_connected = true;
    }
}
