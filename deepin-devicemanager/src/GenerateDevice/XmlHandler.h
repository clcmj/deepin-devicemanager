#ifndef XMLHANDLER_H
#define XMLHANDLER_H

#include <QXmlDefaultHandler>

class XmlHandler : public QXmlDefaultHandler
{
public:

    static XmlHandler *instance()
    {
        if (!sInstance) {
            sInstance = new XmlHandler;
        }
        return sInstance;
    }
    void XmlHandle();
    void GetXmlInfo(QString strType, QMap<QString, QString> &map);
protected:
    bool startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts) override;
    bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName) override;
    bool characters(const QString& ch) override;
    bool fatalError(const QXmlParseException& exception) override;

private:
    static XmlHandler    *sInstance;
    bool ReadFile(QString strFileName);
    QMap<QString, QMap<QString, QString>> mMapXmlInfo;

    QString strXmlCurDevice;
};

#endif // XMLHANDLER_H
