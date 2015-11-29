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

#include <QObject>
#include <QQmlEngine>
#include <QQmlNetworkAccessManagerFactory>
#include "qobjectptr.h"
#include "ilockableurlinterceptor.h"

class QmlRuntime: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject * object READ object NOTIFY objectChanged)
public:
    enum class Status {
        Null,
        Ready,
        Loading,
        Error
    };
    using Ptr = std::unique_ptr<QmlRuntime>;
    QmlRuntime(std::unique_ptr<ILockableUrlInterceptor> &&urlInterceptor,
               std::unique_ptr<QQmlNetworkAccessManagerFactory> &&networkAccessManagerFactory);
    QObject * object() const;
    Status status() const;
    bool preload(const QUrl &url);
    void execute(const QUrl &url);
signals:
    void objectChanged();
    void statusChanged();
private:
    void create();
    void setStatus(Status status);
    std::unique_ptr<ILockableUrlInterceptor> m_urlInterceptor {};
    std::unique_ptr<QQmlNetworkAccessManagerFactory> m_networkAccessManagerFactory {};
    QObjectPtr<QQmlEngine> m_engine {};
    QQmlComponent *m_component {nullptr};
    Status m_status {Status::Null};
    QObjectPtr<QObject> m_object {};
};

#endif
