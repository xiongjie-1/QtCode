#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("udpServer");
    ui->lineEdit_port->setText("8899");

    //1、创建套接字
    socket_ = new QUdpSocket(this);

    //3、读客户端数据
    connect(socket_,&QUdpSocket::readyRead,this,[=](){
        qDebug()<<"readyRead!";
        // 通过函数判断当前是否有等待读取的数据并循环获取
        while(socket_->hasPendingDatagrams())
        {
            //创建数据存储容器，并设置长度为将接收的数据长度
            QByteArray data;
            QHostAddress cli_addr;  //对方地址
            data.resize(socket_->pendingDatagramSize());
            //读取数据并保存信息发送者的地址和ip(方便发送时指定发送位置)
            socket_->readDatagram(data.data(), data.size(), &cli_addr, &cli_port_);
            cli_ip_ = cli_addr.toString();
            ui->textEdit_record->append(cli_ip_ + ":" + QString::number(cli_port_) + ":" + data);
            qDebug()<<"client ip:" <<cli_ip_ << "port:"<< cli_port_;
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_connect_clicked()
{
    port_ = ui->lineEdit_port->text().toUInt();
    //2、bind服务端ip和端口号
    socket_->bind(QHostAddress::Any, port_);

    ui->pushButton_connect->setEnabled(false);
}

void MainWindow::on_pushButton_send_clicked()
{
    QString str = ui->textEdit_send->toPlainText();
    //3、给指定的IP发送数据
    socket_->writeDatagram(str.toUtf8(), QHostAddress(cli_ip_), cli_port_);
    ui->textEdit_record->append("本端发送:" + str);

    ui->textEdit_send->clear();
}
