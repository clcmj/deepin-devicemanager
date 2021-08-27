#ifndef MONITORUSB_H
#define MONITORUSB_H
#include <libudev.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <QObject>
#include <QMap>
#include <QDateTime>

class MonitorUsb : public QObject
{
    Q_OBJECT
public:
    MonitorUsb();

    /**
     * @brief monitor
     */
    void monitor();

signals:
    /**
     * @brief usbChanged
     */
    void usbChanged();

private:
    // 判断/sys/bus/usb/devices目录下的信息是否发生变化
    bool isUsbDevicesChanged();
    void getCurMapFileTime(QMap<QString,QDateTime>& mapFD);
    void updateMemberMapFileTime(const QMap<QString,QDateTime>& mapFD);

private:
    struct udev                       *m_Udev;              //<! udev Environment
    struct udev_monitor               *mon;                 //<! object of mon
    int                               fd;                   //<! fd
    QMap<QString,QDateTime>           m_MapFileTime;        //<! 用于存放/sys/bus/usb/devices目录下文件的创建信息

};

#endif // MONITORUSB_H
