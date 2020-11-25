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
    ui.StatusLabel->setText("���ü����˿�");
    connect(&tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
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

void QtWidgetsApplication3::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();
    connect(tcpServerConnection, SIGNAL(readyRead()), this, SLOT(updateServerProgress()));
    connect(tcpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    ui.StatusLabel->setText("��������");
    //�رշ��������ټ�����ֱ�ӽ��������շ�
    tcpServer.close();
}

void QtWidgetsApplication3::updateServerProgress()
{
    QDataStream in(tcpServerConnection);
    in.setVersion(QDataStream::Qt_5_7);
    // ������յ�������С��16���ֽڣ����浽�����ļ�ͷ�ṹ
    if (bytesReceived <= sizeof(qint64) * 2) 
    {
        if ((tcpServerConnection->bytesAvailable() >= sizeof(qint64) * 2) && (fileNameSize == 0)) 
        {
            // ���������ܴ�С��Ϣ���ļ�����С��Ϣ
            in >> totalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64) * 2;
        }
        if ((tcpServerConnection->bytesAvailable() >= fileNameSize) && (fileNameSize != 0)) 
        {
            // �����ļ������������ļ�
            in >> fileName;
            ui.StatusLabel->setText(tr("�����ļ� %1 ��").arg(fileName));
            bytesReceived += fileNameSize;
            localFile = new QFile(fileName);
            if (!localFile->open(QFile::WriteOnly)) 
            {
                qDebug() << "�ļ���ʧ��";
                return;
            }
        }
        else 
        {
            return;
        }
    }
    // ������յ�����С�������ݣ���ôд���ļ�
    if (bytesReceived < totalBytes) 
    {
        bytesReceived += tcpServerConnection->bytesAvailable();
        inBlock = tcpServerConnection->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }
    ui.progressBar->setMaximum(totalBytes);
    ui.progressBar->setValue(bytesReceived);

    // �����������ʱ
    if (bytesReceived == totalBytes) 
    {
        tcpServerConnection->close();
        localFile->close();
        ui.startButton->setEnabled(true);
        ui.StatusLabel->setText(tr("�����ļ� %1 �ɹ���").arg(fileName));
    }
}

void QtWidgetsApplication3::displayError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    qDebug() << tcpServerConnection->errorString();
    tcpServerConnection->close();
    ui.progressBar->reset();
    ui.StatusLabel->setText("����˾���");
    ui.startButton->setEnabled(true);
}