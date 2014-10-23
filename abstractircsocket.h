#ifndef ABSTRACTIRCSOCKET_H
#define ABSTRACTIRCSOCKET_H

#include <QObject>
#include <QStringList>
#include <QTcpSocket>

class AbstractIrcSocket : public QObject
{
    Q_OBJECT
protected:
    QTcpSocket *socket;
    QString nickname_, pass_;
    QString channel_;
    QString server_;
    QStringList * words_;
    virtual void login();
public:
    explicit AbstractIrcSocket(QObject *parent = 0);
    explicit AbstractIrcSocket(QString host, QString channel, QString nickname, QString pass,
                               QStringList* words);
    virtual void connectToHost() const;
    virtual void disconnect();

signals:
    void connectSignal();

public slots:
    virtual void parseMessage()=0;
    void connected();
};

#endif // ABSTRACTIRCSOCKET_H
