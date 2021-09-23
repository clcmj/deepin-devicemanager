#include "XinputManager.h"

#include <QFile>
#include <QDebug>

XinputManager* XinputManager::s_Instance = nullptr;
XinputManager::XinputManager()
{

}

void XinputManager::update()
{
    updateXinputList();
    updateInputDevice();
}

void XinputManager::disableDevices(const QString& info)
{
    QStringList items = info.split("\n\n");
    foreach(const QString& item,items){
        disableDevice(item);
    }
}

void XinputManager::updateInputDevice()
{
    // 先清理已有数据
    m_MapFsToEvent.clear();

    // 读取/proc/bus/input/devices
    QFile file("/proc/bus/input/devices");
    if(!file.open(QIODevice::ReadOnly))
        return;
    QString info = file.readAll();
    file.close();

    // 解析 info
    QStringList infoList = info.split("\n\n");
    foreach(const QString& item,infoList){
        QString sysFS = "";
        QString event = "";
        if(parseItem(item,sysFS,event)){
//            qInfo() << event << " ** " << sysFS;
            m_MapFsToEvent.insert(sysFS,event);
        }
    }
}

bool XinputManager::parseItem(const QString& item,QString &sysFs,QString &event)
{
    QStringList lines = item.split("\n");
    foreach(const QString& line,lines){
        if(line.startsWith("S: Sysfs")){
            QRegExp reg(".*(usb[0-9]{1,2}/[0-9]{1,2}-[0-9]{1,2}/[0-9]{1,2}-[0-9]{1,2}\\.[0-9]{1,2}).*");
            if(reg.exactMatch(line)){
                sysFs = reg.cap(1);
            }
        }else if(line.startsWith("H: Handlers")){
            QRegExp reg(".*(event[0-9]{1,2}).*");
            if(reg.exactMatch(line)){
                event = reg.cap(1);
            }
        }else{
            continue;
        }
    }
    if(sysFs.isEmpty() || sysFs.isEmpty())
        return false;
    return true;
}

void XinputManager::updateXinputList()
{
    m_MapEventToID.clear();

    QProcess process;
    process.start("xinput --list");
    process.waitForFinished(-1);
    QString info =  process.readAllStandardOutput();
    QStringList lines = info.split("\n");
    foreach(const QString& line,lines){
        QRegExp reg(".*id=([0-9]{1,2}).*");
        if(!reg.exactMatch(line)){
            continue;
        }
        QString id = reg.cap(1);
        parseProps(id);
    }
    qInfo() << m_MapEventToID;
}

void XinputManager::parseProps(const QString& id)
{
    QProcess process;
    QString cmd = "xinput list-props "+id;
    process.start(cmd);
    process.waitForFinished(-1);
    QString info =  process.readAllStandardOutput();
    QStringList lines = info.split("\n");
    foreach(const QString& line,lines){
        QRegExp reg(".*Device Node.*/dev/input/(event[0-9]{1,2}).*");
        if(!reg.exactMatch(line)){
            continue;
        }
        QString event = reg.cap(1);
        m_MapEventToID.insert(event,id);
    }
}

void XinputManager::disableDevice(const QString& item)
{
    QMap<QString,QString> mapDisableInfo;
    mapDisableInfo.insert("Sc8I8IrSX63","input");
    mapDisableInfo.insert("a0ve4Q0u1MD","input");
    mapDisableInfo.insert("zluOvOTNF0E","input");

    QMap<QString,QString> mapInfo;
    getMapInfo(item,mapInfo);

    QRegExp regId(".*\\.(.*)");
    if(!regId.exactMatch(mapInfo["Unique ID"])){
        return;
    }

    if(mapDisableInfo.find(regId.cap(1)) == mapDisableInfo.end()){
        return;
    }

    QRegExp regEvent(".*(event[0-9]{1,2}).*");
    if(regEvent.exactMatch(mapInfo["Device File"]) || regEvent.exactMatch(mapInfo["Device Files"])){
        if(m_MapEventToID.find(regEvent.cap(1)) != m_MapEventToID.end()){
            qInfo() << "----------- " << regEvent.cap(1);
            qInfo() << "----------- " << m_MapEventToID[regEvent.cap(1)];
            QProcess process;
            QString cmd("xinput disable ");
            process.start(cmd+m_MapEventToID[regEvent.cap(1)]);
            process.waitForFinished(-1);
        }
    }else if(true){

    }



}

void XinputManager::getMapInfo(const QString& item,QMap<QString,QString>& mapInfo)
{
    QStringList lines = item.split("\n");
    foreach(const QString& line,lines){
        QStringList words = line.split(": ");
        if(words.size() != 2){
            continue;
        }
        mapInfo.insert(words[0].trimmed(),words[1].trimmed());
    }
}


