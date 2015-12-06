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
#include <ipfsonlyurlinterceptor.h>
#include "formatters.h"

TEST(UrlInterceptor, Unlocked)
{
    IpfsOnlyUrlInterceptor interceptor;

    // Local file
    QUrl local = QUrl::fromLocalFile("/home/test/something/file.png");
    QUrl returnedLocal = interceptor.intercept(local, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedLocal, local);

    // Url
    QUrl http ("http://www.test.com/path/subpath");
    QUrl returnedHttp = interceptor.intercept(http, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedHttp, http);

    // Fake protocol
    QUrl fakeProtocol ("fakeprotocol://test/something/somethingelse");
    QUrl returnedFakeProtocol = interceptor.intercept(fakeProtocol, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedFakeProtocol, fakeProtocol);
}

TEST(UrlInterceptor, Locked)
{
    QUrl notPermitted ("notpermitted://");
    IpfsOnlyUrlInterceptor interceptor;
    interceptor.lock();

    // Local file
    QUrl local = QUrl::fromLocalFile("/home/test/something/file.png");
    QUrl returnedLocal = interceptor.intercept(local, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedLocal, notPermitted);

    // Url
    QUrl http ("http://www.test.com/path/subpath");
    QUrl returnedHttp = interceptor.intercept(http, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedHttp, notPermitted);

    // Fake protocol
    QUrl fakeProtocol ("fakeprotocol://test/something/somethingelse");
    QUrl returnedFakeProtocol = interceptor.intercept(fakeProtocol, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedFakeProtocol, notPermitted);
}

TEST(UrlInterceptor, LockedIpfs)
{
    IpfsOnlyUrlInterceptor interceptor;
    interceptor.lock();

    // IPFS
    QUrl ipfs = QUrl::fromLocalFile("/ipfs/hash/file.png");
    QUrl expectedIpfs = QUrl("http://localhost:8080/ipfs/hash/file.png");
    QUrl returnedIpfs = interceptor.intercept(ipfs, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedIpfs, expectedIpfs);

    // IPNS
    QUrl ipns = QUrl::fromLocalFile("/ipns/hash/file.png");
    QUrl expectedIpns = QUrl("http://localhost:8080/ipns/hash/file.png");
    QUrl returnedIpns = interceptor.intercept(ipns, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedIpns, expectedIpns);
}

TEST(UrlInterceptor, Whitelisting)
{
    QUrl notPermitted ("notpermitted://");
    IpfsOnlyUrlInterceptor interceptor;

    // Whitelisting
    // Local file
    QUrl local = QUrl::fromLocalFile("/home/test/something/file.png");
    QUrl returnedLocal = interceptor.intercept(local, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedLocal, local);

    // Url
    QUrl http ("http://www.test.com/path/subpath");
    QUrl returnedHttp = interceptor.intercept(http, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedHttp, http);

    // Fake protocol
    QUrl fakeProtocol ("fakeprotocol://test/something/somethingelse");
    QUrl returnedFakeProtocol = interceptor.intercept(fakeProtocol, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedFakeProtocol, fakeProtocol);

    interceptor.lock();

    // Check whitelist
    // Local file
    QUrl returnedLocalWhitelist = interceptor.intercept(local, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedLocalWhitelist, local);

    // Url
    QUrl returnedHttpWhitelist = interceptor.intercept(http, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedHttpWhitelist, http);

    // Fake protocol
    QUrl returnedFakeProtocolWhitelist = interceptor.intercept(fakeProtocol, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedFakeProtocolWhitelist, fakeProtocol);

    // Check non-whitelisted
    // Local file
    QUrl blacklistLocal = QUrl::fromLocalFile("/home/test/blacklist/file.png");
    QUrl returnedBlacklistLocal = interceptor.intercept(blacklistLocal, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedBlacklistLocal, notPermitted);

    // Url
    QUrl blacklistHttp ("http://www.test.com/blacklist/subpath");
    QUrl returnedBlacklistHttp = interceptor.intercept(blacklistHttp, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedBlacklistHttp, notPermitted);

    // Fake protocol
    QUrl blacklistFakeProtocol ("fakeprotocol://test/blacklist/somethingelse");
    QUrl returnedBlacklistFakeProtocol = interceptor.intercept(blacklistFakeProtocol, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedBlacklistFakeProtocol, notPermitted);

    // Check IPFS
    // IPFS
    QUrl ipfs = QUrl::fromLocalFile("/ipfs/hash/file.png");
    QUrl expectedIpfs = QUrl("http://localhost:8080/ipfs/hash/file.png");
    QUrl returnedIpfs = interceptor.intercept(ipfs, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedIpfs, expectedIpfs);

    // IPNS
    QUrl ipns = QUrl::fromLocalFile("/ipns/hash/file.png");
    QUrl expectedIpns = QUrl("http://localhost:8080/ipns/hash/file.png");
    QUrl returnedIpns = interceptor.intercept(ipns, QQmlAbstractUrlInterceptor::UrlString);
    EXPECT_EQ(returnedIpns, expectedIpns);
}
