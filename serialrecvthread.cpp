#include "serialrecvthread.h"
#include <QSerialPort>
#include <QMessageBox>

SerialRecvThread::SerialRecvThread(QObject *parent):QThread(parent)
{

}

SerialRecvThread::~SerialRecvThread()
{

}

void SerialRecvThread::getSerialInfo(const QString &portName,
                                  int baudRate,
                                  QSerialPort::DataBits dataBits,
                                  QSerialPort::Parity parity,
                                  QSerialPort::StopBits stopBits,
                                  QSerialPort::FlowControl flowControl)
{
    m_portName = portName;
    m_baudRate = baudRate;
    m_dataBits = dataBits;
    m_parity = parity;
    m_stopBits = stopBits;
    m_flowControl = flowControl;
}

void SerialRecvThread::openCloseSerial(bool openOrClose)
{
    //关闭串口
    openCloseFlag = openOrClose;
}

void SerialRecvThread::sendData(QByteArray data)
{
    m_senddata = data;
    m_senddata.append(13);
    m_senddata.append(10);

    if(openCloseFlag)
    {
        if(!isRunning())
            start();
    }
}

void SerialRecvThread::run()
{
    //实例化串口
    QSerialPort serial;

    //设置串口
    serial.setPortName(m_portName);
    serial.setBaudRate(m_baudRate);
    serial.setDataBits(m_dataBits);
    serial.setParity(m_parity);
    serial.setStopBits(m_stopBits);
    serial.setFlowControl(m_flowControl);

    //打开串口
    if(!serial.open(QIODevice::ReadWrite))
    {
        QMessageBox::about(NULL,"提示","无法连接设备!");
        return;
    }
    else
        serial.setDataTerminalReady(true);

    //关闭串口
    serial.close();

    //开启串口
    serial.open(QIODevice::ReadWrite);
    //发数据
    serial.write(m_senddata);

    serial.waitForBytesWritten(1000);

}
