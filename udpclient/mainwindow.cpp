#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("udpClient");
    ui->lineEdit_ip->setText("192.168.31.98");
    ui->lineEdit_port->setText("8899");

    socket_ = NULL;

    //2、读服务端数据
    connect(socket_,&QUdpSocket::readyRead,this,[=](){
        qDebug()<<"readyRead!...";
        // 通过函数判断当前是否有等待读取的数据并循环获取
        while(socket_->hasPendingDatagrams())
        {
            //创建数据存储容器，并设置长度为将接收的数据长度
            QByteArray data;
            QHostAddress server_addr;  //服务端地址
            data.resize(socket_->pendingDatagramSize());
            //读取数据并保存信息发送者的地址和ip(方便发送时指定发送位置)
            socket_->readDatagram(data.data(), data.size(), &server_addr, &server_port_);
            //cli_ip_ = server_addr.toString();
            ui->textEdit_record->append(server_addr.toString() + ":" + QString::number(server_port_) + "：" + data);
            qDebug()<<"server ip:" <<server_addr.toString() << "port:"<< server_port_;
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_connect_clicked()
{
    if(!socket_){
        //1、创建套接字
        socket_ = new QUdpSocket();
        server_ip_ = ui->lineEdit_ip->text();
        server_port_ = ui->lineEdit_port->text().toUInt();
        ui->pushButton_connect->setEnabled(false);


    }

}

void MainWindow::on_pushButton_disconnect_clicked()
{
    //3、关闭连接
    socket_->close();
    socket_->deleteLater();
    disconnect(socket_,&QUdpSocket::readyRead,this, nullptr);

    socket_ = NULL;
    ui->pushButton_connect->setEnabled(true);
}

void MainWindow::on_pushButton_send_clicked()
{
    if(socket_){
        QString str = ui->textEdit_send->toPlainText();
        //2、给指定的IP发送数据
        socket_->writeDatagram(str.toUtf8(), QHostAddress(server_ip_), server_port_);
        ui->textEdit_send->clear();
        qDebug()<<"btn_send :"<< str;

        ui->textEdit_record->append("本端发送：" + str);
    }
}
