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

#ifndef SAFELOADER_HPP
#define SAFELOADER_HPP

#include <QQuickItem>
#include <QQmlComponent>
#include <QQmlContext>
#include "qobjectptr.h"

class SafeLoader : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_ENUMS(Status)
public:
    enum Status
    {
        Null,
        Ready,
        Loading,
        Error
    };
    explicit SafeLoader(QQuickItem *parent = 0);
    void componentComplete() override;
    Status status() const;
    QUrl source() const;
    void setSource(const QUrl &source);
signals:
    void statusChanged();
    void sourceChanged();
    void finished(bool ok);
protected:
    bool event(QEvent *e) override;
private:
    void deferNull();
    void deferCreation();
    void setStatus(Status status);
    void load();
    void createObject();
    QUrl m_source;
    Status m_status {Null};
    QObjectPtr<QQmlContext> m_context {};
    QObjectPtr<QQmlComponent> m_component {};
    QObjectPtr<QObject> m_object {};
};

#endif // SAFELOADER_HPP
