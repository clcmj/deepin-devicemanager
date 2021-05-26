#include "CpuInfo.h"

#include <QFile>
#include <QDir>
#include <QDebug>

#include <sys/utsname.h>

CpuInfo::CpuInfo()
    : m_Arch("unknow")
{
}
CpuInfo::~CpuInfo()
{
    // clear physicalcpu
    m_MapPhysicalCpu.clear();
    // clear core cpu
    m_CoreCpu.clear();
    // clear logical cpu
    m_MapLogicalCpu.clear();
}

bool CpuInfo::loadCpuInfo()
{
    // read /sys/devices/system/cpu
    readSysCpu();

    // read the file /proc/cpuinfo
    if (!readProcCpuinfo())
        return false;

    // get arch
    readCpuArchitecture();
    return true;
}

QString CpuInfo::arch()
{
    return m_Arch;
}

void CpuInfo::logicalCpus(QString &info)
{
    foreach (int id, m_MapLogicalCpu.keys()) {
        LogicalCpu &cpu = m_MapLogicalCpu[id];
        info += QString("processor : %1\n").arg(cpu.logicalID());
        info += QString("core id : %1\n").arg(cpu.coreID());
        info += QString("physical id : %1\n").arg(cpu.physicalID());
        info += QString("L1d cache : %1").arg(cpu.l1dCache());
        info += QString("L1i cache : %1").arg(cpu.l1iCache());
        info += QString("L2 cache : %1").arg(cpu.l2Cache());
        info += QString("L3 cache : %1").arg(cpu.l3Cache());
        info += QString("CPU MHz : %1\n").arg(cpu.curFreq());
        info += QString("CPU max MHz : %1\n").arg(cpu.maxFreq());
        info += QString("CPU min MHz : %1\n").arg(cpu.minFreq());
        info += QString("flags : %1\n").arg(cpu.flags());
        info += QString("model : %1\n").arg(cpu.model());
        info += QString("model name : %1\n").arg(cpu.modelName());
        info += QString("vendor_id : %1\n").arg(cpu.vendor());
        info += QString("stepping : %1\n").arg(cpu.stepping());
        info += QString("cpu family : %1\n").arg(cpu.cpuFamliy());
        info += QString("bogomips : %1\n").arg(cpu.bogomips());
        info += QString("Architecture : %1\n").arg(m_Arch);
        info += QString("\n");
    }
}

void CpuInfo::readCpuArchitecture()
{
    struct utsname utsbuf;
    if (uname(&utsbuf) == -1)
        return;
    m_Arch = QString::fromLocal8Bit(utsbuf.machine);
}

bool CpuInfo::readProcCpuinfo()
{
    QFile file("/proc/cpuinfo");
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QString cpuInfo = file.readAll();
    QStringList infos = cpuInfo.split("\n\n");
    foreach (const QString &info, infos) {
        if (info.isEmpty())
            continue;
        parseInfo(info);
    }

    file.close();
    return true;
}

bool CpuInfo::parseInfo(const QString &info)
{
    if (info.isEmpty())
        return false;

    QMap<QString, QString> mapInfo;
    int physicalID = -1;
    QStringList lines = info.split("\n");
    foreach (const QString &line, lines) {
        if (line.isEmpty())
            continue;
        QStringList words = line.split(QRegExp("[\\s]*:[\\s]*"));
        if (words.size() != 2)
            continue;
        mapInfo.insert(words[0], words[1]);
        if (words[0].contains("processor"))
            physicalID = words[1].toInt();
    }

    if (m_MapLogicalCpu.find(physicalID) != m_MapLogicalCpu.end()) {
        LogicalCpu &lcpu = m_MapLogicalCpu[physicalID];
        lcpu.setFlags(mapInfo["flags"]);
        lcpu.setModel(mapInfo["model"]);
        lcpu.setModelName(mapInfo["model name"]);
        lcpu.setVendor(mapInfo["vendor_id"]);
        lcpu.setStepping(mapInfo["stepping"]);
        lcpu.setcpuFamily(mapInfo["cpu family"]);
        lcpu.setBogomips(mapInfo["bogomips"]);
    }
    return true;
}

void CpuInfo::readSysCpu()
{
    // /sys/devices/system/cpu/cpu*
    QDir dir("/sys/devices/system/cpu");
    dir.setFilter(QDir::Dirs);
    QRegExp reg("cpu([0-9]{1,4})");
    foreach (const QFileInfo &info, dir.entryInfoList()) {
        const QString &name = info.fileName();
        if (! reg.exactMatch(name))
            continue;
        readSysCpuN(reg.cap(1).toInt(), info.filePath());
    }
}

void CpuInfo::readSysCpuN(int N, const QString &path)
{
    LogicalCpu lcpu;
    QDir dir(path);

    // set logical id
    lcpu.setLogicalID(N);

    // get core id
    int core_id = readCoreID(dir);
    if (core_id > 0)
        lcpu.setCoreID(core_id);

    // get physical id
    int logical_id = readPhysicalID(dir);
    if (logical_id > -1)
        lcpu.setPhysicalID(logical_id);

    // get cpu cache
    if (dir.exists("cache"))
        readCpuCache(dir.filePath("cache"), lcpu);

    // get cpu freq
    if (dir.exists("cpufreq"))
        readCpuFreq(dir.filePath("cpufreq"), lcpu);

    m_MapLogicalCpu.insert(N, lcpu);
}

int CpuInfo::readPhysicalID(const QDir &dir)
{
    foreach (const QString &name, dir.entryList()) {
        QRegExp reg("node([0-9]{1,4})");
        if (!reg.exactMatch(name))
            continue;
        return reg.cap(1).toInt();
    }
    return -1;
}

int CpuInfo::readCoreID(const QDir &dir)
{
    if (!dir.exists("topology"))
        return -1;
    QString corePath = dir.filePath("topology") + "/core_id";
    QFile file(corePath);
    if (!file.open(QIODevice::ReadOnly))
        return -1;
    QString info = file.readAll();
    file.close();
    return info.toInt();
}

void CpuInfo::readCpuCache(const QString &path, LogicalCpu &lcpu)
{
    QDir dir(path);
    dir.setFilter(QDir::Dirs);
    foreach (const QFileInfo &fileInfo, dir.entryInfoList()) {
        QString tpath = fileInfo.absoluteFilePath();
        if (!tpath.contains(QRegExp("index[0-9]")))
            continue;
        readCpuCacheIndex(tpath, lcpu);
    }
}

void CpuInfo::readCpuCacheIndex(const QString &path, LogicalCpu &lcpu)
{
    int level = -1;
    QString type, value;

    // get level
    QString levelPath = path + "/level";
    QFile file(levelPath);
    if (file.open(QIODevice::ReadOnly)) {
        QString info = file.readAll();
        level = info.toInt();
    }
    file.close();

    // get type
    QString typePath = path + "/type";
    QFile fileT(typePath);
    if (fileT.open(QIODevice::ReadOnly)) {
        type = fileT.readAll();
    }
    fileT.close();

    // get size
    QString valuePath = path + "/size";
    QFile fileV(valuePath);
    if (fileV.open(QIODevice::ReadOnly)) {
        value = fileV.readAll();
    }
    fileV.close();

    if (level == 2) {
        lcpu.setL2Cache(value);
    } else if (level == 3) {
        lcpu.setL3Cache(value);
    } else if (level == 1) {
        if (type.contains("Data", Qt::CaseInsensitive))
            lcpu.setL1dCache(value);
        else
            lcpu.setL1iCache(value);
    }
}

void CpuInfo::readCpuFreq(const QString &path, LogicalCpu &lcpu)
{
    // min freq
    QString minFreqPath = path + "/cpuinfo_min_freq";
    QFile minFile(minFreqPath);
    if (minFile.open(QIODevice::ReadOnly)) {
        QString info = minFile.readAll();
        int value = info.toInt() / 1000;
        lcpu.setMinFreq(QString::number(value) + "MHz");
    }
    minFile.close();

    // cur freq
    QString curFreqPath = path + "/scaling_cur_freq";
    QFile curFile(curFreqPath);
    if (curFile.open(QIODevice::ReadOnly)) {
        QString info = curFile.readAll();
        int value = info.toInt() / 1000;
        lcpu.setCurFreq(QString::number(value) + "MHz");
    }
    curFile.close();

    // maxFreq
    QString maxFreqPath = path + "/cpuinfo_max_freq";
    QFile maxFile(maxFreqPath);
    if (maxFile.open(QIODevice::ReadOnly)) {
        QString info = maxFile.readAll();
        int value = info.toInt() / 1000;
        lcpu.setMaxFreq(QString::number(value) + "MHz");
    }
    maxFile.close();
}
