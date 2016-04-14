#include "ocvfgdetector.h"
#include "common.h"

OCVFGDetector::OCVFGDetector()
{
    bg_model=new BackgroundSubtractorMOG2();
}


OCVFGDetector::~OCVFGDetector()
{
    delete bg_model;
}

/**
 * @brief OCVFGDetector::Create
 * @param Source
 * @param S
 * 创建函数
 */
void OCVFGDetector::Create(const Mat &Source )
{
    assert(!Source.empty ());
}

/**
 * @brief OCVFGDetector::Process
 * @param pImg
 * 处理当前图片
 */
void OCVFGDetector::Process(const cv::Mat & pImg)
{
    assert( !pImg.empty() );

    //更新模型
    (*bg_model)(pImg ,  m_ThresholdImg);
    m_FrImg= m_ThresholdImg>10;//去除黑色部分
}
