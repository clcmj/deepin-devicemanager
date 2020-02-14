/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     AaronZhang <ya.zhang@archermind.com>
 *
 * Maintainer: AaronZhang <ya.zhang@archermind.com>
 * Maintainer: Yaobin <yao.bin@archermind.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "camerawidget.h"
#include "deviceinfoparser.h"
#include <DApplication>

DWIDGET_USE_NAMESPACE

CameraWidget::CameraWidget(QWidget *parent) : DeviceInfoWidgetBase(parent, tr("Camera"))
{
    initWidget();
}

void CameraWidget::initWidget()
{
    QStringList cameraList = DeviceInfoParser::Instance().getLshwCameraList();

    if( cameraList.size() < 1 )
    {
        setCentralInfo(tr("No camera found"));
        return;
    }

    QStringList cameraNames = {
                            tr("Name"),
                            tr("Vendor"),
                            tr("Capabilities")
                        };

    QList<QStringList> tabList;
    QList<ArticleStruct> articles;
    QSet<QString> existArticles;

    foreach(const QString& device, cameraList)
    {
        articles.clear();
        existArticles.clear();

        ArticleStruct name(tr("Name"));
        name.queryData( "lshw", device, "product");
        articles.push_back(name);
        existArticles.insert("product");

        ArticleStruct description(tr("Description"));
        description.queryData("lshw", device, "description");
        articles.push_back(description);
        existArticles.insert("description");

        ArticleStruct vendor(tr("Vendor"));
        vendor.queryData( "lshw", device, "vendor");
        articles.push_back(vendor);
        existArticles.insert("vendor");

        ArticleStruct physicalId(tr("Physical ID"));
        physicalId.queryData("lshw", device, "physical id");
        articles.push_back(physicalId);
        existArticles.insert("physical id");

        ArticleStruct busInfo(tr("Bus Info"));
        busInfo.queryData("lshw", device, "bus info");
        articles.push_back(busInfo);
        existArticles.insert("bus info");

        ArticleStruct version(tr("Version"));
        version.queryData("lshw", device, "version");
        articles.push_back(version);
        existArticles.insert("version");

        ArticleStruct capabilities(tr("Capabilities"));
        capabilities.queryData("lshw", device, "capabilities");
        articles.push_back(capabilities);
        existArticles.insert("capabilities");

        DeviceInfoParser::Instance().queryRemainderDeviceInfo("lshw", device, articles, existArticles,
                                                              "ManulTrack__Camera","Camera infomation from lshw");

        addDevice( name.value, articles, cameraList.size() );

        if( cameraList.size() > 1 )
        {
            QStringList tab =
            {
                name.value,
                vendor.value
            };

            tabList.push_back(tab);
        }

        if( overviewInfo_.value.isEmpty() == true )
        {
            overviewInfo_.value = vendor.value;
            overviewInfo_.value += " ";
            if( name.value.isEmpty() == false && name.value != tr("Unknown") )
            {
                overviewInfo_.value += name.value;
            }
            else
            {
                overviewInfo_.value += description.value;
            }
        }
    }

    if( cameraList.size() > 1 )
    {
        QStringList headers = { tr("Name"),tr("Vendor") };
        addTable( headers, tabList);
    }
}

