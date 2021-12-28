#include "XmlHandler.h"

#include <QDebug>
#include <QFile>
XmlHandler    *XmlHandler::sInstance = nullptr;

void XmlHandler::XmlHandle()
{
    mMapXmlInfo.clear();
    // 获取主板信息
    ReadFile("/usr/share/deepin-devicemanager/deviceconfig.xml");
}

void XmlHandler::GetXmlInfo(QString strType, QMap<QString, QString> &map)
{
    map = mMapXmlInfo[strType];
    return;
}
bool XmlHandler::ReadFile(QString strFileName)
{
    QFile file(strFileName);
    if(!file.exists()){
        return true;
    }
    QXmlInputSource inputSource(&file);
    QXmlSimpleReader reader;
    reader.setContentHandler(this);
    reader.setErrorHandler(this);
    return reader.parse(inputSource);
}
bool XmlHandler::startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts)
{
    if("device" == qName)
    {
        strXmlCurDevice = atts.value("type");
    }
    else if ("attr" == qName) {
        if(mMapXmlInfo.end() == mMapXmlInfo.find(strXmlCurDevice))
        {
            QMap<QString, QString> map;
            mMapXmlInfo.insert(strXmlCurDevice,map);
        }
        mMapXmlInfo[strXmlCurDevice].insert(atts.value("name"), atts.value("value"));
    }
    return true;
}
bool XmlHandler::endElement(const QString& namespaceURI, const QString& localName, const QString& qName)
{
    return true;
}
bool XmlHandler::characters(const QString& ch)
{
    return true;
}
bool XmlHandler::fatalError(const QXmlParseException& exception)
{
    qInfo() << exception.message();
    return true;
}
