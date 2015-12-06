/*
* Copyright (C) 2015 Lucien Xu <sfietkonstantin@free.fr>
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

#include "safeloader.h"
#include <QCoreApplication>
#include <QQmlEngine>
#include <QLoggingCategory>

static const QLoggingCategory logger {"safe-loader"};

class SafeLoaderEvent: public QEvent
{
public:
    enum DeferType
    {
        DeferNull,
        DeferCreation
    };
    explicit SafeLoaderEvent(DeferType deferType)
        : QEvent(QEvent::User), m_deferType(deferType)
    {
    }
    DeferType deferType() const
    {
        return m_deferType;
    }
private:
    DeferType m_deferType {DeferNull};
};

SafeLoader::SafeLoader(QQuickItem *parent)
    : QQuickItem(parent)
{
}

void SafeLoader::componentComplete()
{
    QQuickItem::componentComplete();

    QQmlEngine *engine = qmlEngine(this);
    QQmlContext *parentContext = qmlContext(this);
    Q_ASSERT(parentContext);
    m_context.reset(new QQmlContext(engine->rootContext()));
}

SafeLoader::Status SafeLoader::status() const
{
    return m_status;
}

QUrl SafeLoader::source() const
{
    return m_source;
}

void SafeLoader::setSource(const QUrl &source)
{
    if (m_source != source) {
        m_source = source;
        load();
        emit sourceChanged();
    }
}

bool SafeLoader::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::User:
    {
        switch (static_cast<SafeLoaderEvent *>(e)->deferType()) {
        case SafeLoaderEvent::DeferNull:
            setStatus(Null);
            emit finished(true);
            return true;
            break;
        case SafeLoaderEvent::DeferCreation:
            createObject();
            break;
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
    return QQuickItem::event(e);
}

void SafeLoader::deferNull()
{
    QCoreApplication::instance()->postEvent(this, new SafeLoaderEvent(SafeLoaderEvent::DeferNull));
}

void SafeLoader::deferCreation()
{
    QCoreApplication::instance()->postEvent(this, new SafeLoaderEvent(SafeLoaderEvent::DeferCreation));
}

void SafeLoader::setStatus(SafeLoader::Status status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged();
    }
}

void SafeLoader::load()
{
    if (m_source.isEmpty()) {
        m_object.reset();
        deferNull();
        return;
    }

    m_component.reset(new QQmlComponent(qmlEngine(this), m_source, QQmlComponent::Asynchronous));

    if (m_component->isReady() || m_component->isError()) {
        deferCreation();
        return;
    }

    if (m_component->isLoading()) {
        setStatus(Loading);
        connect(m_component.get(), &QQmlComponent::statusChanged, [this](QQmlComponent::Status status) {
            Q_UNUSED(status)
            createObject();
        });
        return;
    }
}

void SafeLoader::createObject()
{
    Q_ASSERT(m_component->isReady() || m_component->isError());

    if (m_component->isError()) {
        qCWarning(logger) << "Failed to load component" << m_component->url();
        foreach (const QQmlError &error, m_component->errors()) {
            qCWarning(logger) << error.toString();
        }
        setStatus(Error);
        emit finished(false);
        return;
    }

    m_object.reset(m_component->create(m_context.get()));
    if (!m_object) {
        qCWarning(logger) << "Failed to create object from component" << m_component->url();
        setStatus(Error);
        emit finished(false);
        return;
    }

    QQuickItem *item = qobject_cast<QQuickItem *>(m_object.get());
    if (item) {
        item->setParentItem(this);
    }
    setStatus(Ready);
    emit finished(true);
}

