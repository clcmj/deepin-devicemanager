#ifndef PARSERLSCPU_H
#define PARSERLSCPU_H

#include <QFile>
#include <QMap>

class ParserLscpu
{
public:
    ParserLscpu();

    /**
     * @brief loadLscpuInfo
     */
    void loadLscpuInfo(QList<QMap<QString,QString>>& logicalCpu,QList<QList<QString>>& overView);

private:
    /**
     * @brief generatorLogicalCpu
     * @param mapInfo
     * @param logicalCpu
     */
    void generatorLogicalCpu(const QMap<QString,QString>& mapInfo,QList<QMap<QString,QString>>& logicalCpu,QList<QList<QString>>& overView);
private:
    int m_CpuNum;
    int m_CoreNum;
    int m_LogicalNum;
};

#endif // PARSERLSCPU_H
