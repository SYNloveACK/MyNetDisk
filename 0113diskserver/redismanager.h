#ifndef REDISMANAGER_H
#define REDISMANAGER_H

#include <hiredis/hiredis.h>
#include <QString>
#include <QObject>
#include <QMutex>
#include <QTimer>
#include "Logger.h"
#include "json.hpp"

using namespace std;
#define json nlohmann::json
class RedisManager : public QObject
{
    Q_OBJECT

public:
    // 数据库枚举
    enum Database {
        SHARE_LINK_DB = 0 //key= file:link

    };

    static RedisManager& instance();

    bool connect(const QString& host = "127.0.0.1", int port = 6379);
    void disconnect();
    bool isConnected() const;

    // 基本操作
    bool set(const QString& key, const QString& value, int expireSeconds = 0);
    QString get(const QString& key);
    bool exists(const QString& key);
    bool del(const QString& key);

    bool storeFileInfoJson(string f_id, string f_path,string link );
    bool getFileInfoJson(string link,json& js);

signals:
    void connectionStatusChanged(bool connected);
    void errorOccurred(const QString& error);

private slots:
    void checkConnection();

private:
    explicit RedisManager(QObject* parent = nullptr);
    ~RedisManager();

    bool selectDatabase(Database db);
    redisContext* getContext();

    redisContext* m_context;
    QMutex m_mutex;
    QTimer* m_heartbeatTimer;
    bool m_connected;
};

#endif // REDISMANAGER_H
