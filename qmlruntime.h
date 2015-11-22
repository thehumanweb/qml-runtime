#ifndef QMLRUNTIME_H
#define QMLRUNTIME_H

#include <QGuiApplication>

class QQmlComponent;
class QQmlContext;

class QMLRuntime : public QGuiApplication
{
	Q_OBJECT

	public:
	QMLRuntime(int argc, char *argv[]) : QGuiApplication(argc, argv)
	{}
	
	int startup();
	
	QQmlComponent *appcomponent;
	QQmlContext *context;

	public slots:
	void continueLoading();
};

#endif
