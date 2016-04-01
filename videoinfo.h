#ifndef VIDEOINFO_H
#define VIDEOINFO_H

class QImage;
class QString;

#include "common.h"

class VideoInfo
{
public:
    explicit VideoInfo(QString videoFile);

    double GetRate() const;
    Size2d GetSize() const;
    int GetTotalFrameNumber() const;


    bool GetNextFrame(Mat& Img);

    static QImage Mat2QImage(Mat& image);

    ~VideoInfo();
    double GetDelay() const;

private:
    int calcRealTotalFrameNumber();

private:
    VideoCapture m_Capture;

    int m_totalFrameNumber;//总帧数
    double m_rate;//帧率
    Size2d m_size;//宽和高
    double m_delay;//延时
};

#endif // VIDEOINFO_H
