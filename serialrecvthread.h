#ifndef SERIALRECVTHREAD_H
#define SERIALRECVTHREAD_H

#include <QThread>
#include <QSerialPort>
class SerialRecvThread : public QThread
{
    Q_OBJECT
public:
    explicit SerialRecvThread(QObject *parent = nullptr);
    ~SerialRecvThread();

    void startThread(const QString &portName);
    void getSerialInfo(const QString &portName,
                    int baudRate,
                    QSerialPort::DataBits dataBits,
                    QSerialPort::Parity parity,
                    QSerialPort::StopBits stopBits,
                    QSerialPort::FlowControl flowControl);
    void openCloseSerial(bool openOrClose);
    void sendData(QByteArray data);

private:

    //run()用来接收数据
    void run() override;

    //主界面传来的串口参数信息
    QString m_portName;
    int m_baudRate;
    QSerialPort::DataBits m_dataBits;
    QSerialPort::Parity m_parity;
    QSerialPort::StopBits m_stopBits;
    QSerialPort::FlowControl m_flowControl;

    //要发送的数据
    QByteArray m_senddata;
    bool openCloseFlag;
};

#endif // SERIALRECVTHREAD_H
