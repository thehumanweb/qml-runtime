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

#ifndef IPFSONLYURLINTERCEPTOR_H
#define IPFSONLYURLINTERCEPTOR_H

#include "ilockableurlinterceptor.h"
#include <set>

class IpfsOnlyUrlInterceptor : public ILockableUrlInterceptor
{
public:
    explicit IpfsOnlyUrlInterceptor() = default;
    void lock();
    QUrl intercept(const QUrl &path, QQmlAbstractUrlInterceptor::DataType type) override;
private:
     bool m_locked {false};
     std::set<QUrl> m_whitelisted {};
};

#endif
