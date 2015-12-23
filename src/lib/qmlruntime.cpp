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

#include <QQmlComponent>
#include <QLoggingCategory>
#include <QCoreApplication>

#include "customnetworkaccessmanager.h"
#include "ipfsonlyurlinterceptor.h"
#include "qmlruntime.h"

static const QLoggingCategory logger {"qml-runtime"};

QmlRuntime::QmlRuntime(std::unique_ptr<ILockableUrlInterceptor> &&urlInterceptor,
                       std::unique_ptr<QQmlNetworkAccessManagerFactory> &&networkAccessManagerFactory)
    : m_urlInterceptor(std::move(urlInterceptor))
    , m_networkAccessManagerFactory(std::move(networkAccessManagerFactory))
    , m_engine(new QQmlEngine())
{
    m_engine->setNetworkAccessManagerFactory(m_networkAccessManagerFactory.get());
    m_engine->setUrlInterceptor(m_urlInterceptor.get());
    QObject::connect(m_engine.get(), &QQmlEngine::quit, QCoreApplication::quit);
}

QObject * QmlRuntime::object() const
{
    return m_object.get();
}

QmlRuntime::Status QmlRuntime::status() const
{
    return m_status;
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
 *
 * @return if the preload succeded.
 */
bool QmlRuntime::preload(const QUrl &url)
{
    QObjectPtr<QQmlComponent> component (new QQmlComponent(m_engine.get(), url, QQmlComponent::PreferSynchronous));

    if (!component->isReady()) {
        qCWarning(logger) << "Failed to create preload component";

        if (component->isLoading()) {
            qCWarning(logger) << "Preloading remote resources are not supported";
        } else {
            foreach (const QQmlError &error, component->errors()) {
                qCInfo(logger) << error.toString();
            }
        }
        return false;
    }

    QQmlContext *context = m_engine->rootContext();
    QObjectPtr<QObject> object (component->create(context));

    if (!object) {
        qCWarning(logger) << "Failed to create preload object";
        return false;
    }

    // now we should switch to another user ? or in start?
    // now sandboxed for URIs
    m_urlInterceptor->lock();
    return true;
}

void QmlRuntime::execute(const QUrl &url)
{
    m_component = new QQmlComponent(m_engine.get(), url);
    if (m_component->isLoading()) {
        setStatus(Status::Loading);
        connect(m_component, &QQmlComponent::statusChanged, [this](QQmlComponent::Status status) {
            Q_UNUSED(status)
            create();
        });
    } else {
        create();
    }
}

void QmlRuntime::create()
{
    Q_ASSERT(!m_component->isLoading());
    if (!m_component->isReady()) {
        setStatus(Status::Error);
        qCWarning(logger) << "Failed create application component";
        foreach (const QQmlError &error, m_component->errors()) {
            qCInfo(logger) << error.toString();
        }
        return;
    }

    m_context.reset(new QQmlContext(m_engine->rootContext()));
    QObjectPtr<QObject> object (m_component->create(m_context.get()));
    if (!object) {
        setStatus(Status::Error);
        qWarning(logger) << "Failed to create application object";
        return;
    }

    setStatus(Status::Ready);
    if (m_object.get() != object.get()) {
        m_object = std::move(object);
        emit objectChanged();
    }
}

void QmlRuntime::setStatus(QmlRuntime::Status status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged();
    }
}
