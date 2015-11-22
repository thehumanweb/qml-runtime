#include "ipfsonlyurlinterceptor.h"

void IPFSOnlyUrlInterceptor::lock()
{	
    qInfo("Sandbox is now locked");
    foreach (const QUrl &url, this->whitelisted)
        qInfo(" whitelisted %s", url.toString().toLatin1().constData());

    sandbox_locked = true;		
}

QUrl IPFSOnlyUrlInterceptor::intercept(const QUrl &path, QQmlAbstractUrlInterceptor::DataType type)
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

