#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>
#include <QNetworkProxy>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("tcpclient");
    ui->portEdit->setText("8899");
    ui->addrEdit->setText("127.0.0.1");//192.168.31.98

    QLabel *textLabel = new QLabel;
    textLabel->setText("连接状态：");
    ui->statusbar->addWidget(textLabel);
    status_ = new QLabel;
    ui->statusbar->addWidget(status_);
    status_->setPixmap(QPixmap(":/fail.png").scaled(20,20));

    //1、创建tcpSocket
    socket_ = new QTcpSocket(this);
    //设置连接服务端（connectToHost()）的时候，跳过检查系统代理。防止因为系统代理导致连接失败
    socket_->setProxy(QNetworkProxy::NoProxy);

    //4、服务端发送数据后，QTcpSocket会发出readyRead信号
    connect(socket_,&QTcpSocket::readyRead,this,[=](){
        //5、读服务端数据
        QByteArray data = socket_->readAll();
        ui->recordEdit->append("服务端：" + data);
    });

    //3、连接服务器成功后，QTcpSocket会发出connected信号
    connect(socket_,&QTcpSocket::connected,this,[=](){
        status_->setPixmap(QPixmap(":/right.png").scaled(20,20));
        ui->recordEdit->append("连接服务端成功...");

        ui->connectButton->setDisabled(true);
        ui->disconnectButton->setDisabled(false);
    });

    //6、当连接断开时，QTcpSocket会发出disconnected信号。
    connect(socket_,&QTcpSocket::disconnected,this,[=](){
        //7、关闭tcpSocket。
        socket_->close();
        //socket_->deleteLater();

        ui->recordEdit->append("已断开服务器...");
        status_->setPixmap(QPixmap(":/fail.png").scaled(20,20));

        ui->connectButton->setDisabled(false);
        ui->disconnectButton->setDisabled(true);
        qDebug()<<"disconnected....";
    });

    qDebug()<<"ok....";
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectButton_clicked()
{
    unsigned short port = ui->portEdit->text().toUShort();
    QString ip = ui->addrEdit->text();
    //2、连接服务端
    socket_->connectToHost(QHostAddress(ip),port);

    qDebug()<<"start connect...."<<ip;
}

void MainWindow::on_disconnectButton_clicked()
{
    socket_->close();
    qDebug()<<"on_disconnectButton_clicked....";
}

void MainWindow::on_sendButton_clicked()
{
    QString data = ui->sendEdit->toPlainText();
    //5、向服务端写数据
    socket_->write(data.toUtf8());
    ui->recordEdit->append("客户端：" + data);
}
