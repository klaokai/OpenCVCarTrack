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
    long GetTotalFrameNumber() const;

    bool GetNextFrame(Mat& Img);

    static QImage Mat2QImage(Mat& image);

private:
    VideoCapture m_Capture;

    long m_totalFrameNumber;//总帧数
    double m_rate;//帧率
    Size2d m_size;//宽和高
};

#endif // VIDEOINFO_H
