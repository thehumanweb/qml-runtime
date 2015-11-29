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

#include "ipfsonlyurlinterceptor.h"
#include <QtCore/QLoggingCategory>

static const QLoggingCategory logger {"url-interceptor"};

void IpfsOnlyUrlInterceptor::lock()
{
    qCInfo(logger) << "Sandbox is now locked";
    for (const QUrl &url : m_whitelisted) {
        qCInfo(logger) << "Whitelisted" << url.toString();
    }
    m_locked = true;
}

QUrl IpfsOnlyUrlInterceptor::intercept(const QUrl &path, QQmlAbstractUrlInterceptor::DataType type)
{
    qCInfo(logger) << "Intercepted" << path.toString() << "Type" << type << "Sandbox" << (m_locked ? "locked" : "unlocked");
    if (!m_locked) {
        m_whitelisted.insert(path);
        return path;
    } else {
        bool isWhiteListed = (m_whitelisted.find(path) != std::end(m_whitelisted));
        if (isWhiteListed) {
            qCInfo(logger) << path.toString() << "is whitelisted, letting through";
            return path;
        }
    }

    if (path.scheme() == "file") {
        if (path.path().startsWith("/ipfs/") || path.path().startsWith("/ipns/")) {
            QUrl redirected;
            /* XXXX get this from env or command line */
            redirected.setScheme("http");
            redirected.setPort(8080);
            redirected.setHost("localhost");
            redirected.setPath(path.path());
            qCInfo(logger) << "Redirected to" << redirected.toString();
            return redirected;
        }
    }

    QUrl notpermitted("notpermitted://");
    qCInfo(logger) << "Redirected to" << notpermitted.toString();
    return notpermitted;
}

