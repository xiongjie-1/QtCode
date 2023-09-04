#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("tcpserver");

    //1、创建QTcpServer
    tcp_ = new QTcpServer(this);
    ui->portEdit->setText("8899");

    QLabel *textLabel = new QLabel;
    textLabel->setText("连接状态：");
    ui->statusbar->addWidget(textLabel);

    status_ = new QLabel;
    status_->setPixmap(QPixmap(":/fail.png").scaled(20,20));
    ui->statusbar->addWidget(status_);

    //3、当有客户端连接时，QTcpServer会发出newConnection信号。
    connect(tcp_,&QTcpServer::newConnection,this,[=](){
        //4、获取与客户端通信的socket
        socket_ = tcp_->nextPendingConnection();

        //5、当有客户端发送数据过来时，QTcpSocket会发出readyRead信号。
        connect(socket_,&QTcpSocket::readyRead,this,[=](){
            //6、读客户端数据
            QByteArray data = socket_->readAll();
            ui->recordEdit->append("客户端：" + data);
        });

        //7、当连接断开时，QTcpSocket会发出disconnected信号。
        connect(socket_,&QTcpSocket::disconnected,this,[=](){
            //8、关闭tcp。（QTcpServer析构了，QTcpSocket就会析构，QTcpSocket是QTcpServer的子节点）
            tcp_->close();
           // tcp_->deleteLater();
            status_->setPixmap(QPixmap(":/fail.png").scaled(20,20));
            ui->listenButton->setDisabled(false);
        });

        status_->setPixmap(QPixmap(":/right.png").scaled(20,20));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_listenButton_clicked()
{
    unsigned short portNum = ui->portEdit->text().toUShort();
    //2、服务端进行bind和listen
    tcp_->listen(QHostAddress::Any, portNum);
    ui->listenButton->setDisabled(true);
}

void MainWindow::on_sendButton_clicked()
{
    QString data = ui->sendEdit->toPlainText();
    //6、向客户端写数据
    socket_->write(data.toUtf8());
    ui->recordEdit->append("服务端：" + data);
}
