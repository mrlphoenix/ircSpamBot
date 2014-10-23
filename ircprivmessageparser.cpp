#include "ircprivmessageparser.h"

IrcPrivMessageParser IrcPrivMessageParser::parse(QString message)
{
    QStringList parts = message.split(" ");
    QString nick = parts.at(0).mid(1,parts.at(0).indexOf("!")-1);
    QString msg;
    for (int i=3; i<parts.count(); i++)
    {
        msg += parts.at(i);
        if (i!= parts.count()-1)
            msg += " ";
    }
    IrcPrivMessageParser result;
    result.nickname = nick;
    result.message = msg.mid(1,msg.length()-1);
    return result;
}

IrcPrivMessageParser::IrcPrivMessageParser()
{
}

