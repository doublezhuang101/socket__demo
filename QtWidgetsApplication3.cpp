#include "QtWidgetsApplication3.h"
#include "ui_QtWidgetsApplication3.h"
#include <QtNetwork>
#include <QDebug>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif


QtWidgetsApplication3::QtWidgetsApplication3(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowTitle(QString::fromUtf8("�����������裬2018023417"));
    ui.startButton->setText("��ʼ����");
    connect(ui.startButton, &QPushButton::clicked, this, &QtWidgetsApplication3::startBtnClicked);

}

void QtWidgetsApplication3::startBtnClicked()
{
    if (!tcpServer.listen(QHostAddress::LocalHost, 10086)) {
        qDebug() << tcpServer.errorString();
        close();
        return;
    }
    ui.startButton->setEnabled(false);
    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;
    ui.StatusLabel->setText("������");
    ui.progressBar->reset();
}