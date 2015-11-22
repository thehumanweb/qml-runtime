/* 
* Copyright (C) 2015 Siteshwar Vashisht <siteshwar@gmail.com>
* 
* This library is free software; you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
* License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef QMLRUNTIME_H
#define QMLRUNTIME_H

#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlNetworkAccessManagerFactory>
#include "qobjectptr.h"
#include "ipfsonlyurlinterceptor.h"

class QmlRuntime : public QGuiApplication
{
    Q_OBJECT
public:
    QmlRuntime(int &argc, char *argv[]);
    int startup();
private:
    std::unique_ptr<IpfsOnlyUrlInterceptor> m_urlInterceptor {};
    std::unique_ptr<QQmlNetworkAccessManagerFactory> m_networkAccessManagerFactory {};
    QObjectPtr<QQmlEngine> m_engine {};
    QQmlComponent *m_appComponent {nullptr};
};

#endif
