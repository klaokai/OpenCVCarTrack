#include "Controller.h"

Controller::Controller()
{
    FGD=new OCVFGDetector();//这里改动可以变换处理
    BD=new BlobDetector();
}

void Controller::create(QString videoFilePath){
    m_SourceCap=new VideoInfo(videoFilePath);//加入车辆信息
}

//-----------------------------【Process函数】---------------------------------------------  
//  描述：取帧处理函数，控制阈值
//-----------------------------------------------------------------------------------------------  
void  Controller::Process ()
{
    namedWindow ("IMG");
    //播放控制参数
    Mat  pImg;
    if(!m_SourceCap->GetNextFrame(pImg))
        return;
    FGD->Create(pImg);
    //BD->Create(pImg,m_SourceCap->GetSize ());
    nFrmNum=1;
    while(m_SourceCap->GetNextFrame(pImg))
    {
        nFrmNum++;
#ifdef QT_DEBUG
        qDebug()<<"运行到"<<nFrmNum<<"帧";
#endif

        FGD->Process(pImg);//前景处理

        try{
            if ( FGD->GetMask().data == NULL ){
                throw "没有获得图像掩膜";
            }
//            BD->Process(FGD->GetMask());//团块处理
//            BD->Draw(pImg);//绘图处理
            imshow("IMG",FGD->GetMask());
            //emit ShowVideo(BD->GetResult());
            emit ShowVideo(FGD->GetMask());
        }
        catch ( string a ){
            cout << a;
        }
        catch ( cv::Exception a ){
            cout << a.what();
        }
        catch ( exception a ){
            cout << a.what();
        }

        cv::waitKey (m_SourceCap->GetDelay ());
    }

    //BD->Sort();//用来整理得到的车辆数据
#ifdef QT_DEBUG
    qDebug()<<"运行完毕";
#endif
    cv::destroyAllWindows ();
}

Controller::~Controller()
{
    delete BD;
    delete FGD;
    delete m_SourceCap;
}
