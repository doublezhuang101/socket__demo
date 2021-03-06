#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication3.h"
#include <qtcpserver.h>
#include <QAbstractSocket>
#include <QTcpServer>
class QFile;
class QTcpSocket;

class QtWidgetsApplication3 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication3(QWidget *parent = Q_NULLPTR);
    void file_save(QString x, QString dis);
    void file_copy(QString filename, QString dis);
public slots:
    void startBtnClicked();
    void acceptConnection();
    void updateServerProgress();
    void displayError(QAbstractSocket::SocketError socketError);
    void selectdir();
    //void file_copy(QString filename, QString dis);
private:
    Ui::QtWidgetsApplication3Class ui;
    QTcpSocket* tcpServerConnection;
    QTcpServer tcpServer;
    qint64 totalBytes;
    qint64 bytesReceived;
    qint64 fileNameSize;
    QString fileName;
    QFile* localFile;
    QByteArray inBlock;
    QString dirname;
    QString name;
    QString dis_tmp;
};
