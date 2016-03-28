#include "Controller.h"

Controller::Controller()
{
    FGD=new SimpleFGDetector();//这里改动可以变换处理
    BD=new BlobDetector();
}

void Controller::create(QString videoFilePath){
    //SourceCap=pCap;
    //获取帧率
    //rate=SourceCap.get(CV_CAP_PROP_FPS);
    //获取大小
    //S=Size((int)SourceCap.get(CV_CAP_PROP_FRAME_WIDTH),(int)SourceCap.get(CV_CAP_PROP_FRAME_HEIGHT));
    m_SourceCap=new VideoInfo(videoFilePath);//加入车辆信息
}

//-----------------------------【Process函数】---------------------------------------------  
//  描述：取帧处理函数，控制阈值
//-----------------------------------------------------------------------------------------------  
void  Controller::Process ()
{
    //播放控制参数
    Mat  pImg;
    for(nFrmNum=0;cvWaitKey(m_SourceCap->GetRate ()),nFrmNum<=m_SourceCap->GetTotalFrameNumber ();nFrmNum++)
    {
        //*******************第一帧准备数据********************************
#ifdef QT_DEBUG
        qDebug()<<"运行到"<<nFrmNum<<"帧";
#endif
        if(nFrmNum==0){
            if(!m_SourceCap->GetNextFrame(pImg))
                break;
            FGD->Create(pImg,m_SourceCap->GetSize ());
            BD->Create(pImg,m_SourceCap->GetSize ());
        }
        //*********************************************************************

        if(!m_SourceCap->GetNextFrame(pImg))
            break;//读入图像

        FGD->Process(pImg);//前景处理
        if(nFrmNum>200){
            BD->Process(FGD->GetMask());//团块处理
            BD->Draw(pImg);//绘图处理
            emit ShowVideo(BD->GetResult());
        }

    }
    BD->Sort();//用来整理得到的车辆数据
}

Controller::~Controller()
{
    delete BD;
    delete m_SourceCap;
}
