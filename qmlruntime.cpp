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

#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>

#include "customnetworkaccessmanager.h"
#include "ipfsonlyurlinterceptor.h"
#include "qmlruntime.h"

QmlRuntime::QmlRuntime(int &argc, char *argv[])
    : QGuiApplication(argc, argv)
{
}

int QmlRuntime::startup()
{
    m_engine.reset(new QQmlEngine());
    m_urlInterceptor.reset(new IpfsOnlyUrlInterceptor()) ;
    m_networkAccessManagerFactory.reset(new CustomNetworkAccessManagerFactory());

    m_engine->setNetworkAccessManagerFactory(m_networkAccessManagerFactory.get());
    m_engine->setUrlInterceptor(m_urlInterceptor.get());

    QQmlComponent *preloadComponent = new QQmlComponent(m_engine.get(),
                                                        QUrl::fromLocalFile("preload.qml"),
                                                        QQmlComponent::PreferSynchronous);
    if (preloadComponent->isError()) {
        foreach (const QQmlError &e, preloadComponent->errors()) {
            qInfo("preload.qml: %s", e.toString().toLatin1().constData());
        }
        qCritical("Failed to load preload.qml, quitting");
    } 
    
    QQmlContext *context = m_engine->rootContext(); //new QQmlContext(engine);
    QObject *obj = preloadComponent->create(context);
    if (!obj) {
        qCritical("Failed to create preload object");
    }
    /* now in cache */
    delete obj;
    obj = 0;
    preloadComponent->deleteLater();
    preloadComponent = 0;

    // now we should switch to another user ? or in start?
    // now sandboxed for URIs
    m_urlInterceptor->lock();

    m_appComponent = new QQmlComponent(m_engine.get(), QUrl(this->arguments().at(1)));
    if (m_appComponent->isLoading()) {
        QObject::connect(m_appComponent, &QQmlComponent::statusChanged, [this](QQmlComponent::Status status) {
            switch (status) {
            case QQmlComponent::Ready:
            {
                QObject *appObject = m_appComponent->create(m_engine->rootContext());
                if (appObject == nullptr) {
                    qCritical("Failed to create app object");
                }
                break;
            }
            default:
                foreach (const QQmlError &e, m_appComponent->errors()) {
                    qInfo("App component: %s", e.toString().toLatin1().constData());
                }
                qCritical("Failed to load app component, quitting");
                break;
            }
        });
    }
    return this->exec();
}
