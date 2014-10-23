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
    //reading every line from socket
    while(socket->canReadLine())
    {
        QString readLine = socket->readLine();
        //Answering to "PING origin" command
        if (readLine.indexOf("PING ")!=-1)
        {
            readLine[1] = QChar('O');
            socket->write(readLine.toUtf8());
        }
        //Processing message in chat
        if (readLine.indexOf("PRIVMSG #"+channel_)!=-1)
        {
            //parsing message
            IrcPrivMessageParser message = IrcPrivMessageParser::parse(readLine);
            //if textEdit was set up - appending line with message
            if (textEdit_)
                textEdit_->append(message.nickname + ": " + message.message);
            //if message sender's nickname is not in spambots list
            //then parse his message as words and add it in words list
            if (!spambots_->contains(message.nickname))
            {
                QStringList wordsInMessage = message.message.split(" ");
                QString wordToAdd;
                foreach (const QString& str, wordsInMessage)
                {
                    wordToAdd += (" " + str);
                    //20% chance that "word" will be a sentence of two words
                    //4% chance that "word" will be a sentence of three words
                    //1% of four words
                    //etc
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
