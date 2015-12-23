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

#ifndef SAFELOADERTESTHELPER_H
#define SAFELOADERTESTHELPER_H

#include <QObject>
#include <QQmlParserStatus>
#include <QVariantMap>

class SafeLoader;
class SafeLoaderTestHelper : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(SafeLoader * loader READ loader WRITE setLoader NOTIFY loaderChanged)
    Q_PROPERTY(QVariantMap properties READ properties WRITE setProperties NOTIFY propertiesChanged)
public:
    explicit SafeLoaderTestHelper(QObject *parent = nullptr);
    ~SafeLoaderTestHelper();
    static const std::map<QString, SafeLoaderTestHelper *> & instances();
    static SafeLoaderTestHelper * instance(const QString &name);
    void classBegin() override;
    void componentComplete() override;
    SafeLoader * loader() const;
    void setLoader(SafeLoader *loader);
    QVariantMap properties() const;
    void setProperties(const QVariantMap &properties);
signals:
    void propertiesChanged();
    void loaderChanged();
    void notifyQml();
    void notifyTest();
private:
    static std::map<QString, SafeLoaderTestHelper *> & mutableInstances();
    QVariantMap m_properties {};
    SafeLoader * m_loader {nullptr};
};

#endif // SAFELOADERTESTHELPER_H
