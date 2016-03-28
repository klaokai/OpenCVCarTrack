﻿#ifndef  SIMPLEFGDETECTOR_H
#define  SIMPLEFGDETECTOR_H

#include "FGDetector.h"

class Histogram1D{
private:
	int histSize[1];//项的数量
	float hranges[2];//像素的最小及最大值
	const float*ranges[1];
	int channels[1];//仅用一个通道
	Mat hist;
public:

	Histogram1D(){
		//准备1D直方图的参数
		histSize[0]=256;
		hranges[0]=0.0;
		hranges[1]=255.0;
		ranges[0]=hranges;
		channels[0]=0;//默认情况，考察0号通道
	}

	Mat getHistogram(const Mat &image){
		//计算直方图
		calcHist(&image,
			1,//计算单张图像的直方图
			channels,//通道数量
			Mat(),//不使用图像作为掩码
			hist,//返回的直方图
			1,//这是1D的直方图
			histSize,//项的数量
			ranges//像素值范围
			);
		return hist;
	}
	void drawHist(){
		namedWindow("Histogram Demo");
		// 创建直方图画布
		int hist_w = 400; int hist_h = 400;
		int histSize = 255;
		int bin_w = cvRound( (double) hist_w/histSize );
		Mat histImage(hist_w,hist_h,hist.depth());
		Mat temp;
		/// 将直方图归一化到范围 [ 0, histImage.rows ]
		normalize(hist,temp, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

		for( int i = 1; i < histSize; i++ )
		{
			line( histImage, Point( bin_w*(i-1), hist_h - cvRound(temp.at<float>(i-1)) ) ,
                          Point( bin_w*(i), hist_h - cvRound(temp.at<float>(i)) ),
                          Scalar( 255, 255, 255), 2, 8, 0   );
		}
		imshow("Histogram Demo", histImage );
	}
};



class SimpleFGDetector:virtual public FGDetector{
private:
	Mat m_BkImg;//背景图片（只有车辆以外的其他物体）
	

	//下面是为了更精确的图像处理计算而准备的
	//分别对应上面四个数据
	Mat pFrameMat;
	Mat pBKfirstMat;
	Mat	pBkMat;
	Mat	pFrMat;

	int Otsuthreshold;
	Histogram1D Hist;

private:
	void Otsu(Mat & frame);
	void RenewBKImage();

public:
	/*创建函数*/
	void Create(Mat &Source,const Size S);
	/* 处理当前图片*/
	void Process(const cv::Mat &);
	
};
#endif
