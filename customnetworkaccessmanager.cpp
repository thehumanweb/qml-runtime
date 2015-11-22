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

#include "customnetworkaccessmanager.h"

QNetworkAccessManager *CustomNetworkAccessManagerFactory::customNetworkAccessManager = 0;

CustomNetworkAccessManagerFactory::CustomNetworkAccessManagerFactory() {
    customNetworkAccessManager = new CustomNetworkAccessManager();
}

QNetworkAccessManager * CustomNetworkAccessManagerFactory::create(QObject *parent) {
    Q_UNUSED(parent)
    return customNetworkAccessManager;
}

CustomNetworkAccessManager::CustomNetworkAccessManager(QObject *parent):QNetworkAccessManager(parent) {

}

QNetworkReply *CustomNetworkAccessManager::createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData) {
    QUrl url = req.url();
    QNetworkRequest newRequest(req);
  
    if (url.scheme() != "http" || !isLocalHost(url.host()) || url.port() != 8080) {
        qWarning() << "Application is trying to access non-IPFS service. Host is " << url.scheme() << "://" <<url.host() << ":" << url.port();
        // Access denied. Redirect to an IPFS object representing ipfs error
        newRequest.setUrl(QUrl(QString("")));
    }

    return QNetworkAccessManager::createRequest(op, newRequest, outgoingData);
}

bool CustomNetworkAccessManager::isLocalHost(QString host) {
    return host == "localhost" || host == "127.0.0.1";
}
