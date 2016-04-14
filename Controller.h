#include<vector>
#include <iostream>
using namespace std;

#include <QObject>

#include "common.h"

#include "CarInfo.h"
#include "videoinfo.h"

#include "FGDetector/ocvfgdetector.h"
#include "BlobDetector/BlobDetector.h"
#include "BlobTracker/blobtracker.h"

class Controller:public QObject
{
    Q_OBJECT

private:
    //--------------算法类--------------------
    FGDetector * FGD;
    BlobDetector * BD;
    BlobTracker *BT;

    //--------------数据----------------------
    VideoInfo * m_SourceCap;

    int nFrmNum=0;//运行时的当前帧


signals:
    void ShowVideo(Mat pImg);

    //------------函数------------------
public:
    //主要函数
    void create(QString videoFilePath);
    void Process();
    QString GetResult();
    Controller();
    ~Controller();
};
