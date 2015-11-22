/* 
* Copyright (C) 2015 Carsten V. Munk
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
#include "qmlruntime.h"

class IPFSOnlyUrlInterceptor : public QQmlAbstractUrlInterceptor
{
private:
     bool sandbox_locked = false;
     QList<QUrl> whitelisted;
public:
     void lock()
     {	
	 qInfo("Sandbox is now locked");
	 foreach (const QUrl &url, this->whitelisted)
		qInfo(" whitelisted %s", url.toString().toLatin1().constData());

	 sandbox_locked = true;		
     }

     QUrl intercept(const QUrl &path, QQmlAbstractUrlInterceptor::DataType type) Q_DECL_OVERRIDE
     {
	 qInfo("Intercepted %s, %i, sandbox is %s", path.toString().toLatin1().constData(), type, sandbox_locked ? "locked" : "unlocked");
	 if (!sandbox_locked)
	 {
		whitelisted.append(path);
		return path;
	 }
	 else 
	 {
		bool isWhiteListed = false;
		foreach (const QUrl &url, this->whitelisted)
		{
			if (url == path)
			{
				isWhiteListed = true;	
				break;
			}
		}
		if (isWhiteListed)
		{
			qInfo("%s is whitelisted, letting through", path.toString().toLatin1().constData());
			return path;		
		}	
	 }
	 if (path.scheme().compare("file") == 0)
	 {
		if (path.path().startsWith("/ipfs/"))
		{
			QUrl redirected(path);
			/* XXXX get this from env or command line */
			redirected.setScheme("http");
			redirected.setPort(8080);
			redirected.setHost("localhost");
			qInfo("Redirected to %s", redirected.toString().toLatin1().constData());
			return redirected;
		}
		else if (path.path().startsWith("/ipns/"))
		{
			QUrl redirected(path);
			/* XXXX get this from env or command line */
			redirected.setScheme("http");
			redirected.setPort(8080);
			redirected.setHost("localhost");
			qInfo("Redirected to %s", redirected.toString().toLatin1().constData());
			return redirected;
		}
	 }
	
	 QUrl notpermitted("notpermitted://");
	 qInfo("Redirected to %s", notpermitted.toString().toLatin1().constData());
	 return notpermitted;
   }
};

int QMLRuntime::startup()
{
    QQmlEngine *engine = new QQmlEngine;
    IPFSOnlyUrlInterceptor *interceptor = new IPFSOnlyUrlInterceptor;    
    QQmlComponent *preloadcomponent = 0; 
 
    engine->setUrlInterceptor(interceptor);

    preloadcomponent = new QQmlComponent(engine, QUrl::fromLocalFile("preload.qml"), QQmlComponent::PreferSynchronous);	


    if (preloadcomponent->isError())
    {
	foreach (const QQmlError &e, preloadcomponent->errors())
	{
		qInfo("preload.qml: %s", e.toString().toLatin1().constData());
	}
	qCritical("Failed to load preload.qml, quitting");
    } 
    
    context = new QQmlContext(engine);
    QObject *obj = preloadcomponent->create(context);
    if (!obj)
	qCritical("Failed to create preload object");
    /* now in cache */
    delete obj;
    obj = 0;
    preloadcomponent->deleteLater(); 
    preloadcomponent = 0;

    // now we should switch to another user ? or in start?
    // now sandboxed for URIs
    interceptor->lock();

    this->appcomponent = new QQmlComponent(engine, QUrl(this->arguments().at(1)));
    if (this->appcomponent->isLoading())
    {
	 QObject::connect(this->appcomponent, SIGNAL(statusChanged(QQmlComponent::Status)),
                         this, SLOT(continueLoading()));
    }
    return this->exec();
}

void QMLRuntime::continueLoading()
{
    if (this->appcomponent->isError())
    {
	foreach (const QQmlError &e, this->appcomponent->errors())
	{
		qInfo("App component: %s", e.toString().toLatin1().constData());
	}
	qCritical("Failed to load app component, quitting"); 
    }
    
    QObject *appobj = this->appcomponent->create(context);   
 
    if (!appobj)
    {
	qCritical("Failed to create app object"); 
    }
}

int main(int argc, char *argv[])
{
	QMLRuntime runtime(argc, argv);
	return runtime.startup();
}
