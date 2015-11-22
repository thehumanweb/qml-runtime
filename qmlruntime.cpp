#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>

#include "ipfsonlyurlinterceptor.h"
#include "qmlruntime.h"

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
