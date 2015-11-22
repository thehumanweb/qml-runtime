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
#include <QLoggingCategory>

#include "customnetworkaccessmanager.h"
#include "ipfsonlyurlinterceptor.h"
#include "qmlruntime.h"

static const QLoggingCategory logger {"qmlruntime"};


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
    QObject::connect(m_engine.get(), &QQmlEngine::quit, QCoreApplication::quit);

    preload();

    QQmlComponent *component = new QQmlComponent(m_engine.get(), QUrl(this->arguments().at(1)));
    if (component->isLoading()) {
        QObject::connect(component, &QQmlComponent::statusChanged, [this, component](QQmlComponent::Status status) {
            switch (status) {
            case QQmlComponent::Ready:
            {
                QObject *appObject = component->create(m_engine->rootContext());
                if (appObject == nullptr) {
                    qCCritical(logger) << "Failed to create application object";
                }
                break;
            }
            default:
                foreach (const QQmlError &error, component->errors()) {
                    qCInfo(logger) << "Application component:" << error.toString();
                }
                qCCritical(logger) << "Failed to load application component";
                break;
            }
        });
    }
    return this->exec();
}

/**
 * @brief Preload whitelisted components
 *
 * This method will preload whitelisted components
 * in the QML engine, so that the URL interceptor will
 * let them be loaded by the engine.
 *
 * When the preloaded components are loaded, this method
 * will lock the URL interceptor.
 */
void QmlRuntime::preload()
{
    QObjectPtr<QQmlComponent> component (new QQmlComponent(m_engine.get(),
                                                           QUrl::fromLocalFile("preload.qml"),
                                                           QQmlComponent::PreferSynchronous));

    if (component->isError()) {
        foreach (const QQmlError &error, component->errors()) {
            qCInfo(logger) << "preload.qml:" << error.toString();
        }
        qCCritical(logger) << "Failed to load preload.qml";
    }

    QQmlContext *context = m_engine->rootContext();
    QObjectPtr<QObject> object (component->create(context));

    if (!object) {
        qCCritical(logger) << "Failed to create preload object";
    }

    // now we should switch to another user ? or in start?
    // now sandboxed for URIs
    m_urlInterceptor->lock();
}
