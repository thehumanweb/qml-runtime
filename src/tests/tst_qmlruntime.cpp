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
#include <qmlruntime.h>
#include <QSignalSpy>
#include <QTest>
#include "mocknetworkaccessmanager.h"
#include "mockurlinterceptor.h"

using testing::_;
using testing::Return;
using testing::Invoke;
using testing::NiceMock;

static QUrl getUrl(const QUrl &url, QQmlAbstractUrlInterceptor::DataType type)
{
    Q_UNUSED(type)
    return url;
}

TEST(QmlRuntime, Preload)
{
    std::unique_ptr<NiceMock<MockNetworkAccessManagerFactory>> namFactory (new NiceMock<MockNetworkAccessManagerFactory>());

    std::unique_ptr<MockUrlInterceptor> urlInterceptor (new MockUrlInterceptor());
    // intercept called 3 times:
    // - preload.qml
    // - QtQuick qmldir
    // - QtQuick.Window qmldir
    EXPECT_CALL(*urlInterceptor, intercept(_, _)).Times(3).WillRepeatedly(Invoke(getUrl));
    EXPECT_CALL(*urlInterceptor, lock()).Times(1);

    QmlRuntime runtime (std::move(urlInterceptor), std::move(namFactory));
    EXPECT_TRUE(runtime.preload(QUrl("qrc:/tests/preload.qml")));
}

TEST(QmlRuntime, PreloadNull)
{
    std::unique_ptr<NiceMock<MockNetworkAccessManagerFactory>> namFactory (new NiceMock<MockNetworkAccessManagerFactory>());
    std::unique_ptr<NiceMock<MockUrlInterceptor>> urlInterceptor (new NiceMock<MockUrlInterceptor>());

    QmlRuntime runtime (std::move(urlInterceptor), std::move(namFactory));
    EXPECT_FALSE(runtime.preload(QUrl()));
}

TEST(QmlRuntime, PreloadFail)
{
    std::unique_ptr<NiceMock<MockNetworkAccessManagerFactory>> namFactory (new NiceMock<MockNetworkAccessManagerFactory>());

    std::unique_ptr<MockUrlInterceptor> urlInterceptor (new MockUrlInterceptor());
    // intercept called 1 time:
    // - preload-fail.qml
    EXPECT_CALL(*urlInterceptor, intercept(_, _)).Times(1).WillRepeatedly(Invoke(getUrl));
    EXPECT_CALL(*urlInterceptor, lock()).Times(0);

    QmlRuntime runtime (std::move(urlInterceptor), std::move(namFactory));
    EXPECT_FALSE(runtime.preload(QUrl("qrc:/tests/preload-fail.qml")));
}

TEST(QmlRuntime, PreloadRemote)
{
    QUrl remoteUrl ("http://localhost/preload-remote.qml");
    QObjectPtr<MockNetworkAccessManager> nam (new MockNetworkAccessManager());
    EXPECT_CALL(*nam, mockCreateRequest(remoteUrl)).Times(1);

    std::unique_ptr<MockNetworkAccessManagerFactory> namFactory (new MockNetworkAccessManagerFactory());
    EXPECT_CALL(*namFactory, create(_)).Times(1).WillRepeatedly(Return(nam.get()));

    std::unique_ptr<MockUrlInterceptor> urlInterceptor (new MockUrlInterceptor());
    // intercept called 1 time:
    // - preload-remote.qml
    EXPECT_CALL(*urlInterceptor, intercept(_, _)).Times(1).WillRepeatedly(Invoke(getUrl));
    EXPECT_CALL(*urlInterceptor, lock()).Times(0);

    QmlRuntime runtime (std::move(urlInterceptor), std::move(namFactory));
    EXPECT_FALSE(runtime.preload(remoteUrl));
}

TEST(QmlRuntime, ExecuteLocal)
{
    std::unique_ptr<NiceMock<MockNetworkAccessManagerFactory>> namFactory (new NiceMock<MockNetworkAccessManagerFactory>());

    std::unique_ptr<MockUrlInterceptor> urlInterceptor (new MockUrlInterceptor());
    EXPECT_CALL(*urlInterceptor, intercept(_, _)).WillRepeatedly(Invoke(getUrl));
    EXPECT_CALL(*urlInterceptor, lock()).Times(0);

    QmlRuntime runtime (std::move(urlInterceptor), std::move(namFactory));
    QSignalSpy objectSpy (&runtime, SIGNAL(objectChanged()));
    QSignalSpy statusSpy (&runtime, SIGNAL(statusChanged()));

    // Local component should be loaded synchronously
    runtime.execute(QUrl("qrc:/tests/preload.qml"));
    EXPECT_EQ(objectSpy.count(), 1);
    EXPECT_EQ(statusSpy.count(), 1);
    EXPECT_NE(runtime.object(), nullptr);
    EXPECT_EQ(runtime.status(), QmlRuntime::Status::Ready);
}

TEST(QmlRuntime, ExecuteLocalFail)
{
    std::unique_ptr<NiceMock<MockNetworkAccessManagerFactory>> namFactory (new NiceMock<MockNetworkAccessManagerFactory>());

    std::unique_ptr<MockUrlInterceptor> urlInterceptor (new MockUrlInterceptor());
    EXPECT_CALL(*urlInterceptor, intercept(_, _)).WillRepeatedly(Invoke(getUrl));
    EXPECT_CALL(*urlInterceptor, lock()).Times(0);

    QmlRuntime runtime (std::move(urlInterceptor), std::move(namFactory));
    QSignalSpy objectSpy (&runtime, SIGNAL(objectChanged()));
    QSignalSpy statusSpy (&runtime, SIGNAL(statusChanged()));

    // Local component should be loaded synchronously
    runtime.execute(QUrl("qrc:/tests/preload-fail.qml"));
    EXPECT_EQ(objectSpy.count(), 0);
    EXPECT_EQ(statusSpy.count(), 1);
    EXPECT_EQ(runtime.object(), nullptr);
    EXPECT_EQ(runtime.status(), QmlRuntime::Status::Error);
}

TEST(QmlRuntime, ExecuteRemote)
{
    QUrl remoteUrl ("https://raw.githubusercontent.com/thehumanweb/qml-runtime/master/src/tests/preload.qml");
    QUrl remoteQmlDir ("https://raw.githubusercontent.com/thehumanweb/qml-runtime/master/src/tests/qmldir");
    QObjectPtr<MockNetworkAccessManager> nam (new MockNetworkAccessManager());
    EXPECT_CALL(*nam, mockCreateRequest(remoteUrl)).Times(1);
    EXPECT_CALL(*nam, mockCreateRequest(remoteQmlDir)).Times(1);

    std::unique_ptr<MockNetworkAccessManagerFactory> namFactory (new MockNetworkAccessManagerFactory());
    EXPECT_CALL(*namFactory, create(_)).Times(1).WillRepeatedly(Return(nam.get()));

    std::unique_ptr<MockUrlInterceptor> urlInterceptor (new MockUrlInterceptor());
    EXPECT_CALL(*urlInterceptor, intercept(_, _)).WillRepeatedly(Invoke(getUrl));
    EXPECT_CALL(*urlInterceptor, lock()).Times(0);

    QmlRuntime runtime (std::move(urlInterceptor), std::move(namFactory));
    QSignalSpy objectSpy (&runtime, SIGNAL(objectChanged()));
    QSignalSpy statusSpy (&runtime, SIGNAL(statusChanged()));

    // Remote component should be loaded asynchronously
    runtime.execute(remoteUrl);
    EXPECT_EQ(objectSpy.count(), 0);
    EXPECT_EQ(statusSpy.count(), 1);
    EXPECT_EQ(runtime.object(), nullptr);
    EXPECT_EQ(runtime.status(), QmlRuntime::Status::Loading);

    while (statusSpy.count() == 1) {
        QTest::qWait(100);
    }
    EXPECT_EQ(objectSpy.count(), 1);
    EXPECT_EQ(statusSpy.count(), 2);
    EXPECT_NE(runtime.object(), nullptr);
    EXPECT_EQ(runtime.status(), QmlRuntime::Status::Ready);
}

TEST(QmlRuntime, ExecuteRemoteFail)
{
    QUrl remoteUrl ("https://raw.githubusercontent.com/thehumanweb/qml-runtime/master/src/tests/preload-fail.qml");
    QObjectPtr<MockNetworkAccessManager> nam (new MockNetworkAccessManager());
    EXPECT_CALL(*nam, mockCreateRequest(remoteUrl)).Times(1);

    std::unique_ptr<MockNetworkAccessManagerFactory> namFactory (new MockNetworkAccessManagerFactory());
    EXPECT_CALL(*namFactory, create(_)).Times(1).WillRepeatedly(Return(nam.get()));

    std::unique_ptr<MockUrlInterceptor> urlInterceptor (new MockUrlInterceptor());
    EXPECT_CALL(*urlInterceptor, intercept(_, _)).WillRepeatedly(Invoke(getUrl));
    EXPECT_CALL(*urlInterceptor, lock()).Times(0);

    QmlRuntime runtime (std::move(urlInterceptor), std::move(namFactory));
    QSignalSpy objectSpy (&runtime, SIGNAL(objectChanged()));
    QSignalSpy statusSpy (&runtime, SIGNAL(statusChanged()));

    // Remote component should be loaded asynchronously
    runtime.execute(remoteUrl);
    EXPECT_EQ(objectSpy.count(), 0);
    EXPECT_EQ(statusSpy.count(), 1);
    EXPECT_EQ(runtime.object(), nullptr);
    EXPECT_EQ(runtime.status(), QmlRuntime::Status::Loading);

    while (statusSpy.count() == 1) {
        QTest::qWait(100);
    }
    EXPECT_EQ(objectSpy.count(), 0);
    EXPECT_EQ(statusSpy.count(), 2);
    EXPECT_EQ(runtime.object(), nullptr);
    EXPECT_EQ(runtime.status(), QmlRuntime::Status::Error);
}
