#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_connect_clicked();

    void on_pushButton_send_clicked();

private:
    Ui::MainWindow *ui;

    QUdpSocket *socket_;
    uint16_t port_;
    QString cli_ip_;
    uint16_t cli_port_;
};
#endif // MAINWINDOW_H
