#include "QtWidgetsApplication3.h"
#include "ui_QtWidgetsApplication3.h"
#include <QtNetwork>
#include <QDebug>
#include <qfiledialog.h>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include <QFile>


QtWidgetsApplication3::QtWidgetsApplication3(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowTitle(QString::fromUtf8("�����������裬2018023417"));
    ui.startButton->setText("��ʼ����");
    ui.label->setText("���ü����˿�");
    ui.StatusLabel->setText("δ��ʼ����");
    ui.label_2->setText("δָ�������ļ��У�������ڹ���Ŀ¼��");
    ui.setButton->setText("���ý����ļ���");
    connect(&tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    connect(ui.startButton, &QPushButton::clicked, this, &QtWidgetsApplication3::startBtnClicked);
    connect(ui.setButton, &QPushButton::clicked, this, &QtWidgetsApplication3::selectdir);
}

void QtWidgetsApplication3::startBtnClicked()
{
    if (!tcpServer.listen(QHostAddress::LocalHost, ui.lineEdit->text().toInt())) {
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
            qDebug() << localFile;
            //localFile->setFileName("D:/" + fileName);
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
        //localFile->setFileName(dirname + fileName);
        localFile->write(inBlock);
        QFile tmp = inBlock;
        qDebug() << tmp;
        tmp.setFileName("D:/" + fileName);
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

void QtWidgetsApplication3::selectdir()
{
    dirname = QFileDialog::getExistingDirectory(NULL, "caption", ".");
    ui.label_2->setText(dirname);
}