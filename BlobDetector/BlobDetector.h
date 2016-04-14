#ifndef  MYBLOBDETECTOR_H
#define  MYBLOBDETECTOR_H

#include <vector>
#include "CarInfo.h"

class BlobDetector{

	//数据
private:

    cv::Mat pI;

    cv::Size m_SourceSize;//源图像大小

    const int m_Maxinone;//最大保留团块数量
    bool isnFrmNumZero;//是否为第一帧
    unsigned int m_NextBlobID;//ID号分配

    std::vector<CarInfo> m_BlobListOld;//前一帧的团块
    std::vector<CarInfo> m_BlobListNow;//当前帧的团块
    std::vector<CarInfo> HistoryBlobList;//所有有记录的团块
    std::vector<CarInfo> SameblobList;//临时存储某个ID的全部信息
    std::vector<std::vector <CarInfo> > ProcblobList;//运行完成后把全部车辆信息以ID编号保存轨迹

private:
	//算法运行时所要使用到的方法
    void DetectFrBlob(const cv::Mat &m_FrImg);
	void DetectANamedNewBlob();
	void TempGetBlobByID(int BlobID);
	
	//用户可以调用的函数
public:
	BlobDetector();
    void Create(const cv::Size S);//创建函数
    void Process(const cv::Mat &FrMask);//处理当前图片
    cv::Mat GetMask();//获取处理后的前景图片
    void Draw(cv::Mat &pImg);//用矩形框出每一个轮廓
	void Sort();//整理所有数据，准备分析
    std::vector<CarInfo> GetBlobByID(int BlobID);

    cv::Mat GetResult(){return pI;}
    std::vector<std::vector<CarInfo> > getProcblobList() const;
};
#endif
