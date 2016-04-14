#include "Controller.h"

Controller::Controller()
{
    FGD=new OCVFGDetector();//这里改动可以变换处理
    BD=new BlobDetector();
    BT=new BlobTracker();
}

void Controller::create(QString videoFilePath){
    m_SourceCap=new VideoInfo(videoFilePath);//加入车辆信息
}

//-----------------------------【Process函数】---------------------------------------------  
//  描述：取帧处理函数，控制阈值
//-----------------------------------------------------------------------------------------------  
void  Controller::Process ()
{
    namedWindow ("前景");
    moveWindow ("前景",20,30);
    //播放控制参数
    Mat  pImg;
    if(!m_SourceCap->GetNextFrame(pImg))
        return;
    FGD->Create(pImg);
    BD->Create(m_SourceCap->GetSize ());
    nFrmNum=1;
    while(m_SourceCap->GetNextFrame(pImg))
    {
        nFrmNum++;
#ifdef QT_DEBUG
        //qDebug()<<"运行到"<<nFrmNum<<"帧";
#endif

        FGD->Process(pImg);//前景处理

        try{
            if ( FGD->GetMask().data == NULL ){
                throw "没有获得图像掩膜";
            }
            BD->Process(FGD->GetMask());//团块处理
            BD->Draw(pImg);//绘图处理
            imshow("前景",FGD->GetMask());
            emit ShowVideo(BD->GetResult());
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

    BD->Sort();//用来整理得到的车辆数据
    BT->SetProbList (BD->getProcblobList ());
    BT->SetTimeParameter((m_SourceCap->GetTotalFrameNumber()/m_SourceCap->GetRate()));
    BT->ShowProbList ();
#ifdef QT_DEBUG
    qDebug()<<"运行完毕"<<(BT->GetResult());
#endif
    cv::destroyAllWindows ();
}

QString Controller::GetResult()
{
    return BT->GetResult();
}

Controller::~Controller()
{
    delete BT;
    delete BD;
    delete FGD;
    delete m_SourceCap;
}
