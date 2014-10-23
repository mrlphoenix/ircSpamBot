#include "spamsocketmanager.h"

void SpamSocketManager::loadAccounts(QString filename)
{
    accounts.clear();
    QFile textFile(filename);
    if(!textFile.open(QIODevice::ReadOnly))
    {
        qDebug("Error");
        return;
    }

    QTextStream textStream(&textFile);
    bool oddLine = false;
    while (true)
    {
        QString line = textStream.readLine();
        if (line.isNull())
            break;
        else
        {
            if (oddLine)
            {
                accounts.last().pass = line;
                oddLine = false;
            }
            else
            {
                accountDescriptor acc;
                acc.login = line;
                accounts.append(acc);
                oddLine = true;
            }
        }
    }
    textFile.close();
}

void SpamSocketManager::createSockets()
{
    int count = accounts.count();
    int randomListenerIndex = rand()%count;
    accountDescriptor grabberAccount = accounts.at(randomListenerIndex);
    accounts.removeAt(randomListenerIndex);
    foreach (const accountDescriptor& d, accounts)
    {
        SpamMessageSocket * sock = new SpamMessageSocket(server_,channel_,d.login,d.pass,&words_,minWords_,maxWords_);
        spamSockets.append(sock);
        spambots_.append(d.login.toLower());
    }

    grabSocket = new GrabMessageIrcSocket(server_,channel_,grabberAccount.login,grabberAccount.pass,
                                          &words_, &spambots_, out_);
}

SpamSocketManager::SpamSocketManager(QObject *parent) :
    QObject(parent)
{
}

SpamSocketManager::SpamSocketManager(QString server, QString channel, QString accountsFile,
                                     int globalCD, int maxMessages, int minWords, int maxWords, QTextEdit* out)
{
    server_ = server;
    channel_ = channel;

    loadAccounts(accountsFile);
    globalCD_ = globalCD;
    maxMessages_ = maxMessages;
    currentMessages_ = 0;
    currentSpamSocket = 0;
    socketConnectedCount = 0;
    minWords_ = minWords;
    maxWords_ = maxWords;
    state = CONNECTING_STATE;
    out_ = out;
    createSockets();
}

void SpamSocketManager::start()
{
    foreach (const SpamMessageSocket* ss, spamSockets)
    {
        connect(ss,SIGNAL(connectSignal()), SLOT(socketConnected()));
        ss->connectToHost();
    }
    connect(grabSocket, SIGNAL(connectSignal()), SLOT(socketConnected()));
    grabSocket->connectToHost();
    botsCount = spambots_.count() + 1;
    emit connectingState(botsCount);
}

void SpamSocketManager::socketConnected()
{
    socketConnectedCount++;
    emit socketConnectedSignal();
    if (socketConnectedCount == botsCount)
    {
        state = GRABBING_MESSAGES_STATE;
        connect(grabSocket, SIGNAL(wordAdded()), SLOT(wordAdded()));
        emit grabbingState(maxMessages_);
    }

}

void SpamSocketManager::wordAdded()
{
    currentMessages_++;
    if (currentMessages_ > maxMessages_)
        return;
    if (currentMessages_ < maxMessages_)
        emit messageGrabbed();
    else if (currentMessages_ == maxMessages_)
    {
        emit messageGrabbed();
        emit spammingState();
        spamTimer = new QTimer();
        connect(spamTimer,SIGNAL(timeout()),SLOT(spam()));
        spamTimer->start(globalCD_/spambots_.count());
    }
}

void SpamSocketManager::spam()
{
    spamSockets.at(currentSpamSocket)->sendMessage();
    currentSpamSocket = (currentSpamSocket+1)%spambots_.count();
}
