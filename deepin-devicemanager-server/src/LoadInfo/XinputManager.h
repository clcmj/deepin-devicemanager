#ifndef XINPUTMANAGER_H
#define XINPUTMANAGER_H

#include <QProcess>
#include <QMap>

class XinputManager
{
public:
    static XinputManager* instance()
    {
        if(!s_Instance)
            s_Instance = new XinputManager;
        return s_Instance;
    }

    void update();
    void disableDevices(const QString& info);

protected:
    XinputManager();

private:
    /**
     * @brief updateInputDevice
     */
    void updateInputDevice();
    bool parseItem(const QString& item,QString &sysFs,QString &event);

    /**
     * @brief updateXinputList
     */
    void updateXinputList();
    void parseProps(const QString& id);

    void disableDevice(const QString& item);
    void getMapInfo(const QString& item,QMap<QString,QString>& mapInfo);

private:
    static XinputManager* s_Instance;
    QMap<QString,QString> m_MapFsToEvent;
    QMap<QString,QString> m_MapEventToID;
};

#endif // XINPUTMANAGER_H
