/*
 * Copyright (C) 2015 Lucien XU <sfietkonstantin@free.fr>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * The names of its contributors may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include "safeloadertesthelper.h"
#include "safeloader.h"

SafeLoaderTestHelper::SafeLoaderTestHelper(QObject *parent)
    : QObject(parent)
{
}

SafeLoaderTestHelper::~SafeLoaderTestHelper()
{
    mutableInstances().erase(objectName());
}

const std::map<QString, SafeLoaderTestHelper *> & SafeLoaderTestHelper::instances()
{
    return mutableInstances();
}

SafeLoaderTestHelper * SafeLoaderTestHelper::instance(const QString &name)
{
    std::map<QString, SafeLoaderTestHelper *>::const_iterator it = instances().find(name);
    return it != instances().end() ? it->second : nullptr;
}

void SafeLoaderTestHelper::classBegin()
{
}

void SafeLoaderTestHelper::componentComplete()
{
    mutableInstances().insert({objectName(), this});
}

SafeLoader * SafeLoaderTestHelper::loader() const
{
    return m_loader;
}

void SafeLoaderTestHelper::setLoader(SafeLoader *loader)
{
    if (m_loader != loader) {
        m_loader = loader;
        emit loaderChanged();
    }
}

QVariantMap SafeLoaderTestHelper::properties() const
{
    return m_properties;
}

void SafeLoaderTestHelper::setProperties(const QVariantMap &properties)
{
    if (m_properties != properties) {
        m_properties = properties;
        emit propertiesChanged();
    }
}

std::map<QString, SafeLoaderTestHelper *> & SafeLoaderTestHelper::mutableInstances()
{
    static std::map<QString, SafeLoaderTestHelper *> s_instances {};
    return s_instances;
}

