#ifndef IRCPRIVMESSAGEPARSER_H
#define IRCPRIVMESSAGEPARSER_H

#include <QStringList>

class IrcPrivMessageParser
{
public:
    QString nickname;
    QString message;

    //Parses the messega to two strings: sender's nickname and his message exactly.
    static IrcPrivMessageParser parse(QString message);
    IrcPrivMessageParser();
    ~IrcPrivMessageParser(){;}
};

#endif // IRCPRIVMESSAGEPARSER_H
