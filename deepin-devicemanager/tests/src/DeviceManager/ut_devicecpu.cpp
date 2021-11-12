/*
* Copyright (C) 2019 ~ 2020 UnionTech Software Technology Co.,Ltd
*
* Author:      zhangkai <zhangkai@uniontech.com>
* Maintainer:  zhangkai <zhangkai@uniontech.com>
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "../src/DeviceManager/DeviceCpu.h"

#include "../ut_Head.h"
#include <QCoreApplication>
#include <QPaintEvent>
#include <QPainter>

#include <gtest/gtest.h>
#include "../stub.h"

class DeviceCpu_UT : public UT_HEAD
{
public:
    void SetUp()
    {
        m_deviceCpu = new DeviceCpu;
    }
    void TearDown()
    {
        delete m_deviceCpu;
    }
    DeviceCpu *m_deviceCpu;
};

void setLscpuData(QMap<QString, QString> &mapLscpu)
{
    mapLscpu.insert("model name", "Intel(R) Core(TM) i3-9100F CPU @ 3.60GHz");
    mapLscpu.insert("vendor_id", "GenuineIntel");
    mapLscpu.insert("Thread(s) per core", "1");
    mapLscpu.insert("bogomips", "7200.00");
    mapLscpu.insert("Architecture", "x86_64");
    mapLscpu.insert("cpu family", "6");
    mapLscpu.insert("CPU MHz", "4085.639");
    mapLscpu.insert("model", "158");
    mapLscpu.insert("stepping", "10");
    mapLscpu.insert("L1d cache", "128 KiB");
    mapLscpu.insert("L1i cache", "128 KiB");
    mapLscpu.insert("L2 cache", "1 MiB");
    mapLscpu.insert("L3 cache", "6 MiB");
    mapLscpu.insert("Flags", "MMX SSE SSE2 SSE3 3D Now SSE4 SSSE3 SSE4_1 SSE4_2 AMD64 EM64T");
    mapLscpu.insert("flags", "MMX SSE SSE2 SSE3 3D Now SSE4 SSSE3 SSE4_1 SSE4_2 AMD64 EM64T");
    mapLscpu.insert("Virtualization", "VT-x");
    mapLscpu.insert("processor", "0");
    mapLscpu.insert("core id", "0");
}

void setLshwData(QMap<QString, QString> &mapLshw)
{
    mapLshw.insert("version", "Intel(R) Core(TM) i3-9100F CPU @ 3.60GHz");
    mapLshw.insert("vendor", "Intel Corp.");
}

void setDmidecodeData(QMap<QString, QString> &mapDmidecode)
{
    mapDmidecode.insert("product", "Intel(R) Core(TM) i3-9100F CPU @ 3.60GHz");
    mapDmidecode.insert("Manufacturer", "Intel(R) Corporation");
    mapDmidecode.insert("Max Speed", "8300 MHz");
    mapDmidecode.insert("Family", "Core i3");
}

TEST_F(DeviceCpu_UT, DeviceCpu_UT_setCpuInfo)
{
    QMap<QString, QString> mapLscpu;
    QMap<QString, QString> mapLshw;
    QMap<QString, QString> mapDmidecode;

    setLscpuData(mapLscpu);
    setLshwData(mapLshw);
    setDmidecodeData(mapDmidecode);

    m_deviceCpu->setCpuInfo(mapLscpu, mapLshw, mapDmidecode, 4, 4);

    EXPECT_STREQ("Intel(R) Core(TM) i3-9100F CPU @ 3.60GHz", m_deviceCpu->m_Name.toStdString().c_str());
    EXPECT_EQ(4, m_deviceCpu->m_LogicalCPUNum);
    EXPECT_EQ(4, m_deviceCpu->m_CPUCoreNum);

}

TEST_F(DeviceCpu_UT, DeviceCpu_UT_initFilterKey)
{
    m_deviceCpu->initFilterKey();
    m_deviceCpu->getOverviewInfo();
    m_deviceCpu->loadBaseDeviceInfo();
    m_deviceCpu->loadOtherDeviceInfo();
    m_deviceCpu->loadTableHeader();
    m_deviceCpu->loadTableData();
}

TEST_F(DeviceCpu_UT, DeviceCpu_UT_name)
{
    m_deviceCpu->name();
    m_deviceCpu->driver();
    m_deviceCpu->subTitle();
    ASSERT_FALSE(m_deviceCpu->frequencyIsRange());
}

TEST_F(DeviceCpu_UT, DeviceCpu_UT_setInfoFromLscpu)
{
    QMap<QString, QString> mapLscpu;
    QMap<QString, QString> mapLshw;
    QMap<QString, QString> mapDmidecode;
    QMap<QString, QString> catInfo;
    mapLscpu.insert("Vendor", "1@n");
    mapLshw.insert("Version", "1.0");
    mapDmidecode.insert("Vendor", "test");
    catInfo.insert("Version", "2.0");
    m_deviceCpu->setInfoFromLscpu(mapLscpu);
}

TEST_F(DeviceCpu_UT, DeviceCpu_UT_setInfoFromLshw)
{
    QMap<QString, QString> mapLscpu;
    QMap<QString, QString> mapLshw;
    QMap<QString, QString> mapDmidecode;
    QMap<QString, QString> catInfo;
    mapLscpu.insert("Vendor", "1@n");
    mapLshw.insert("Version", "1.0");
    mapDmidecode.insert("Vendor", "test");
    catInfo.insert("Version", "2.0");
    m_deviceCpu->setInfoFromLshw(mapLshw);
}

TEST_F(DeviceCpu_UT, DeviceCpu_UT_setInfoFromDmidecode)
{
    QMap<QString, QString> mapLscpu;
    QMap<QString, QString> mapLshw;
    QMap<QString, QString> mapDmidecode;
    QMap<QString, QString> catInfo;
    mapLscpu.insert("Vendor", "1@n");
    mapLshw.insert("Version", "1.0");
    mapDmidecode.insert("Name", "Loongson");
    catInfo.insert("Version", "2.0");
    m_deviceCpu->setInfoFromDmidecode(mapDmidecode);
}

TEST_F(DeviceCpu_UT, DeviceCpu_UT_getTrNumber)
{
    m_deviceCpu->getTrNumber();
}
