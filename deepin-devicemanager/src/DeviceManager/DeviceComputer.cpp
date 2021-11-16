// 项目自身文件
#include "DeviceComputer.h"
#include "DeviceManager.h"

// Qt库文件
#include <QDebug>

DeviceComputer::DeviceComputer()
    : m_HomeUrl("")
    , m_OsDescription("")
    , m_OS("")
    , m_Vendor("")
    , m_Name("")
    , m_Type("")
    , m_Driver("")
{

}

const QString &DeviceComputer::name() const
{
    return m_Name;
}

const QString &DeviceComputer::driver() const
{
    return m_Driver;
}

void DeviceComputer::setHomeUrl(const QString &value)
{
    // 设置主页网站
    m_HomeUrl = value;
}

void DeviceComputer::setOsDescription(const QString &value)
{
    // 设置操作系统描述
    m_OsDescription = value;
}

void DeviceComputer::setOS(const QString &value)
{
    // 设置操作系统
    m_OS = value;
}

void DeviceComputer::setVendor(const QString &value)
{
    // 设置制造商
    m_Vendor = value;
}

void DeviceComputer::setName(const QString &value)
{
    // 设置计算机名称
    m_Name = value;
    if (m_Name.contains("None", Qt::CaseInsensitive))
        m_Name = "";
}

void DeviceComputer::setType(const QString &value)
{
    // 设置设备类型
    m_Type = value;
}

void DeviceComputer::setVendor(const QString &dm1Vendor, const QString &dm2Vendor)
{
    // 设置制造商
    if (dm1Vendor.contains("System manufacturer"))
        m_Vendor = dm2Vendor;
    else
        m_Vendor = dm1Vendor;
}

void DeviceComputer::setName(const QString &dm1Name, const QString &dm2Name, const QString &dm1Family, const QString &dm1Version)
{
    // name
    QString pname;
    if (dm1Name.contains("System Product Name"))
        pname = dm2Name;
    else
        pname = dm1Name;

    // family
    QString family = dm1Family;
    if (dm1Family.contains("unknown", Qt::CaseInsensitive) \
            || dm1Family.contains("System Version") \
            || dm1Family.contains("Not Specified", Qt::CaseInsensitive) \
            || dm1Family.contains("x.x", Qt::CaseInsensitive) \
            || dm1Family.contains("Not Applicable", Qt::CaseInsensitive)) {
        family = "";
    }

    // version
    QString version = dm1Version;
    if (dm1Version.contains("unknown", Qt::CaseInsensitive) \
            || dm1Version.contains("System Version") \
            || dm1Version.contains("Not Specified", Qt::CaseInsensitive) \
            || dm1Version.contains("x.x", Qt::CaseInsensitive) \
            || dm1Version.contains("Not Applicable", Qt::CaseInsensitive)) {
        version = "";
    }

    // 去除 name,vendor,family中的重复字段.再拼接
    pname = pname.remove(m_Vendor, Qt::CaseInsensitive);
    pname = pname.remove(version, Qt::CaseInsensitive);
    version = version.remove(m_Vendor, Qt::CaseInsensitive);
    version = version.remove(family, Qt::CaseInsensitive);
    family = family.remove(m_Vendor, Qt::CaseInsensitive);
    family = family.remove(version, Qt::CaseInsensitive);

    m_Name = family + " " + version + " " + pname;
}

const QString DeviceComputer::getOverviewInfo()
{
    // 获取概况信息
    QString model;
    model += m_Vendor + QString(" ");
    model += m_Name + QString(" ");
    model += m_Type + QString(" ");

    return model;
}

const QString DeviceComputer::getOSInfo()
{
    return m_OsDescription + " " + m_OS;
}

const QMap<QString, QString> &DeviceComputer::getHWCloudInfo()
{
    return m_CloudMap;
}

void DeviceComputer::getInfoFromEnv()
{
    QFile file("/usr/local/vdi/base.env");
    if (file.open(QIODevice::ReadOnly)) {
        QString info = file.readAll();
        file.close();

        QStringList lines = info.split("\n");
        foreach (QString line, lines) {
            if (line.isEmpty())
                continue;
            line.replace("export ", "");
            QStringList words = line.split("=");
            if (words.size() != 2)
                continue;
            QString key = words[0];
            if (words[0] == "SW_IMG_VERSION") {
                key = tr("SW_IMG_VERSION");        // 整机系统版本号
                m_CloudMap.insert(key, words[1]);
            } else if (words[0] == "SW_HARDWARE_MODEL") {
                key = tr("SW_HARDWARE_MODEL");     // 整机产品名称
                m_CloudMap.insert(key, words[1]);
            } else {
                continue;
            }
        }
    }
}

void DeviceComputer::getInfoFromDmi()
{
    // get SYSTEM_VERSION  /usr/share/version/VersionInfo
    QFile file("/usr/share/version/VersionInfo");
    if (file.open(QIODevice::ReadOnly)) {
        QString info = file.readAll();
        file.close();

        QStringList lines = info.split("\n");
        foreach (QString line, lines) {
            if (line.isEmpty())
                continue;
            QStringList words = line.split("=");
            if (words.size() != 2)
                continue;
            QString key = words[0];
            if (key == "Version") {
                key = tr("SYSTEM_VERSION");   // 整机系统版本号
                m_CloudMap.insert(key, words[1]);
            }
        }
    }

    // get TERMINAL_MODEL
    const QList<QMap<QString, QString>> &lstInfo = DeviceManager::instance()->cmdInfo("dmidecode1");
    if (lstInfo.size() <= 0)
        return;

    const QMap<QString, QString> &mapt = lstInfo[0];
    if (mapt.find("SKU Number") == mapt.end())
        return;

    QString info = mapt["SKU Number"];
    if (!info.contains("|"))
        return;

    QStringList lst = info.split("|");
    if (lst.size() != 2)
        return;

    m_CloudMap.insert(tr("SERIAL_NUMBER"), lst[0]);    // 整机序列号
    m_CloudMap.insert(tr("TERMINAL_MODEL"), lst[1]);   // 整机产品名称
}

void DeviceComputer::initFilterKey()
{

}

void DeviceComputer::loadBaseDeviceInfo()
{
    // 添加基本信息
    addBaseDeviceInfo(tr("Name"), m_Name);
}

void DeviceComputer::loadOtherDeviceInfo()
{
    // 将QMap<QString, QString>内容转存为QList<QPair<QString, QString>>
    mapInfoToList();
}

void DeviceComputer::loadTableData()
{

}
