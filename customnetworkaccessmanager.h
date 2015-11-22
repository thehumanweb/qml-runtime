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

#ifndef CUSTOMNETWORKACCESSMANAGER_H
#define CUSTOMNETWORKACCESSMANAGER_H

#include <QQmlNetworkAccessManagerFactory>
#include <QNetworkAccessManager>

class CustomNetworkAccessManagerFactory : public QQmlNetworkAccessManagerFactory
{
public:
    CustomNetworkAccessManagerFactory();
    QNetworkAccessManager *create(QObject *) override;
private:
    static QNetworkAccessManager *s_customNetworkAccessManager;
};

class CustomNetworkAccessManager : public QNetworkAccessManager
{
public:
    CustomNetworkAccessManager(QObject *parent=0);
protected:
    QNetworkReply *createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0);
private:
    bool isLocalHost(QString);
};

#endif
