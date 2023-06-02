#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("服务器");
    ui->port->setText("8899");
    //创建监听的服务器对象
    m_s=new QTcpServer(this);

    //等待连接
    connect(m_s,&QTcpServer::newConnection,this,[=]()
    {
        m_tcp=m_s->nextPendingConnection();
        m_status->setText("已连接");

        //检查是否可以接收数据
        connect(m_tcp,&QTcpSocket::readyRead,this,[=]()
        {
            QByteArray data=m_tcp->readAll();
            ui->record->append("客户端："+data);
        });

        //断开连接
        connect(m_tcp,&QTcpSocket::disconnected,this,[=]()
        {
            m_tcp->close();
            m_tcp->deleteLater();
            m_status->setText("未连接");
        });

    });

    //状态栏
    m_status=new QLabel(this);
    m_status->setText("未连接");
    ui->statusbar->addWidget(m_status);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//启动监听
void MainWindow::on_setlisten_clicked()
{
    unsigned short port =ui->port->text().toUShort();
    m_s->listen(QHostAddress::Any,port);
    ui->setlisten->setDisabled(true);
}

//发送信息
void MainWindow::on_Button_send_clicked()
{
    QString msg=ui->send->toPlainText();
    m_tcp->write(msg.toUtf8());
    ui->record->append("服务器："+msg);
}

