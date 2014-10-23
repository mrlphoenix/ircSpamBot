#include "abstractircsocket.h"

void AbstractIrcSocket::login()
{
    QString passStr = "PASS "+pass_+"\r\n";
    QString nickStr = "NICK "+nickname_+"\r\n";
    QString sysName = " " + nickname_.toLower()+"."+server_;
    QString userStr = "USER "+nickname_ + sysName + sysName+ " :NOTHERBOT\r\n";
    QString joinStr = "JOIN #" + channel_ + "\r\n";
    socket->write(passStr.toUtf8());
    socket->write(nickStr.toUtf8());
    socket->write(userStr.toUtf8());
    socket->write(joinStr.toUtf8());
}

AbstractIrcSocket::AbstractIrcSocket(QObject *parent) :
    QObject(parent)
{
    words_ = 0;
    socket = 0;
}

AbstractIrcSocket::AbstractIrcSocket(QString host, QString channel, QString nickname, QString pass, QStringList *words)
{
    server_ = host;
    channel_ = channel;
    nickname_ = nickname;
    pass_ = pass;
    words_ = words;
    socket = new QTcpSocket(this);
    //connecting "connected" signal from internal socket to "connected" slot
    connect(socket,SIGNAL(connected()),SLOT(connected()));
    //connecting "readyRead" signal from internal socket to "parseMessage" slot
    //"readyRead" emitted every time socket got some data from server
    connect(socket,SIGNAL(readyRead()),SLOT(parseMessage()));
}

void AbstractIrcSocket::connectToHost() const
{
    if (!socket)
        return;
    //IRC protocol port = 6667
    socket->connectToHost(server_,6667);
}

void AbstractIrcSocket::disconnect()
{
    socket->disconnect(this);
}

void AbstractIrcSocket::connected()
{
    login();
    emit connectSignal();
}
