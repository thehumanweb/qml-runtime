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
#include <QtTest/qtest.h>
#include <QSignalSpy>
#include <QQmlEngine>
#include <QQuickView>
#include "safeloader.h"
#include "safeloadertesthelper.h"
#include "formatters.h"

// The SafeLoader will be used only with QML and JS,
// as C++ will be banned. The best way to check if
// SafeLoader is not leaking is to use everything
// we have with QML/JS.
//
// The use of C++ should be limited in this series of
// tests. The SafeLoader itself will be tested with
// SafeLoaderWatcher. No call to QQmlContext, QQmlComponent
// or QQmlEngine should be done.

TEST(SafeLoaderTestHelper, Instances)
{
    QObjectPtr<SafeLoaderTestHelper> watcher1 (new SafeLoaderTestHelper());
    watcher1->setObjectName(QLatin1String("w1"));

    EXPECT_TRUE(SafeLoaderTestHelper::instances().empty());

    watcher1->classBegin();
    watcher1->componentComplete();

    EXPECT_EQ(SafeLoaderTestHelper::instances().size(), static_cast<size_t>(1));
    EXPECT_EQ(SafeLoaderTestHelper::instance(QLatin1String("w1")), watcher1.get());

    watcher1.reset();
    QTest::qWait(100);

    EXPECT_TRUE(SafeLoaderTestHelper::instances().empty());
}

TEST(SafeLoader, Simple)
{
    QQuickView view;
    view.setSource(QUrl("qrc:/tests/SafeLoaderTestParent.qml"));

    // Wait for parent SafeLoaderWatcher to be ready
    while (SafeLoaderTestHelper::instance(QLatin1String("parentHelper")) == nullptr) {
        QTest::qWait(100);
    }

    SafeLoaderTestHelper *helper = SafeLoaderTestHelper::instance(QLatin1String("parentHelper"));
    while (!helper->properties().value(QLatin1String("ready"), false).toBool()) {
        qDebug() << helper->properties();
        QTest::qWait(100);
    }

    // Now setup spies and load SafeLoaderTestChild.qml
    SafeLoader *loader = helper->loader();
    EXPECT_NE(helper->loader(), nullptr);
    QSignalSpy finishedSpy (loader, SIGNAL(finished(bool)));
    QSignalSpy sourceSpy (loader, SIGNAL(sourceChanged()));
    QSignalSpy statusSpy (loader, SIGNAL(statusChanged()));
    std::vector<SafeLoader::Status> statuses {};
    QObject::connect(loader, &SafeLoader::statusChanged, [loader, &statuses]() {
        statuses.push_back(loader->status());
    });

    helper->setProperties(QVariantMap({
        {QLatin1String("source"), QLatin1String("SafeLoaderTestChild.qml")}
    }));
    helper->notifyQml();

    while (finishedSpy.isEmpty()) {
        QTest::qWait(100);
    }

    EXPECT_EQ(finishedSpy.count(), 1);
    EXPECT_EQ(finishedSpy[0].count(), 1);
    EXPECT_EQ(finishedSpy[0][0].toBool(), true);
    EXPECT_EQ(sourceSpy.count(), 1);
    EXPECT_EQ(loader->source(), QUrl("qrc:/tests/SafeLoaderTestChild.qml"));
    EXPECT_EQ(statusSpy.count(), 2);
    EXPECT_EQ(statuses.size(), static_cast<size_t>(2));
    EXPECT_EQ(statuses[0], SafeLoader::Loading);
    EXPECT_EQ(statuses[1], SafeLoader::Ready);

    // Check if child is created
    EXPECT_NE(SafeLoaderTestHelper::instance(QLatin1String("childHelper")), nullptr);
}

TEST(SafeLoader, SimpleFail)
{
    QQuickView view;
    view.setSource(QUrl("qrc:/tests/SafeLoaderTestParent.qml"));

    // Wait for parent SafeLoaderWatcher to be ready
    while (SafeLoaderTestHelper::instance(QLatin1String("parentHelper")) == nullptr) {
        QTest::qWait(100);
    }

    SafeLoaderTestHelper *helper = SafeLoaderTestHelper::instance(QLatin1String("parentHelper"));
    while (!helper->properties().value(QLatin1String("ready"), false).toBool()) {
        qDebug() << helper->properties();
        QTest::qWait(100);
    }

    // Now setup spies and load SafeLoaderTestChildFail.qml
    SafeLoader *loader = helper->loader();
    EXPECT_NE(helper->loader(), nullptr);
    QSignalSpy finishedSpy (loader, SIGNAL(finished(bool)));
    QSignalSpy sourceSpy (loader, SIGNAL(sourceChanged()));
    QSignalSpy statusSpy (loader, SIGNAL(statusChanged()));
    std::vector<SafeLoader::Status> statuses {};
    QObject::connect(loader, &SafeLoader::statusChanged, [loader, &statuses]() {
        statuses.push_back(loader->status());
    });

    helper->setProperties(QVariantMap({
        {QLatin1String("source"), QLatin1String("SafeLoaderTestChildFail.qml")}
    }));
    helper->notifyQml();

    while (finishedSpy.isEmpty()) {
        QTest::qWait(100);
    }

    EXPECT_EQ(finishedSpy.count(), 1);
    EXPECT_EQ(finishedSpy[0].count(), 1);
    EXPECT_EQ(finishedSpy[0][0].toBool(), false);
    EXPECT_EQ(sourceSpy.count(), 1);
    EXPECT_EQ(loader->source(), QUrl("qrc:/tests/SafeLoaderTestChildFail.qml"));
    EXPECT_EQ(statusSpy.count(), 2);
    EXPECT_EQ(statuses.size(), static_cast<size_t>(2));
    EXPECT_EQ(statuses[0], SafeLoader::Loading);
    EXPECT_EQ(statuses[1], SafeLoader::Error);
}

TEST(SafeLoader, Empty)
{
    QQuickView view;
    view.setSource(QUrl("qrc:/tests/SafeLoaderTestParent.qml"));

    // Wait for parent SafeLoaderWatcher to be ready
    while (SafeLoaderTestHelper::instance(QLatin1String("parentHelper")) == nullptr) {
        QTest::qWait(100);
    }

    SafeLoaderTestHelper *helper = SafeLoaderTestHelper::instance(QLatin1String("parentHelper"));
    while (!helper->properties().value(QLatin1String("ready"), false).toBool()) {
        qDebug() << helper->properties();
        QTest::qWait(100);
    }

    // Now setup spies and load empty child
    SafeLoader *loader = helper->loader();
    EXPECT_NE(helper->loader(), nullptr);
    QSignalSpy finishedSpy (loader, SIGNAL(finished(bool)));
    QSignalSpy sourceSpy (loader, SIGNAL(sourceChanged()));
    QSignalSpy statusSpy (loader, SIGNAL(statusChanged()));
    std::vector<SafeLoader::Status> statuses {};
    QObject::connect(loader, &SafeLoader::statusChanged, [loader, &statuses]() {
        statuses.push_back(loader->status());
    });

    helper->setProperties(QVariantMap({
        {QLatin1String("source"), QLatin1String("SafeLoaderTestChild.qml")}
    }));
    helper->notifyQml();

    while (finishedSpy.isEmpty()) {
        QTest::qWait(100);
    }

    // First, create child (also test already loaded QML component)
    EXPECT_EQ(finishedSpy.count(), 1);
    EXPECT_EQ(finishedSpy[0].count(), 1);
    EXPECT_EQ(finishedSpy[0][0].toBool(), true);
    EXPECT_EQ(sourceSpy.count(), 1);
    EXPECT_EQ(loader->source(), QUrl("qrc:/tests/SafeLoaderTestChild.qml"));
    EXPECT_EQ(statusSpy.count(), 2);
    EXPECT_EQ(statuses.size(), static_cast<size_t>(2));
    EXPECT_EQ(statuses[0], SafeLoader::Loading);
    EXPECT_EQ(statuses[1], SafeLoader::Ready);

    // Check if child is created
    EXPECT_NE(SafeLoaderTestHelper::instance(QLatin1String("childHelper")), nullptr);

    finishedSpy.clear();
    sourceSpy.clear();
    statusSpy.clear();
    statuses.clear();

    // Then back to Null
    helper->setProperties(QVariantMap({
        {QLatin1String("source"), QString()}
    }));
    helper->notifyQml();

    while (finishedSpy.isEmpty()) {
        QTest::qWait(100);
    }

    EXPECT_EQ(finishedSpy.count(), 1);
    EXPECT_EQ(sourceSpy.count(), 1);
    EXPECT_EQ(loader->source(), QUrl());
    EXPECT_EQ(statusSpy.count(), 1);
    EXPECT_EQ(statuses.size(), static_cast<size_t>(1));
    EXPECT_EQ(statuses[0], SafeLoader::Null);

    // Check if child is destroyed
    EXPECT_EQ(SafeLoaderTestHelper::instance(QLatin1String("childHelper")), nullptr);
}

TEST(SafeLoader, Introspect)
{
    QQuickView view;
    view.setSource(QUrl("qrc:/tests/SafeLoaderTestParent.qml"));

    // Wait for parent SafeLoaderWatcher to be ready
    while (SafeLoaderTestHelper::instance(QLatin1String("parentHelper")) == nullptr) {
        QTest::qWait(100);
    }

    SafeLoaderTestHelper *helper = SafeLoaderTestHelper::instance(QLatin1String("parentHelper"));
    while (!helper->properties().value(QLatin1String("ready"), false).toBool()) {
        qDebug() << helper->properties();
        QTest::qWait(100);
    }

    // Now setup spies and load SafeLoaderTestChild.qml
    SafeLoader *loader = helper->loader();
    EXPECT_NE(helper->loader(), nullptr);
    QSignalSpy finishedSpy (loader, SIGNAL(finished(bool)));

    helper->setProperties(QVariantMap({
        {QLatin1String("source"), QLatin1String("SafeLoaderTestChild.qml")}
    }));
    helper->notifyQml();

    while (finishedSpy.isEmpty()) {
        QTest::qWait(100);
    }

    // Check if child is created
    SafeLoaderTestHelper *childHelper = SafeLoaderTestHelper::instance(QLatin1String("childHelper"));
    EXPECT_NE(childHelper, nullptr);

    // Perform introspection
    helper->setProperties(QVariantMap({
        {QLatin1String("introspect"), true}
    }));
    helper->notifyQml();

    while (!helper->properties().contains(QLatin1String("introspect_done"))) {
        QTest::qWait(100);
    }

    EXPECT_TRUE(helper->properties().value(QLatin1String("children")).toList().isEmpty());
    EXPECT_TRUE(helper->properties().value(QLatin1String("resources")).toList().isEmpty());
    EXPECT_NE(helper->properties().value(QLatin1String("global_object")).value<QObject *>(), nullptr);

    childHelper->setProperties(QVariantMap({
        {QLatin1String("introspect"), true}
    }));
    childHelper->notifyQml();

    while (!childHelper->properties().contains(QLatin1String("introspect_done"))) {
        QTest::qWait(100);
    }

    EXPECT_NE(childHelper->properties().value(QLatin1String("container")).value<QObject *>(), nullptr);
    EXPECT_EQ(childHelper->properties().value(QLatin1String("parent")).value<QObject *>(), helper->loader());
    EXPECT_EQ(childHelper->properties().value(QLatin1String("global_object")).value<QObject *>(), nullptr);
}
