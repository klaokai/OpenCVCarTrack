#ifndef  FGDETECTOR_H
#define  FGDETECTOR_H

#include "opencv2/imgproc/imgproc.hpp" 
using namespace cv;

class FGDetector{
protected:
    Mat m_FrImg;//前景图片（只有车辆）

	Size m_SourceSize;//源图像大小
	unsigned int frameNum;//现在第几帧

public:
	/*创建函数*/
	virtual void Create(Mat &Source,const Size S)=0;
	/* 处理当前图片*/
	virtual void Process(const cv::Mat &)=0;
	/* 获取处理后的前景图片*/
	Mat GetMask();
};

#endif
