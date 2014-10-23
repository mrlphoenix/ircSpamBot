#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectState(int count)
{

}

void MainWindow::grabState(int count)
{
    ui->grabLabel->setEnabled(true);
    ui->messagesPB->setEnabled(true);
    ui->messagesPB->setMaximum(count);
}

void MainWindow::spamState()
{
    ui->grabLabel->setEnabled(false);
    ui->messagesPB->setEnabled(false);
    ui->spamLabel->setEnabled(true);
}

void MainWindow::messageGrabbed()
{
    ui->messagesPB->setValue(ui->messagesPB->value()+1);
}

void MainWindow::on_pushButton_2_clicked()
{
    spamManager = new SpamSocketManager(ui->host->text(),ui->channel->text(),ui->accountFileName->text(),
                                        ui->globalCD->text().toInt(),ui->maxMessages->text().toInt(),
                                        ui->minW->text().toInt(),ui->maxW->text().toInt(),ui->textEdit);
    spamManager->start();
    connect(spamManager, SIGNAL(grabbingState(int)), SLOT(grabState(int)));
    connect(spamManager, SIGNAL(spammingState()),SLOT(spamState()));
    connect(spamManager, SIGNAL(messageGrabbed()),SLOT(messageGrabbed()));
}
