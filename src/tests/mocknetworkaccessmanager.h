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

#ifndef MOCKNETWORKACCESSMANAGER_H
#define MOCKNETWORKACCESSMANAGER_H

#include <gmock/gmock.h>
#include <QQmlNetworkAccessManagerFactory>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

class MockNetworkAccessManager: public QNetworkAccessManager
{
public:
    MOCK_METHOD1(mockCreateRequest, void (const QUrl &url));
private:
    QNetworkReply * createRequest(Operation operation, const QNetworkRequest &request,
                                  QIODevice *outgoingData) override
    {
        mockCreateRequest(request.url());
        return QNetworkAccessManager::createRequest(operation, request, outgoingData);
    }
};

class MockNetworkAccessManagerFactory: public QQmlNetworkAccessManagerFactory
{
public:
    MOCK_METHOD1(create, QNetworkAccessManager * (QObject *parent));
};

#endif // MOCKNETWORKACCESSMANAGER_H

