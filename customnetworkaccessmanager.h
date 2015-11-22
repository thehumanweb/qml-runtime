#ifndef CUSTOMNETWORKACCESSMANAGER_H
#define CUSTOMNETWORKACCESSMANAGER_H

#include <QQmlNetworkAccessManagerFactory>
#include <QNetworkAccessManager>

class CustomNetworkAccessManagerFactory : public QQmlNetworkAccessManagerFactory {
    public:
        virtual QNetworkAccessManager *create(QObject *);
        CustomNetworkAccessManagerFactory();

    private:
        static QNetworkAccessManager *customNetworkAccessManager;
};

class CustomNetworkAccessManager : public QNetworkAccessManager {
    public:
        CustomNetworkAccessManager(QObject *parent=0);
    protected:
        QNetworkReply *createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0);

    private:
        bool isLocalHost(QString);
};

#endif
