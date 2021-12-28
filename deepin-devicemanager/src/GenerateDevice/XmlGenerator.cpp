#include "XmlGenerator.h"

#include "X86Generator.h"
#include "MipsGenerator.h"
#include "ArmGenerator.h"
#include "KLUGenerator.h"
#include "PanguGenerator.h"
#include "PanguVGenerator.h"
#include "../commondefine.h"

#include "XmlHandler.h"
#include "DeviceManager.h"
#include "DeviceCpu.h"
#include "DeviceGpu.h"
#include "DeviceStorage.h"
#include "DeviceMemory.h"
#include "DeviceMonitor.h"
#include "DeviceBios.h"
#include "DeviceAudio.h"
#include "DeviceBluetooth.h"
#include "DeviceNetwork.h"
#include "DeviceImage.h"
#include "DeviceOthers.h"
#include "DeviceComputer.h"
#include "DevicePower.h"
#include "DeviceCdrom.h"
#include "DevicePrint.h"
#include "DeviceInput.h"

#include <QFile>

XmlGenerator::XmlGenerator()
{

}

XmlGenerator::XmlGenerator(QString strArch)
{
    if (!dg) {
        if (strArch == "x86_64")
            dg = new X86Generator();
        else if (strArch == "mips64")
            dg = new MipsGenerator();
        else if (strArch == "aarch64")
            dg = new ArmGenerator();
        else if (strArch == "KLU")
            dg = new KLUGenerator();
        else if (strArch == "PanGu")
            dg = new PanguGenerator();
        else if (strArch == "PanGuV")
            dg = new PanguVGenerator();
        else
            dg = new X86Generator();
    }
}


void XmlGenerator::XmlGenerator::generatorBiosDevice()
{
    // 生成BIOS
    getBiosInfo();
    getSystemInfo();
    getBaseBoardInfo();
    getChassisInfo();
    getBiosMemoryInfo();
}

void XmlGenerator::XmlGenerator::getBiosInfo()
{
    QMap<QString, QString> mapXmlInfo;
    XmlHandler::instance()->GetXmlInfo("BIOS Information", mapXmlInfo);
    // 获取BIOS信息
    const QList<QMap<QString, QString>> &lstInfo = DeviceManager::instance()->cmdInfo("dmidecode0");
    QList<QMap<QString, QString> >::const_iterator it = lstInfo.begin();
    for (; it != lstInfo.end(); ++it) {
        if ((*it).size() < 2)
            continue;

        DeviceBios *device = new DeviceBios();
        device->setBiosInfo(*it);
        if(!mapXmlInfo.isEmpty()){
            device->setInfoFromXml(mapXmlInfo);
        }
        DeviceManager::instance()->addBiosDevice(device);
    }

    const QList<QMap<QString, QString>> &lanInfo = DeviceManager::instance()->cmdInfo("dmidecode13");
    QList<QMap<QString, QString> >::const_iterator iter = lanInfo.begin();
    for (; iter != lanInfo.end(); ++iter) {
        if ((*iter).size() < 2)
            continue;

        DeviceManager::instance()->setLanguageInfo(*iter);
    }
    return;
}
void XmlGenerator::XmlGenerator::getSystemInfo()
{
    QMap<QString, QString> mapXmlInfo;
    XmlHandler::instance()->GetXmlInfo("System Information", mapXmlInfo);
    // 获取系统信息
    const QList<QMap<QString, QString>> &lstInfo = DeviceManager::instance()->cmdInfo("dmidecode1");
    QList<QMap<QString, QString> >::const_iterator it = lstInfo.begin();
    for (; it != lstInfo.end(); ++it) {
        if ((*it).size() < 2)
            continue;

        DeviceBios *device = new DeviceBios();
        device->setSystemInfo(*it);
        if(!mapXmlInfo.isEmpty()){
            device->setInfoFromXml(mapXmlInfo);
        }
        DeviceManager::instance()->addBiosDevice(device);
    }
    return;
}
void XmlGenerator::XmlGenerator::getBaseBoardInfo()
{
    QMap<QString, QString> mapXmlInfo;
    XmlHandler::instance()->GetXmlInfo("Base Board Information", mapXmlInfo);
    // 获取主板信息
    const QList<QMap<QString, QString>> &lstInfo = DeviceManager::instance()->cmdInfo("dmidecode2");
    QList<QMap<QString, QString> >::const_iterator it = lstInfo.begin();
    for (; it != lstInfo.end(); ++it) {
        if ((*it).size() < 2)
            continue;

        DeviceBios *device = new DeviceBios();
        device->setBaseBoardInfo(*it);
        if(!mapXmlInfo.isEmpty()){
            device->setInfoFromXml(mapXmlInfo);
        }
        DeviceManager::instance()->addBiosDevice(device);
    }
    return;
}
void XmlGenerator::XmlGenerator::getChassisInfo()
{
    QMap<QString, QString> mapXmlInfo;
    XmlHandler::instance()->GetXmlInfo("Chassis Information", mapXmlInfo);
    // 获取机箱信息
    const QList<QMap<QString, QString>> &lstInfo = DeviceManager::instance()->cmdInfo("dmidecode3");
    QList<QMap<QString, QString> >::const_iterator it = lstInfo.begin();
    for (; it != lstInfo.end(); ++it) {
        if ((*it).size() < 2)
            continue;

        DeviceBios *device = new DeviceBios();
        device->setChassisInfo(*it);
        if(!mapXmlInfo.isEmpty()){
            device->setInfoFromXml(mapXmlInfo);
        }
        DeviceManager::instance()->addBiosDevice(device);
    }
    return;
}
void XmlGenerator::XmlGenerator::getBiosMemoryInfo()
{
    QMap<QString, QString> mapXmlInfo;
    XmlHandler::instance()->GetXmlInfo("Physical Memory Array", mapXmlInfo);
    // 获取内存插槽信息
    const QList<QMap<QString, QString>> &lstInfo = DeviceManager::instance()->cmdInfo("dmidecode16");
    QList<QMap<QString, QString> >::const_iterator it = lstInfo.begin();
    for (; it != lstInfo.end(); ++it) {
        if ((*it).size() < 2)
            continue;

        DeviceBios *device = new DeviceBios();
        device->setMemoryInfo(*it);
        if(!mapXmlInfo.isEmpty()){
            device->setInfoFromXml(mapXmlInfo);
        }
        DeviceManager::instance()->addBiosDevice(device);
    }
    return;
}

void XmlGenerator::XmlGenerator::generatorCpuDevice()
{
    dg->generatorCpuDevice();

    QMap<QString, QString> mapXmlInfo;
    XmlHandler::instance()->GetXmlInfo("CPU", mapXmlInfo);
    QList<DeviceBaseInfo*> lstCpu = DeviceManager::instance()->getCpuList();
    DeviceCpu *dc = nullptr;
    foreach(DeviceBaseInfo * d,lstCpu){
        dc = dynamic_cast<DeviceCpu*>(d);

        dc->setInfoFromLscpu(mapXmlInfo);//tmp

        //dc->setInfoFromXml(mapXmlInfo);
    }
}
void XmlGenerator::XmlGenerator::generatorMemoryDevice()
{
    dg->generatorMemoryDevice();
    QList<QMap<QString, QString> > lstXmlInfo;
    QString strKey = "slot";
    int i = 0;
    while (true) {
        QMap<QString, QString> mXmlInfo;
        XmlHandler::instance()->GetXmlInfo(QString("Memory ") + QString::number(i,10), mXmlInfo);
        i++;
        if(mXmlInfo.isEmpty()){
            break;
        }
        if(!mXmlInfo[strKey].isEmpty()){
            lstXmlInfo.append(mXmlInfo);
        }
    }
    if(lstXmlInfo.length() < 1){
        return;
    }

    QList<DeviceBaseInfo*> lstMem = DeviceManager::instance()->getMemoryList();
    DeviceMemory *dm = nullptr;
    for (int i = 0; i < lstMem.length(); i++) {
        dm = dynamic_cast<DeviceMemory*>(lstMem[i]);
        if(dm){
            dm->setInfoFromXml(strKey,lstXmlInfo);
        }
    }


}

/**
 * @brief generatorGpuDevice:生成显卡信息
 */
void XmlGenerator::generatorGpuDevice()
{
    dg->generatorGpuDevice();
    QList<QMap<QString, QString> > lstXmlInfo;
    QString strKey = "bus info";
    int i = 0;
    while (true) {
        QMap<QString, QString> mXmlInfo;
        XmlHandler::instance()->GetXmlInfo(QString("GPU ") + QString::number(i,10), mXmlInfo);
        i++;
        if(mXmlInfo.isEmpty()){
            break;
        }
        if(!mXmlInfo[strKey].isEmpty()){
            lstXmlInfo.append(mXmlInfo);
        }
    }
    if(lstXmlInfo.length() < 1){
        return;
    }

    QList<DeviceBaseInfo*> lstDev = DeviceManager::instance()->getGpuList();
    DeviceGpu *dg = nullptr;
    for (int i = 0; i < lstDev.length(); i++) {
        dg = dynamic_cast<DeviceGpu*>(lstDev[i]);
        if(dg){
            dg->setInfoFromXml(strKey,lstXmlInfo);
        }
    }
}

/**
 * @brief generatorDiskDevice:生成存储设备信息
 */
void XmlGenerator::generatorDiskDevice()
{
    dg->generatorDiskDevice();
    QList<QMap<QString, QString> > lstXmlInfo;
    QString strKey = "Serial ID";
    int i = 0;
    while (true) {
        QMap<QString, QString> mXmlInfo;
        XmlHandler::instance()->GetXmlInfo(QString("DISK ") + QString::number(i,10), mXmlInfo);
        i++;
        if(mXmlInfo.isEmpty()){
            break;
        }
        if(!mXmlInfo[strKey].isEmpty()){
            lstXmlInfo.append(mXmlInfo);
        }
    }
    if(lstXmlInfo.length() < 1){
        return;
    }

    QList<DeviceBaseInfo*> lstDev = DeviceManager::instance()->getStorageList();
    DeviceStorage *ds = nullptr;
    for (int i = 0; i < lstDev.length(); i++) {
        ds = dynamic_cast<DeviceStorage*>(lstDev[i]);
        if(ds){
            ds->setInfoFromXml(strKey,lstXmlInfo);
        }
    }
}

/**
 * @brief generatorMonitorDevice:生成显示设备
 */
void XmlGenerator::generatorMonitorDevice()
{
    dg->generatorMonitorDevice();

    QList<QMap<QString, QString> > lstXmlInfo;
    QString strKey = "SerialNumber";
    int i = 0;
    while (true) {
        QMap<QString, QString> mXmlInfo;
        XmlHandler::instance()->GetXmlInfo(QString("Monitor ") + QString::number(i,10), mXmlInfo);
        i++;
        if(mXmlInfo.isEmpty()){
            break;
        }
        if(!mXmlInfo[strKey].isEmpty()){
            lstXmlInfo.append(mXmlInfo);
        }
    }
    if(lstXmlInfo.length() < 1){
        return;
    }

    QList<DeviceBaseInfo*> lstDev = DeviceManager::instance()->getMonitorList();
    DeviceMonitor *ds = nullptr;
    for (int i = 0; i < lstDev.length(); i++) {
        ds = dynamic_cast<DeviceMonitor*>(lstDev[i]);
        if(ds){
            ds->setInfoFromXml(strKey,lstXmlInfo);
        }
    }
}

/**
 * @brief generatorNetworkDevice:生成网卡
 */
void XmlGenerator::generatorNetworkDevice()
{
    dg->generatorNetworkDevice();
    QList<QMap<QString, QString> > lstXmlInfo;
    QString strKey = "BusInfo";
    int i = 0;
    while (true) {
        QMap<QString, QString> mXmlInfo;
        XmlHandler::instance()->GetXmlInfo(QString("Network ") + QString::number(i,10), mXmlInfo);
        i++;
        if(mXmlInfo.isEmpty()){
            break;
        }
        if(!mXmlInfo[strKey].isEmpty()){
            lstXmlInfo.append(mXmlInfo);
        }
    }
    if(lstXmlInfo.length() < 1){
        return;
    }

    QList<DeviceBaseInfo*> lstDev = DeviceManager::instance()->getNetworkList();
    DeviceNetwork *ds = nullptr;
    for (int i = 0; i < lstDev.length(); i++) {
        ds = dynamic_cast<DeviceNetwork*>(lstDev[i]);
        if(ds){
            ds->setInfoFromXml(strKey,lstXmlInfo);
        }
    }
}

/**
 * @brief generatorAudioDevice:生成音频适配器
 */
void XmlGenerator::generatorAudioDevice()
{
    dg->generatorAudioDevice();
    QList<QMap<QString, QString> > lstXmlInfo;
    QString strKey = "SysFS BusID";
    int i = 0;
    while (true) {
        QMap<QString, QString> mXmlInfo;
        XmlHandler::instance()->GetXmlInfo(QString("AUDIO ") + QString::number(i,10), mXmlInfo);
        i++;
        if(mXmlInfo.isEmpty()){
            break;
        }
        if(!mXmlInfo[strKey].isEmpty()){
            lstXmlInfo.append(mXmlInfo);
        }
    }
    if(lstXmlInfo.length() < 1){
        return;
    }

    QList<DeviceBaseInfo*> lstDev = DeviceManager::instance()->getAudioList();
    DeviceAudio *da = nullptr;
    for (int i = 0; i < lstDev.length(); i++) {
        da = dynamic_cast<DeviceAudio*>(lstDev[i]);
        if(da){
            da->setInfoFromXml(strKey,lstXmlInfo);
        }
    }
}

/**
 * @brief generatorBluetoothDevice:生成蓝牙设备
 */
void XmlGenerator::generatorBluetoothDevice()
{
    dg->generatorBluetoothDevice();
    QList<QMap<QString, QString> > lstXmlInfo;
    QString strKey = "BusInfo";
    int i = 0;
    while (true) {
        QMap<QString, QString> mXmlInfo;
        XmlHandler::instance()->GetXmlInfo(QString("BLUETOOTH ") + QString::number(i,10), mXmlInfo);
        i++;
        if(mXmlInfo.isEmpty()){
            break;
        }
        if(!mXmlInfo[strKey].isEmpty()){
            lstXmlInfo.append(mXmlInfo);
        }
    }
    if(lstXmlInfo.length() < 1){
        return;
    }

    QList<DeviceBaseInfo*> lstDev = DeviceManager::instance()->getBluetoothList();
    DeviceBluetooth *d = nullptr;
    for (int i = 0; i < lstDev.length(); i++) {
        d = dynamic_cast<DeviceBluetooth*>(lstDev[i]);
        if(d){
            d->setInfoFromXml(strKey,lstXmlInfo);
        }
    }
}

/**
 * @brief generatorKeyboardDevice:生成键盘设备
 */
void XmlGenerator::generatorKeyboardDevice()
{
    dg->generatorKeyboardDevice();
    QList<QMap<QString, QString> > lstXmlInfo;
    QString strKey;
    int i = 0;
    while (true) {
        QMap<QString, QString> mXmlInfo;
        XmlHandler::instance()->GetXmlInfo(QString("Input ") + QString::number(i,10), mXmlInfo);
        i++;
        if(mXmlInfo.isEmpty()){
            break;
        }
        if(!mXmlInfo["BusInfo"].isEmpty()){
            strKey = "BusInfo";
        }
        else if (!mXmlInfo["keysToPairedDevice"].isEmpty()){
            strKey = "keysToPairedDevice";
        }
        else if (!mXmlInfo["KeysToCatDevices"].isEmpty()){
            strKey = "KeysToCatDevices";
        }
        else {
            return;
        }

        lstXmlInfo.append(mXmlInfo);
    }
    if(lstXmlInfo.length() < 1){
        return;
    }

    QList<DeviceBaseInfo*> lstDev = DeviceManager::instance()->getKeyboardList();
    DeviceInput *d = nullptr;
    for (int i = 0; i < lstDev.length(); i++) {
        d = dynamic_cast<DeviceInput*>(lstDev[i]);
        if(d){
            d->setInfoFromXml(strKey,lstXmlInfo);
        }
    }
}

/**
 * @brief generatorMouseDevice:生成鼠标设备
 */
void XmlGenerator::generatorMouseDevice()
{
    dg->generatorMouseDevice();
    QList<QMap<QString, QString> > lstXmlInfo;
    QString strKey;
    int i = 0;
    while (true) {
        QMap<QString, QString> mXmlInfo;
        XmlHandler::instance()->GetXmlInfo(QString("Input ") + QString::number(i,10), mXmlInfo);
        i++;
        if(mXmlInfo.isEmpty()){
            break;
        }
        if(!mXmlInfo["BusInfo"].isEmpty()){
            strKey = "BusInfo";
        }
        else if (!mXmlInfo["keysToPairedDevice"].isEmpty()){
            strKey = "keysToPairedDevice";
        }
        else {
            return;
        }

        lstXmlInfo.append(mXmlInfo);
    }
    if(lstXmlInfo.length() < 1){
        return;
    }

    QList<DeviceBaseInfo*> lstDev = DeviceManager::instance()->getMouseList();
    DeviceInput *d = nullptr;
    for (int i = 0; i < lstDev.length(); i++) {
        d = dynamic_cast<DeviceInput*>(lstDev[i]);
        if(d){
            d->setInfoFromXml(strKey,lstXmlInfo);
        }
    }
}

/**
 * @brief generatorPrinterDevice:生成打印机设备
 */
void XmlGenerator::generatorPrinterDevice()
{
    dg->generatorPrinterDevice();
    QList<QMap<QString, QString> > lstXmlInfo;
    QString strKey;
    int i = 0;
    while (true) {
        QMap<QString, QString> mXmlInfo;
        XmlHandler::instance()->GetXmlInfo(QString("Print ") + QString::number(i,10), mXmlInfo);
        i++;
        if(mXmlInfo.isEmpty()){
            break;
        }
        if(!mXmlInfo["SerialNumber"].isEmpty()){
            strKey = "SerialNumber";
        }
        else if (!mXmlInfo["URI"].isEmpty()){
            strKey = "URI";
        }
        else {
            return;
        }

        lstXmlInfo.append(mXmlInfo);
    }
    if(lstXmlInfo.length() < 1){
        return;
    }

    QList<DeviceBaseInfo*> lstDev = DeviceManager::instance()->getPrintList();
    DevicePrint *d = nullptr;
    for (int i = 0; i < lstDev.length(); i++) {
        d = dynamic_cast<DevicePrint*>(lstDev[i]);
        if(d){
            d->setInfoFromXml(strKey,lstXmlInfo);
        }
    }
}

/**
 * @brief generatorCameraDevice:生成图像设备
 */
void XmlGenerator::generatorCameraDevice()
{
    dg->generatorCameraDevice();
    //DeviceImage
    QList<QMap<QString, QString> > lstXmlInfo;
    QString strKey = "BusInfo";
    int i = 0;
    while (true) {
        QMap<QString, QString> mXmlInfo;
        XmlHandler::instance()->GetXmlInfo(QString("Camera ") + QString::number(i,10), mXmlInfo);
        i++;
        if(mXmlInfo.isEmpty()){
            break;
        }
        if(!mXmlInfo[strKey].isEmpty()){
            lstXmlInfo.append(mXmlInfo);
        }
    }
    if(lstXmlInfo.length() < 1){
        return;
    }

    QList<DeviceBaseInfo*> lstDev = DeviceManager::instance()->getCameraList();
    DeviceImage *d = nullptr;
    for (int i = 0; i < lstDev.length(); i++) {
        d = dynamic_cast<DeviceImage*>(lstDev[i]);
        if(d){
            d->setInfoFromXml(strKey,lstXmlInfo);
        }
    }
}

/**
 * @brief generatorCdromDevice:生成CDrom设备
 */
void XmlGenerator::generatorCdromDevice()
{
    dg->generatorCdromDevice();
    QList<QMap<QString, QString> > lstXmlInfo;
    QString strKey = "BusInfo";
    int i = 0;
    while (true) {
        QMap<QString, QString> mXmlInfo;
        XmlHandler::instance()->GetXmlInfo(QString("CDROM ") + QString::number(i,10), mXmlInfo);
        i++;
        if(mXmlInfo.isEmpty()){
            break;
        }
        if(!mXmlInfo[strKey].isEmpty()){
            lstXmlInfo.append(mXmlInfo);
        }
    }
    if(lstXmlInfo.length() < 1){
        return;
    }

    QList<DeviceBaseInfo*> lstDev = DeviceManager::instance()->getCdromList();
    DeviceCdrom *d = nullptr;
    for (int i = 0; i < lstDev.length(); i++) {
        d = dynamic_cast<DeviceCdrom*>(lstDev[i]);
        if(d){
            d->setInfoFromXml(strKey,lstXmlInfo);
        }
    }

}

/**
 * @brief generatorOthersDevice:生成其他设备
 */
void XmlGenerator::generatorOthersDevice()
{
    dg->generatorOthersDevice();
}

/**
 * @brief generatorPowerDevice:生成电池设备
 */
void XmlGenerator::generatorPowerDevice()
{
    dg->generatorPowerDevice();
    QList<QMap<QString, QString> > lstXmlInfo;
    QString strKey = "SerialNumber";
    int i = 0;
    while (true) {
        QMap<QString, QString> mXmlInfo;
        XmlHandler::instance()->GetXmlInfo(QString("Power ") + QString::number(i,10), mXmlInfo);
        i++;
        if(mXmlInfo.isEmpty()){
            break;
        }
        if(!mXmlInfo[strKey].isEmpty()){
            lstXmlInfo.append(mXmlInfo);
        }
    }
    if(lstXmlInfo.length() < 1){
        return;
    }

    QList<DeviceBaseInfo*> lstDev = DeviceManager::instance()->getPowerList();
    DevicePower *d = nullptr;
    for (int i = 0; i < lstDev.length(); i++) {
        d = dynamic_cast<DevicePower*>(lstDev[i]);
        if(d){
            d->setInfoFromXml(strKey,lstXmlInfo);
        }
    }
}
