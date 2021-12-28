#ifndef XMLGENERATOR_H
#define XMLGENERATOR_H

#include"DeviceGenerator.h"

class XmlGenerator : public DeviceGenerator
{
public:
    explicit XmlGenerator();
    explicit XmlGenerator(QString strArch);
    /**
     * @brief generatorBiosDevice:生成主板设备信息
     */
    virtual void generatorBiosDevice() override;
    /**
     * @brief getBiosInfo:生成Bios信息
     */
    virtual void getBiosInfo() override;
    /**
     * @brief getSystemInfo:生成System信息
     */
    virtual void getSystemInfo() override;
    /**
     * @brief getBaseBoardInfo:生成BaseBoard信息
     */
    virtual void getBaseBoardInfo() override;
    /**
     * @brief getChassisInfo:生成Chassis信息
     */
    virtual void getChassisInfo() override;
    /**
     * @brief getBiosMemoryInfo:生成BiosMemory信息
     */
    virtual void getBiosMemoryInfo() override;

    /**
     * @brief generatorCpuDevice:生成CPU设备
     */
    virtual void generatorCpuDevice() override;

    /**
     * @brief generatorMemoryDevice:生成内存设备
     */
    virtual void generatorMemoryDevice() override;

    /**
     * @brief generatorDiskDevice生成存储设备信息
     */
    virtual void generatorDiskDevice() override;

    /**
     * @brief generatorGpuDevice:生成显卡信息
     */
    virtual void generatorGpuDevice() override;

    /**
     * @brief generatorMonitorDevice:生成显示设备
     */
    virtual void generatorMonitorDevice() override;

    /**
     * @brief generatorNetworkDevice:生成网卡
     */
    virtual void generatorNetworkDevice() override;

    /**
     * @brief generatorAudioDevice:生成音频适配器
     */
    virtual void generatorAudioDevice() override;

    /**
     * @brief generatorBluetoothDevice:生成蓝牙设备
     */
    virtual void generatorBluetoothDevice() override;

    /**
     * @brief generatorKeyboardDevice:生成键盘设备
     */
    virtual void generatorKeyboardDevice() override;

    /**
     * @brief generatorMouseDevice:生成鼠标设备
     */
    virtual void generatorMouseDevice() override;

    /**
     * @brief generatorPrinterDevice:生成打印机设备
     */
    virtual void generatorPrinterDevice() override;

    /**
     * @brief generatorCameraDevice:生成图像设备
     */
    virtual void generatorCameraDevice() override;

    /**
     * @brief generatorCdromDevice:生成CDrom设备
     */
    virtual void generatorCdromDevice() override;

    /**
     * @brief generatorOthersDevice:生成其他设备
     */
    virtual void generatorOthersDevice() override;

    /**
     * @brief generatorPowerDevice:生成电池设备
     */
    virtual void generatorPowerDevice() override;
private:
    DeviceGenerator *dg = nullptr;

};

#endif // XMLGENERATOR_H
