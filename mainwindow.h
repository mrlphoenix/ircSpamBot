#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <spamsocketmanager.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SpamSocketManager * spamManager;
public slots:
    void connectState(int count);
    void grabState(int count);
    void spamState();
    void messageGrabbed();
private slots:
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H
