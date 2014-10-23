#ifndef IRCPRIVMESSAGEPARSER_H
#define IRCPRIVMESSAGEPARSER_H

#include <QStringList>

class IrcPrivMessageParser
{
public:
    QString nickname;
    QString message;

    static IrcPrivMessageParser parse(QString message);
    IrcPrivMessageParser();
    ~IrcPrivMessageParser(){;}
};

#endif // IRCPRIVMESSAGEPARSER_H
