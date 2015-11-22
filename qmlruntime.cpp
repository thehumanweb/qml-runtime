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
    QQmlEngine *engine = new QQmlEngine;
    IpfsOnlyUrlInterceptor *interceptor = new IpfsOnlyUrlInterceptor;
    QQmlComponent *preloadcomponent = 0; 

    engine->setNetworkAccessManagerFactory(new CustomNetworkAccessManagerFactory); 
    engine->setUrlInterceptor(interceptor);

    preloadcomponent = new QQmlComponent(engine, QUrl::fromLocalFile("preload.qml"), QQmlComponent::PreferSynchronous);	
    
    if (preloadcomponent->isError()) {
        foreach (const QQmlError &e, preloadcomponent->errors()) {
            qInfo("preload.qml: %s", e.toString().toLatin1().constData());
        }
        qCritical("Failed to load preload.qml, quitting");
    } 
    
    m_context = new QQmlContext(engine);
    QObject *obj = preloadcomponent->create(m_context);
    if (!obj) {
        qCritical("Failed to create preload object");
    }
    /* now in cache */
    delete obj;
    obj = 0;
    preloadcomponent->deleteLater(); 
    preloadcomponent = 0;

    // now we should switch to another user ? or in start?
    // now sandboxed for URIs
    interceptor->lock();

    this->m_appcomponent = new QQmlComponent(engine, QUrl(this->arguments().at(1)));
    if (this->m_appcomponent->isLoading()) {
        QObject::connect(this->m_appcomponent, SIGNAL(statusChanged(QQmlComponent::Status)),
                         this, SLOT(continueLoading()));
    }
    return this->exec();
}

void QmlRuntime::continueLoading()
{
    if (this->m_appcomponent->isError()) {
        foreach (const QQmlError &e, this->m_appcomponent->errors()) {
            qInfo("App component: %s", e.toString().toLatin1().constData());
        }
        qCritical("Failed to load app component, quitting");
    }
    
    QObject *appobj = this->m_appcomponent->create(m_context);
 
    if (!appobj) {
        qCritical("Failed to create app object");
    }
}
