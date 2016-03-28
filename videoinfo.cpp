#include "videoinfo.h"

#include <QImage>
#include <QDebug>
VideoInfo::VideoInfo(QString videoFile)
{
    //打开视频
    m_Capture.open(videoFile.toLocal8Bit().data());
    //检查视频是否成功打开
    if ( !m_Capture.isOpened() )
        qWarning(  "视频未正确打开" );
    else
    {
        //获取整个帧数
        m_totalFrameNumber = m_Capture.get(CV_CAP_PROP_FRAME_COUNT);
        //获取视频帧率
        m_rate = m_Capture.get(CV_CAP_PROP_FPS);
        //视频流的宽度
        m_size = cv::Size(m_Capture.get(CV_CAP_PROP_FRAME_WIDTH) ,m_Capture.get(CV_CAP_PROP_FRAME_HEIGHT));
#ifdef QT_DEBUG
        qDebug() <<"整个视频共" << m_totalFrameNumber << "帧\n"<< "帧率为:" << m_rate <<  "帧/秒\n" << "大小为:"  << m_size.width <<'*'<<m_size.height;
#endif
    }
}

double VideoInfo::GetRate() const{
    return m_rate;
}

Size2d VideoInfo::GetSize() const{
    return m_size;
}

bool VideoInfo::GetNextFrame(Mat &Img){
    return m_Capture.read(Img) ;
}

QImage VideoInfo::Mat2QImage(Mat &image)
{

    QImage img;

    if (image.channels()==3) {
        cvtColor(image, image, CV_BGR2RGB);
        img = QImage((const unsigned char *)(image.data), image.cols, image.rows,
                     image.cols*image.channels(), QImage::Format_RGB888);
    } else if (image.channels()==1) {
        img = QImage((const unsigned char *)(image.data), image.cols, image.rows,
                     image.cols*image.channels(), QImage::Format_ARGB32);
    } else {
        img = QImage((const unsigned char *)(image.data), image.cols, image.rows,
                     image.cols*image.channels(), QImage::Format_RGB888);
    }

    return img;
}

long VideoInfo::GetTotalFrameNumber() const
{
    return m_totalFrameNumber;
}



