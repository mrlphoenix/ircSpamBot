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

    //Reading file with accounts
    //file format:
    //login
    //pass
    //login
    //pass
    //...
    //login
    //pass

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
    //choosing random acc in accounts list for listening
    //and removing it from account list
    int randomListenerIndex = rand()%count;
    accountDescriptor grabberAccount = accounts.at(randomListenerIndex);
    accounts.removeAt(randomListenerIndex);
    //creating N-1 spambots sockets
    foreach (const accountDescriptor& d, accounts)
    {
        SpamMessageSocket * sock = new SpamMessageSocket(server_,channel_,d.login,d.pass,&words_,minWords_,maxWords_);
        spamSockets.append(sock);
        spambots_.append(d.login.toLower());
    }
    //creating listening socket
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
    //connecting signal and slots for spambots
    //and connecting them to IRC server
    foreach (const SpamMessageSocket* ss, spamSockets)
    {
        connect(ss,SIGNAL(connectSignal()), SLOT(socketConnected()));
        ss->connectToHost();
    }
    //connecting signals and slots for listening sock and connecting it to the server
    connect(grabSocket, SIGNAL(connectSignal()), SLOT(socketConnected()));
    grabSocket->connectToHost();
    botsCount = spambots_.count() + 1;
    //emit signal "connecting state"
    emit connectingState(botsCount);
}

void SpamSocketManager::socketConnected()
{
    socketConnectedCount++;
    emit socketConnectedSignal();
    //when all sockets is connected - start grabbing state
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
    //when count of grabbed words is enough - start spamming state
    else if (currentMessages_ == maxMessages_)
    {
        emit messageGrabbed();
        emit spammingState();
        spamTimer = new QTimer();
        //connect spam timer with spam() slot
        //timer timeout time depends on globalCD and count of bots
        connect(spamTimer,SIGNAL(timeout()),SLOT(spam()));
        spamTimer->start(globalCD_/spambots_.count());
    }
}

void SpamSocketManager::spam()
{
    //send message with current spam socket
    spamSockets.at(currentSpamSocket)->sendMessage();
    //choose next spam socket
    currentSpamSocket = (currentSpamSocket+1)%spambots_.count();
}
