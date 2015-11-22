#ifndef IPFSONLYURLINTERCEPTOR_H
#define IPFSONLYURLINTERCEPTOR_H

#include <QQmlAbstractUrlInterceptor>

class IPFSOnlyUrlInterceptor : public QQmlAbstractUrlInterceptor
{
public:
    void lock();
    QUrl intercept(const QUrl &path, QQmlAbstractUrlInterceptor::DataType type) Q_DECL_OVERRIDE;

private:
     bool sandbox_locked = false;
     QList<QUrl> whitelisted;
};

#endif
