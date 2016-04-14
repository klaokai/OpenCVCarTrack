#ifndef  FGDETECTOR_H
#define  FGDETECTOR_H

#include "opencv2/imgproc/imgproc.hpp" 

class FGDetector{
protected:
    cv::Mat m_FrImg;//前景图片（只有车辆）

public:
    /*创建函数*/
    virtual void Create(const cv::Mat &Source)=0;
    /* 处理当前图片*/
    virtual void Process(const cv::Mat &pImg)=0;
    /*
     * 销毁函数
     */
    virtual ~FGDetector(){};
    /* 获取处理后的前景图片*/
    const cv::Mat GetMask();
};

#endif
