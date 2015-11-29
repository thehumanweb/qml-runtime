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
#include <QGuiApplication>
#include <QLoggingCategory>

// Wrapper object used to wrap gtest around the Qt event loop
class WrapperObject: public QObject
{
public:
    explicit WrapperObject(int &argc, char **argv)
        : m_argc(argc), m_argv(argv)
    {
    }
    void run()
    {
        testing::InitGoogleTest(&m_argc, m_argv);
        m_returnCode = RUN_ALL_TESTS();
        QCoreApplication::instance()->postEvent(this, new QEvent(QEvent::User));
    }
protected:
    bool event(QEvent *e) override
    {
        if (e->type() == QEvent::User) {
            QCoreApplication::exit(m_returnCode);
            return true;
        } else {
            return QObject::event(e);
        }
    }
private:
    int &m_argc;
    char **m_argv {nullptr};
    int m_returnCode {0};
};

static const char *FILTER_RULES = "url-interceptor.info=false\n"
                                  "network-access-manager.info=false\n"
                                  "network-access-manager.warning=false\n"
                                  "qml-runtime.info=false\n"
                                  "qml-runtime.warning=false";

int main(int argc, char **argv)
{
    QLoggingCategory::setFilterRules(FILTER_RULES);

    QGuiApplication app (argc, argv);
    WrapperObject object {argc, argv};
    object.run();
    return app.exec();
}

