#include "spammessagesocket.h"

SpamMessageSocket::SpamMessageSocket(QObject *parent) :
    AbstractIrcSocket(parent)
{
    socket = 0;
    words_ = 0;
}

SpamMessageSocket::SpamMessageSocket(QString host, QString channel, QString nickname, QString pass,
                                     QStringList *words, int minWords, int maxWords) :
                                     AbstractIrcSocket(host,channel,nickname,pass,words)

{
    minWords_ = minWords;
    maxWords_ = maxWords;
}

void SpamMessageSocket::sendMessage()
{
    //message prefix
    QString out = "PRIVMSG #" + channel_ + " :";
    //calculating words count: random between minWords_ and maxWords_
    int wordsCount = (qrand()%(maxWords_-minWords_+1))+minWords_;
    //picking up $wordsCount words from words_ list
    //and writing it into socket
    for (int i=0; i<wordsCount; i++)
    {
        out += words_->at(qrand()%words_->count());
        if (i != wordsCount-1)
            out+= " ";
    }
    out+="\r\n";
    socket->write(out.toUtf8());
}

void SpamMessageSocket::parseMessage()
{
    while(socket->canReadLine())
    {
        QString readLine = socket->readLine();
        //answering a PING command
        if (readLine.indexOf("PING ")!=-1)
        {
            readLine[1] = QChar('O');
            socket->write(readLine.toUtf8());
        }
    }
}
