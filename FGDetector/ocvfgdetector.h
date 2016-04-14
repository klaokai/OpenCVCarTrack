#ifndef OCVFGDETECTOR_H
#define OCVFGDETECTOR_H

#include"FGDetector.h"
#include "opencv2/video/background_segm.hpp"

class OCVFGDetector :
        public FGDetector
{
    cv::BackgroundSubtractorMOG2 *bg_model;//(100, 3, 0.3, 5);
    cv::Mat m_ThresholdImg;
public:
    OCVFGDetector();
    ~OCVFGDetector();

public:
    /*创建函数*/
    virtual void Create(const cv::Mat &Source );
    /* 处理当前图片*/
    virtual void Process(const cv::Mat & pImg);
};

#endif // OCVFGDETECTOR_H
