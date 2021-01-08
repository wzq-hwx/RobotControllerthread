#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QKeyEvent>
#include <serialrecvthread.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(this,&MainWindow::keypressed,this,&MainWindow::robotMove);

    //发送键失能
    ui->sendButton->setEnabled(false);
    ui->clearButton->setEnabled(false);
    ui->recvTextEdit->setEnabled(false);
    ui->sendTextEdit->setEnabled(false);

    //选择波特率菜单第一页
    ui->baudrateBox->setCurrentIndex(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_searchButton_clicked()
{
    ui ->portNameBox ->clear();
    //通过QSerialPortInfo查找可用串口
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
        ui ->portNameBox->addItem(info.portName());
}

void MainWindow::on_openButton_clicked()
{

    if(ui ->openButton ->text()==QString("启动")){
        //获取串口名
        portName = ui ->portNameBox ->currentText();

        //设置波特率
        baudRate = ui ->baudrateBox ->currentText().toInt();

        //设置数据位
        switch(ui ->dataBitsBox ->currentText().toInt())
        {
        case 8:
            dataBits = QSerialPort::Data8;break;
        default: break;
        }

        //设置奇偶校验
        switch(ui ->ParityBox ->currentIndex())
        {
        case 0: parity = QSerialPort::NoParity;break;
        default: break;
        }

        //设置停止位
        switch(ui ->stopBitsBox ->currentIndex())
        {
        case 1: stopBits = QSerialPort::OneStop;break;
        case 2: stopBits = QSerialPort::TwoStop;break;
        default: break;
        }

        //设置流控制
        flowControl = QSerialPort::NoFlowControl;

        //下拉菜单失效
        ui ->portNameBox ->setEnabled(false);
        ui ->baudrateBox ->setEnabled(false);
        ui ->dataBitsBox ->setEnabled(false);
        ui ->ParityBox ->setEnabled(false);
        ui ->stopBitsBox ->setEnabled(false);
        ui ->searchButton ->setEnabled(false);
        ui ->openButton ->setText("停止");
        //发送键使能
        ui->sendButton->setEnabled(false);
        //开启串口
        serialThread.openCloseSerial(true);

        //传递界面的设置信息到串口类
        serialThread.getSerialInfo(portName,
                                   baudRate,
                                   dataBits,
                                   parity,
                                   stopBits,
                                   flowControl);

        QPalette palette;
        ui->ledLabel->setAutoFillBackground(true);  //一定要这句，否则不行
        palette.setColor(QPalette::Background, QColor(0, 255, 0));
        ui->ledLabel->setPalette(palette);
        ui->connectstateLabel->setText("已连接");

    }
    else
    {
        //关闭串口
        serialThread .openCloseSerial(false);

        //下拉菜单使能
        ui ->portNameBox ->setEnabled(true);
        ui ->baudrateBox ->setEnabled(true);
        ui ->dataBitsBox ->setEnabled(true);
        ui ->ParityBox ->setEnabled(true);
        ui ->stopBitsBox ->setEnabled(true);
        ui ->searchButton ->setEnabled(true);
        ui ->openButton ->setText("启动");

        //发送键使能
        ui->sendButton->setEnabled(false);

        QPalette palette;
        ui->ledLabel->setAutoFillBackground(true);  //一定要这句，否则不行
        palette.setColor(QPalette::Background, QColor(255, 0, 0));
        ui->ledLabel->setPalette(palette);
        ui->connectstateLabel->setText("未连接");
    }
}

void MainWindow::on_sendButton_clicked()
{
    QByteArray data = ui ->sendTextEdit ->toPlainText().toUtf8();
    serialThread.sendData(data);
}

void MainWindow::on_clearButton_clicked()
{
    ui ->recvTextEdit ->clear();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{

    if(!event->isAutoRepeat())
    {
        switch(event ->key())
        {
        case Qt::Key_W: emit keypressed('W');break;
        case Qt::Key_A: emit keypressed('A');break;
        case Qt::Key_D: emit keypressed('D');break;
        default:break;
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat())
        if(event ->key() == Qt::Key_W||Qt::Key_A||Qt::Key_D)
        {
            robotMove(0);
        }
}

void MainWindow::robotMove(int value)
{

    switch(value)
    {
    case 'W':
        serialThread.sendData(Go_Stright);
        ui->statelabel->setText("前进");
        break;
    case 'A':
        serialThread.sendData(Go_Left);
        ui->statelabel->setText("左转");
        break;
    case 'D':
        serialThread.sendData(Go_Right);
        ui->statelabel->setText("右转");
        break;
    case 0:
        serialThread.sendData(STOP);
        ui->statelabel->setText("停止");
        break;
    default:break;
    }
}
