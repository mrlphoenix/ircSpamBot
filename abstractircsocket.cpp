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
    /*
    ircSocket->Socket->SendText("PASS "+cfg.Get("pass")+"\r\n");
    Sleep(uint(100));
    ircSocket->Socket->SendText("NICK "+cfg.Get("nick")+"\r\n");
    Sleep(uint(100));

    String sysName = String(" ") + cfg.Get("nick").LowerCase()+"."+cfg.Get("server");

    ircSocket->Socket->SendText("USER " + cfg.Get("nick") + sysName + sysName + " :NotherKappa Bot\r\n");
    Sleep(uint(100));
    ircSocket->Socket->SendText("JOIN #" + cfg.Get("channel") + "\r\n");
    Sleep(uint(100));
    */
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
    connect(socket,SIGNAL(connected()),SLOT(connected()));
    connect(socket,SIGNAL(readyRead()),SLOT(parseMessage()));
}

void AbstractIrcSocket::connectToHost() const
{
    if (!socket)
        return;
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
