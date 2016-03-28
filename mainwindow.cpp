#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&CarTrack,SIGNAL(ShowVideo(Mat)),this,SLOT(ShowResult(Mat)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_OpenImageButton_clicked()
{
    QString fileName=QFileDialog::getOpenFileName(this,tr("打开视频"),".",tr("视频文件(*.avi)"));
    QMessageBox::information(NULL, tr("路径"), tr("您选择:\n") + fileName);
    CarTrack.create(fileName);
    CarTrack.Process();
}

void MainWindow::ShowResult(Mat pImg){
    cvtColor(pImg,pImg,CV_BGR2RGB);
    QImage img=QImage((const unsigned char*)(pImg.data),pImg.cols,pImg.rows,QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(img));
    //ui->label->resize(ui->label->pixmap()->size());
}