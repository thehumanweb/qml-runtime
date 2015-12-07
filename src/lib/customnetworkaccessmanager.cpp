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

#include <QUrl>
#include <QNetworkRequest>
#include <QLoggingCategory>
#include "customnetworkaccessmanager.h"

static const QLoggingCategory logger {"network-access-manager"};

QNetworkAccessManager * CustomNetworkAccessManagerFactory::create(QObject *parent)
{
    return new CustomNetworkAccessManager(parent);
}

CustomNetworkAccessManager::CustomNetworkAccessManager(QObject *parent)
    : QNetworkAccessManager(parent)
{
}

QNetworkReply * CustomNetworkAccessManager::createRequest(Operation operation,
                                                          const QNetworkRequest &request,
                                                          QIODevice *outgoingData)
{
    QUrl url = request.url();
    QNetworkRequest newRequest(request);
  
    if (url.scheme() != QLatin1String("http") || !isLocalHost(url.host()) || url.port() != 8080) {
        // Access denied. Redirect to an IPFS object representing ipfs error
        qCWarning(logger) << "Application is trying to access non-IPFS service. Host is "
                          << url.scheme() << "://" <<url.host() << ":" << url.port();
        newRequest.setUrl(QUrl());
    }
    return QNetworkAccessManager::createRequest(operation, newRequest, outgoingData);
}

bool CustomNetworkAccessManager::isLocalHost(const QString &host)
{
    return host == QLatin1String("localhost") || host == QLatin1String("127.0.0.1");
}
