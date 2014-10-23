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
    QList<SpamMessageSocket*> spamSockets;
    GrabMessageIrcSocket * grabSocket;
    QList<accountDescriptor> accounts;
    QTextEdit * out_;
    QTimer * spamTimer;
    QStringList words_, spambots_;
    QString server_, channel_;
    int globalCD_;
    int currentSpamSocket;
    int socketConnectedCount;
    int maxMessages_, currentMessages_;
    int minWords_,maxWords_;
    enum SpamSocketManagerState{CONNECTING_STATE, GRABBING_MESSAGES_STATE, SPAMMING_STATE};
    SpamSocketManagerState state;

    void loadAccounts(QString filename);
    void createSockets();

public:
    explicit SpamSocketManager(QObject *parent = 0);
    explicit SpamSocketManager(QString server, QString channel, QString accountsFile,
                               int globalCD, int maxMessages,
                               int minWords, int maxWords,QTextEdit* out);
    void start();
    void stop();
    int botsCount;

signals:
    void connectingState(int count);
    void grabbingState(int count);
    void spammingState();
    void socketConnectedSignal();
    void messageGrabbed();

public slots:
    void socketConnected();
    void wordAdded();
    void spam();

};

#endif // SPAMSOCKETMANAGER_H
