#ifndef SPAMSOCKETMANAGER_H
#define SPAMSOCKETMANAGER_H

#include <QObject>
#include <QList>
#include <QTextEdit>
#include <QTimer>
#include <QFile>
#include "spammessagesocket.h"
#include "grabmessageircsocket.h"

struct accountDescriptor
{
    QString login;
    QString pass;
};

class SpamSocketManager : public QObject
{
    Q_OBJECT
    QList<SpamMessageSocket*> spamSockets;      //list of spamSockets
    GrabMessageIrcSocket * grabSocket;          //listening socket
    QList<accountDescriptor> accounts;          //accounts List
    QTextEdit * out_;                           //textEdit pointer to draw messages
    QTimer * spamTimer;                         //spam timer
    QStringList words_, spambots_;              //words_ - list of spam words; spambots_ - list of spambots nicknames
    QString server_, channel_;
    int globalCD_;                              //global CD to spam
    int currentSpamSocket;                      //current socket to spam
    int socketConnectedCount;                   //connected sockets count
    int maxMessages_, currentMessages_;         //maxMessages to read before attack; currentMessages Grabbed
    int minWords_,maxWords_;
    enum SpamSocketManagerState{CONNECTING_STATE, GRABBING_MESSAGES_STATE, SPAMMING_STATE};
    SpamSocketManagerState state;

    void loadAccounts(QString filename);        //Method for loading account info from file
    void createSockets();                       //Method setuping sockets

public:
    explicit SpamSocketManager(QObject *parent = 0);
    explicit SpamSocketManager(QString server, QString channel, QString accountsFile,
                               int globalCD, int maxMessages,
                               int minWords, int maxWords,QTextEdit* out);
    void start();                               //starts work
    void stop();
    int botsCount;

signals:
    void connectingState(int count);            //Sends when manager is tarting to connect sockets
    void grabbingState(int count);              //Sends when manager is starting to grab messages
    void spammingState();                       //Sends when manager is starting to spam
    void socketConnectedSignal();               //Sends when socket got connected
    void messageGrabbed();                      //Sends when message is grabbed by listener socket

public slots:
    void socketConnected();                     //processor of internal "connected()" signal
    void wordAdded();                           //processor of internal "wordAdded()" signal
    void spam();                                //called every time when its time to write spam message

};

#endif // SPAMSOCKETMANAGER_H
