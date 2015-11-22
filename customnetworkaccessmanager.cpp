#include <QUrl>
#include <QNetworkRequest>

#include "customnetworkaccessmanager.h"

QNetworkAccessManager *CustomNetworkAccessManagerFactory::customNetworkAccessManager = 0;

CustomNetworkAccessManagerFactory::CustomNetworkAccessManagerFactory() {
    customNetworkAccessManager = new CustomNetworkAccessManager();
}

QNetworkAccessManager * CustomNetworkAccessManagerFactory::create(QObject *parent) {
    Q_UNUSED(parent)
    return customNetworkAccessManager;
}

CustomNetworkAccessManager::CustomNetworkAccessManager(QObject *parent):QNetworkAccessManager(parent) {

}

QNetworkReply *CustomNetworkAccessManager::createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData) {
    QUrl url = req.url();
    QNetworkRequest newRequest(req);
  
    if (url.scheme() != "http" || !isLocalHost(url.host()) || url.port() != 8080) {
        qWarning() << "Application is trying to access non-IPFS service. Host is " << url.scheme() << "://" <<url.host() << ":" << url.port();
        // Access denied. Redirect to an IPFS object representing ipfs error
        newRequest.setUrl(QUrl(QString("")));
    }

    return QNetworkAccessManager::createRequest(op, newRequest, outgoingData);
}

bool CustomNetworkAccessManager::isLocalHost(QString host) {
    return host == "localhost" || host == "127.0.0.1";
}
