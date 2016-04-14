#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CarTrack=new Controller();

    connect(CarTrack,SIGNAL(ShowVideo(Mat)),this,SLOT(ShowResult(Mat)));
}

MainWindow::~MainWindow()
{
    delete CarTrack;
    delete ui;
}

void MainWindow::on_OpenImageButton_clicked()
{
    QString fileName=QFileDialog::getOpenFileName(this,tr("打开视频"),"../Test",tr("视频文件(*.avi)"));
    QMessageBox::information(NULL, tr("路径"), tr("您选择:\n") + fileName);
    CarTrack->create(fileName);
    CarTrack->Process();
    QMessageBox::information(NULL, tr("结果"), CarTrack->GetResult());
}

void MainWindow::ShowResult(Mat pImg){
    ui->label->setPixmap(QPixmap::fromImage(VideoInfo::Mat2QImage (pImg)));
    ui->label->resize(ui->label->pixmap()->size());
}
