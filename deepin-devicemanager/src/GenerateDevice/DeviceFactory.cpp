// 项目自身文件
#include "DeviceFactory.h"

// Qt库文件
#include <QProcess>
#include <QFile>

// 其它头文件
#include "X86Generator.h"
#include "MipsGenerator.h"
#include "ArmGenerator.h"
#include "KLUGenerator.h"
#include "PanguGenerator.h"
#include "PanguVGenerator.h"
#include "../commondefine.h"
#include "XmlGenerator.h"

QString DeviceFactory::s_GeneratorKey = "";

DeviceFactory::DeviceFactory()
{

}

DeviceGenerator *DeviceFactory::getDeviceGenerator()
{
    // 根据架构创建设备信息生成器
    DeviceGenerator *generator = nullptr;
    generator = new XmlGenerator(s_GeneratorKey);
    return generator;
}

void DeviceFactory::setGeneratorKey(const QString &key)
{
    // 设置机器架构
    s_GeneratorKey = key;
}
