#ifndef SPAMMESSAGESOCKET_H
#define SPAMMESSAGESOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QStringList>
#include <QtGlobal>
#include "abstractircsocket.h"

class SpamMessageSocket : public AbstractIrcSocket
{
    Q_OBJECT
    //min and max count of words in spam message
    int minWords_, maxWords_;
public:
    explicit SpamMessageSocket(QObject *parent = 0);
    explicit SpamMessageSocket(QString host, QString channel, QString nickname, QString pass,
                               QStringList* words, int minWords, int maxWords);
    void sendMessage();

signals:

public slots:
    void parseMessage();
};

#endif // SPAMMESSAGESOCKET_H
