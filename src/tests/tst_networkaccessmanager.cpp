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

#include <gtest/gtest.h>
#include <QTest>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <customnetworkaccessmanager.h>
#include <qobjectptr.h>

TEST(NetworkAccessManager, Get)
{
    CustomNetworkAccessManagerFactory factory;
    QObjectPtr<QNetworkAccessManager> nam (factory.create(nullptr));

    // Non-localhost:8080 Url
    QObjectPtr<QNetworkReply> reply1 (nam->get(QNetworkRequest(QUrl("http://www.google.com"))));
    while (!reply1->isFinished()) {
        QTest::qWait(100);
    }
    EXPECT_EQ(reply1->url(), QUrl());
    EXPECT_NE(reply1->error(), QNetworkReply::NoError);

    // localhost:8080 Url
    QUrl url2 ("http://localhost:8080/ipfs/hash/test.png");
    QObjectPtr<QNetworkReply> reply2 (nam->get(QNetworkRequest(url2)));
    EXPECT_EQ(reply2->url(), url2);

    QUrl url3 ("http://127.0.0.1:8080/ipfs/hash/test.png");
    QObjectPtr<QNetworkReply> reply3 (nam->get(QNetworkRequest(url3)));
    EXPECT_EQ(reply3->url(), url3);
}

