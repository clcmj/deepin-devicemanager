#include "ParserLscpu.h"

ParserLscpu::ParserLscpu()
{

}

void ParserLscpu::loadLscpuInfo(QList<QMap<QString,QString>>& logicalCpu,QList<QList<QString>>& overView)
{
    QFile file("/home/liujun/Work/Project/DeviceManager/56011/device-info/lscpu.txt");
    if(!file.open(QIODevice::ReadOnly))
        return;
    QString info = file.readAll();
    QStringList lines = info.split("\n");
    QMap<QString,QString> commonInfo;
    QList<QMap<QString,QString>> lstCpu;
    bool isCommonInfo = true;
    foreach(const QString& line,lines){
        QStringList words = line.trimmed().split(": ");
        if(words.size() != 2)
            continue;
        if(words[0].trimmed() == "Model name"){
            QMap<QString,QString> last = commonInfo;
            lstCpu.append(last);
            isCommonInfo = false;
        }
        if(isCommonInfo){
            commonInfo.insert(words[0].trimmed(),words[1].trimmed());
        }
        else{
            QMap<QString,QString>& last = lstCpu.last();
            last.insert(words[0].trimmed(),words[1].trimmed());
        }
    }

    m_CpuNum = lstCpu.size();
    for (int i = 0; i < m_CpuNum; i++) {
        generatorLogicalCpu(lstCpu[i],logicalCpu,overView);
    }
}

void ParserLscpu::generatorLogicalCpu(const QMap<QString,QString>& mapInfo,QList<QMap<QString,QString>>& logicalCpu,QList<QList<QString>>& overView)
{
    int corePerSock = 1;
    if(mapInfo.find("Core(s) per socket") != mapInfo.end()){
        corePerSock = mapInfo["Core(s) per socket"].toInt();
    }
    int threadPerCore = 1;
    if(mapInfo.find("Thread(s) per core") != mapInfo.end()){
        threadPerCore = mapInfo["Thread(s) per core"].toInt();
    }

    int logicalNum = corePerSock*threadPerCore;
    m_CoreNum += corePerSock;
    m_LogicalNum += logicalNum;
    QList<QString> ov;
    ov.append(QString::number(corePerSock));
    ov.append(QString::number(logicalNum));
    ov.append(mapInfo["Model name"]);
    overView.append(ov);

    for (int i = 0; i < logicalNum; i++) {
        QMap<QString,QString> logicalInfo = mapInfo;
        int processor = logicalCpu.size();
        logicalInfo.insert("processor",QString::number(processor));
        logicalCpu.append(logicalInfo);
    }
}
