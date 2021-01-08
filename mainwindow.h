#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialrecvthread.h"
#define STOP "2"
#define Go_Stright "5"
#define Go_Left "1"
#define Go_Right "3"


QT_END_NAMESPACE
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void sendData(char * value);
    void serialRead();

signals:
    void keypressed(int);
    void keyreleased();

private slots:

    void on_searchButton_clicked();

    void on_openButton_clicked();

    void on_sendButton_clicked();

    void on_clearButton_clicked();

    void robotMove(int value);

private:
    Ui::MainWindow *ui;
    //实例化线程
    SerialRecvThread serialThread;
    QString portName;
    int baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;

    //QKeyEvent key_vent;
};
#endif // MAINWINDOW_H
