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

    //method for logining on server
    virtual void login();
public:
    explicit AbstractIrcSocket(QObject *parent = 0);
    ///
    /// \brief AbstractIrcSocket
    /// \param host Irc server to connect
    /// \param channel channel on IRC server
    /// \param nickname your nickname
    /// \param pass your pass (or oAuth code for twitch)
    /// \param words pointer to list with words to spam
    ///
    explicit AbstractIrcSocket(QString host, QString channel, QString nickname, QString pass,
                               QStringList* words);
    virtual void connectToHost() const;
    virtual void disconnect();

signals:
    //this signal sends when socket got connected
    void connectSignal();

public slots:
    //this slot is for processing incoming messages from server
    virtual void parseMessage()=0;
    //this slot is activated when socket is connected
    void connected();
};

#endif // ABSTRACTIRCSOCKET_H
