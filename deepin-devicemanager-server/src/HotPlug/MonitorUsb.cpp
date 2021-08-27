#include "MonitorUsb.h"

#include "QDebug"
#include <QDir>


MonitorUsb::MonitorUsb()
    : m_Udev(nullptr)
{
    m_Udev = udev_new();
    if (!m_Udev) {
        printf("error!!!\n");
    }
    // 创建一个新的monitor
    mon = udev_monitor_new_from_netlink(m_Udev, "udev");
    // 增加一个udev事件过滤器
    udev_monitor_filter_add_match_subsystem_devtype(mon, "usb", nullptr);
    // 启动监控
    udev_monitor_enable_receiving(mon);
    // 获取该监控的文件描述符，fd就代表了这个监控
    fd = udev_monitor_get_fd(mon);

    //
    QMap<QString,QDateTime> mapFileTime;
    getCurMapFileTime(mapFileTime);
    updateMemberMapFileTime(mapFileTime);
}

void MonitorUsb::monitor()
{
    char buf[10];
    fd_set fds;
    struct timeval tv;
    int ret;
    while (true) {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        tv.tv_sec = 0;
        tv.tv_usec = 10000;
        ret = select(fd + 1, &fds, nullptr, nullptr, &tv);

        // 判断是否有事件产生
        if (!ret)
            continue;
        if (! FD_ISSET(fd, &fds))
            continue;

        // 获取产生事件的设备映射
        struct udev_device *dev = udev_monitor_receive_device(mon);
        if (!dev)
            continue;

        // 获取事件并判断是否是插拔
        unsigned long long curNum = udev_device_get_devnum(dev);
        if (curNum <= 0) {
            udev_device_unref(dev);
            continue;
        }

        // 只有add和remove事件才会更新缓存信息
        strcpy(buf, udev_device_get_action(dev));
        if (0 == strcmp("add", buf) || 0 == strcmp("remove", buf)) {
            // 当监听到新的usb时，内核需要加载usb信息，而上层应用需要在内核处理之后获取信息
            // 为了确保缓存信息之前，内核已经处理完毕，先判断内核是否处理完信息，且判断时间不能多于10s
            qint64 begin = QDateTime::currentMSecsSinceEpoch();
            qint64 end = begin;
            while ((end - begin) <= 10000) {
                if(isUsbDevicesChanged())
                    break;
                sleep(1);
                end = QDateTime::currentMSecsSinceEpoch();
            }
            emit usbChanged();

            qInfo() << curNum << " ************ " << QDateTime::currentMSecsSinceEpoch() - begin;
        }

        udev_device_unref(dev);
    }
}

bool MonitorUsb::isUsbDevicesChanged()
{
    // 获取当前目录下的文件信息
    QMap<QString,QDateTime> mapFileTime;
    getCurMapFileTime(mapFileTime);

    // 先比较数量有没有变化，如果数量变化了则说明内核处理完成了
    if(mapFileTime.size() != m_MapFileTime.size()){
        updateMemberMapFileTime(mapFileTime);
        qInfo() << "m_MapFileTime 和 mapFileTime 的数量不一样 " ;
        return true;
    }

    // 如果数量一样，则判断时间是否最新
    foreach(const QString& file,mapFileTime.keys()){
        // 如果m_MapFileTime找不到file，则说明是新的USB设备
        if(m_MapFileTime.find(file) == m_MapFileTime.end()){
            updateMemberMapFileTime(mapFileTime);
            qInfo() << "m_MapFileTime 中找不到 " << file << "****************************** ";
            return true;
        }

        // 如果m_MapFileTime中能找到file，则比较时间。
        // 如果file的时间比m_MapFileTime中所有时间都新，则说明是新的usb设备
        const QDateTime& time = mapFileTime[file];
        bool largeThanAll = true; // time 比m_MapFileTime中的所有时间都大
        foreach(const QString& fileM,m_MapFileTime.keys()){
            if(time <= m_MapFileTime[fileM]){
                largeThanAll = false;
            }
        }

        if(largeThanAll){
            qInfo() << file << " 中的时间比 m_MapFileTime 中的时间都大 ******************************* ";
            updateMemberMapFileTime(mapFileTime);
            return true;
        }
    }
    return false;
}

void MonitorUsb::getCurMapFileTime(QMap<QString,QDateTime>& mapFileTime)
{
    QDir dir("/sys/bus/usb/devices");
    QFileInfoList lstInfo = dir.entryInfoList();
    foreach(const QFileInfo& info,lstInfo){
        mapFileTime.insert(info.fileName(),info.created());
    }
}

void MonitorUsb::updateMemberMapFileTime(const QMap<QString,QDateTime>& mapFileTime)
{
    m_MapFileTime.clear();
    m_MapFileTime = mapFileTime;
}

