#include "PanguWGenerator.h"
#include "DeviceManager.h"
#include "../DeviceManager/DeviceNetwork.h"

PanguWGenerator::PanguWGenerator()
{

}

void PanguWGenerator::getDiskInfoFromLshw()
{
    {
        const QList<QMap<QString, QString>> lstDisk = DeviceManager::instance()->cmdInfo("lshw_disk");
        QList<QMap<QString, QString> >::const_iterator dIt = lstDisk.begin();
        for (; dIt != lstDisk.end(); ++dIt) {
            if ((*dIt).size() < 2) {
                continue;
            }
            DeviceManager::instance()->addLshwinfoIntoStorageDevice(*dIt);
        }
    }

    {
        const QList<QMap<QString, QString>> lstDisk = DeviceManager::instance()->cmdInfo("lshw_storage");
        QList<QMap<QString, QString> >::const_iterator dIt = lstDisk.begin();
        for (; dIt != lstDisk.end(); ++dIt) {
            if ((*dIt).size() < 2) {
                continue;
            }
            DeviceManager::instance()->addLshwinfoIntoStorageDevice(*dIt);
        }
    }
}

void PanguWGenerator::generatorNetworkDevice()
{
    const QList<QMap<QString, QString>> lstInfo = DeviceManager::instance()->cmdInfo("lshw_network");
    QList<QMap<QString, QString> >::const_iterator it = lstInfo.begin();
    for (; it != lstInfo.end(); ++it) {
        if ((*it).size() < 2) {
            continue;
        }

        // (eth[0-9]$)|(wlan[0-9]$)
        QRegExp reg("(eth[0-9]$)|(wlan[0-9]$)");
        if (!reg.exactMatch((*it)["logical name"])){
            continue;
        }

        // KLU的问题特殊处理
        QMap<QString, QString> tempMap;
        foreach (const QString &key, (*it).keys()) {
            tempMap.insert(key, (*it)[key]);
        }

        //if (tempMap["description"].contains("Wireless", Qt::CaseInsensitive) && tempMap["logical name"].contains("wlan0", Qt::CaseInsensitive)) {
        if ((*it)["description"].contains("Wireless", Qt::CaseInsensitive)) {
            continue;
        }
        DeviceNetwork device;
        device.setInfoFromLshw(*it);
        DeviceManager::instance()->addNetworkDevice(device);
    }
//    //wifiinfo
//    const QList<QMap<QString, QString>> lstWifiInfo = DeviceManager::instance()->cmdInfo("wifiinfo");
//    it = lstWifiInfo.begin();
//    for (; it != lstWifiInfo.end(); ++it) {
//        if ((*it).size() < 2) {
//            continue;
//        }
//        DeviceNetwork device;
//        device.setInfoFromWifiInfo(*it);
//        DeviceManager::instance()->addNetworkDevice(device);
//    }
}
