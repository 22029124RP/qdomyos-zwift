#ifndef TRIXTERXDREAMSERIAL_H
#define TRIXTERXDREAMSERIAL_H

#include <QMutex>
#include <QSerialPort>
#include <QThread>
#include <QWaitCondition>

/**
 * @brief A basic serial port monitoring thread.
 * Avoids using signals to prevent complications with objects, threads and timers.
 */
class trixterxdreamv1serial : public QThread {
    Q_OBJECT

public:
    explicit trixterxdreamv1serial(QObject *parent = nullptr);
    ~trixterxdreamv1serial();

    /**
     * @brief Opens the port.
     * @param portName The name of the serial port.
     * @param baudRate The baud rate.
     * @param waitTimeout The timeout for the serial port.
     */
    void open(const QString &portName, QSerialPort::BaudRate baudRate, int waitTimeout);

    /**
     * @brief Writes the array of bytes to the serial port
     * @param buffer The bytes to send.
     * @param info Debug information
     */
    void write(const QByteArray& buffer, QString info);

    void set_receiveBytes(std::function<void(const QByteArray& bytes)> value) { this->receiveBytes = value; }

    /**
     * @brief availablePorts Returns a list of information objects for the serial ports found in the system.
     */
    static QList<QSerialPortInfo> availablePorts();

protected:
    /**
     * @brief receive Override this to process received data.
     * @param bytes
     */
    virtual void receive(const QByteArray &bytes) { if(this->receiveBytes) this->receiveBytes(bytes); }

    /**
     * @brief error Log an error.
     * @param s The error text.
     */
    virtual void error(const QString &s) {}

  private:
    void run() override;

    QSerialPort serial;
    QString portName;
    QSerialPort::BaudRate baudRate;
    int waitTimeout = 1000;
    QMutex mutex;
    bool quitPending = false;
    std::function<void(const QByteArray& bytes)> receiveBytes=nullptr;
};

#endif // TRIXTERXDREAMSERIAL_H
