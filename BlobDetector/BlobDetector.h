#ifndef  MYBLOBDETECTOR_H
#define  MYBLOBDETECTOR_H

#include <vector>
#include "CarInfo.h"

class BlobDetector{

	//数据
private:
    //Mat m_FrImg;
	vector<CarInfo> m_BlobListOld;//前一帧的团块
	vector<CarInfo> m_BlobListNow;//当前帧的团块
	vector<CarInfo> HistoryBlobList;//所有有记录的团块
	vector<CarInfo> SameblobList;//临时存储某个ID的全部信息
	vector<vector <CarInfo> > ProcblobList;//运行完成后把全部车辆信息以ID编号保存轨迹
	
	const int m_Maxinone;//最大保留团块数量
	unsigned int nFrmNum;//帧数
	unsigned int m_NextBlobID;//ID号分配

	Size m_SourceSize;//源图像大小

    Mat pI;

private:
	//算法运行时所要使用到的方法
    void DetectFrBlob(const Mat &m_FrImg);
	void DetectANamedNewBlob();
	void TempGetBlobByID(int BlobID);
	
	//用户可以调用的函数
public:
	BlobDetector();
	void Create(Mat &Source,const Size S);//创建函数
	void Process(const cv::Mat &);//处理当前图片
	Mat GetMask();//获取处理后的前景图片
    void Draw(cv::Mat &);//用矩形框出每一个轮廓
	void Sort();//整理所有数据，准备分析
	vector<CarInfo> GetBlobByID(int BlobID);

    Mat GetResult(){return pI;}
};
#endif
