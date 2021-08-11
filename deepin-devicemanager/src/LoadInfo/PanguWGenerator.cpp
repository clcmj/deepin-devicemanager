#include "PanguWGenerator.h"
#include "DeviceManager.h"

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
