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

#ifndef QOBJECTPTR_H
#define QOBJECTPTR_H

#include <memory>
#include <QObject>

/**
 * @brief A deleter for QObjects
 */
template<class T>
class QObjectDeleter
{
public:
    void operator()(T *object)
    {
        object->deleteLater();
    }
};

/**
 * QObject based std::unique_ptr
 */
template<class T>
using QObjectPtr = std::unique_ptr<T, QObjectDeleter<T>>;

#endif // QOBJECTPTR_H
