#include "grabmessageircsocket.h"


GrabMessageIrcSocket::GrabMessageIrcSocket(QObject *parent) :
    AbstractIrcSocket(parent)
{
    textEdit_ = 0;
    spambots_ = 0;
}

GrabMessageIrcSocket::GrabMessageIrcSocket(QString host, QString channel, QString nickname, QString pass,
                                           QStringList *words, QStringList *spambots, QTextEdit *out) :
                      AbstractIrcSocket(host,channel,nickname,pass,words)
{
    textEdit_ = out;
    spambots_ = spambots;
}


void GrabMessageIrcSocket::parseMessage()
{
    while(socket->canReadLine())
    {
    QString readLine = socket->readLine();
    if (readLine.indexOf("PING ")!=-1)
    {
        readLine[1] = QChar('O');
        socket->write(readLine.toUtf8());
    }
    if (readLine.indexOf("PRIVMSG #"+channel_)!=-1)
    {
        IrcPrivMessageParser message = IrcPrivMessageParser::parse(readLine);
        if (textEdit_)
            textEdit_->append(message.nickname + ": " + message.message);
        if (!spambots_->contains(message.nickname))
        {
            QStringList wordsInMessage = message.message.split(" ");
            QString wordToAdd;
            foreach (const QString& str, wordsInMessage)
            {
                wordToAdd += (" " + str);
                int randomInt = rand()%100;
                if (randomInt < 80)
                {
                    if (!words_->contains(wordToAdd,Qt::CaseInsensitive))
                        words_->append(wordToAdd.replace("\r\n",""));
                    wordToAdd = "";
                }
            }
            if (wordToAdd != "" && !words_->contains(wordToAdd,Qt::CaseInsensitive))
                words_->append(wordToAdd.replace("\r\n",""));
            emit wordAdded();
        }
    }
    }
}
