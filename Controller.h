#include<vector>
#include <iostream>
using namespace std;

#include <QObject>

#include "common.h"

#include "CarInfo.h"
#include "videoinfo.h"
#include "FGDetector/FGDetector.h"
#include "FGDetector/SimpleFGDetector.h"
//#include "FGDetector/VibeFGDetector.h"
#include "BlobDetector/BlobDetector.h"

class Controller:public QObject
{
    Q_OBJECT

private:
    //--------------算法类--------------------
    FGDetector *FGD;
    BlobDetector *BD;

    //--------------数据----------------------
    VideoInfo*m_SourceCap;

    int nFrmNum;//运行时的当前帧


signals:
    void ShowVideo(Mat pImg);

    //------------函数------------------
public:
    //主要函数
    void create(QString videoFilePath);
    void Process();
    Controller();
    ~Controller();
};
