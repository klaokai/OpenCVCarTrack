#include "BlobDetector.h"
#include "common.h"

BlobDetector::BlobDetector():m_Maxinone(8),isnFrmNumZero(true),m_NextBlobID(0){

}
//------------------------【BlobDetector::Create函数】--------------------------------
//  描述：准备1个观察窗口，并准备原始图像的拷贝
//-----------------------------------------------------------------------------------------------
void BlobDetector::Create(const Size S)
{
    BlobDetector();
    m_SourceSize=S;
    m_NextBlobID=0;//ID号分配
}


//--------------------【BlobDetector::DetectFrBlob函数】---------------------------
//  描述：准备1个单通道的前景图像pIB用查找轮廓，只提取最大的10个轮廓，
//其余太小的舍去
//----------------------------------------------------------------------------------------------
vector<vector<CarInfo> > BlobDetector::getProcblobList() const
{
    return ProcblobList;
}

void BlobDetector::DetectFrBlob(const Mat &m_FrImg){
    //准备数据
    m_BlobListNow.clear();
    Mat pIB (m_SourceSize,CV_8UC1);
    m_FrImg.convertTo(pIB,CV_8UC1);//在findContours时保护图像m_FrImg不被修改，所以拷贝一个图像出来
    vector<CarInfo> Blobs;//m_BlobListNow存储前10个最大blob，Blobs存储全部的blob
    vector< vector<Point> > contours;//轮廓的数组
    contours.clear();

    //从二值图像中检索轮廓，存储轮廓容器contours 并返回检测到的轮廓头指针
    //findContour设置轮廓的大小 CV_RETR_EXTERNAL为检测方式，只是找出最大的外轮廓
    findContours( pIB,contours,  CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

    //接下来就是对每一个轮廓的坐标长宽，作为团块存储
    for(vector<int>::size_type i=0; i<contours.size(); i++)
    {
        CarInfo  NewBlob;
        Rect  r = boundingRect(Mat(contours.at((int)i)));//获得最小包围矩形
        //这里的判断是为了忽略较小的团块，这里的【数值】可以修改为合适的值
        if(r.height < m_SourceSize.height*0.1 || r.width < m_SourceSize.width*0.08) continue;
        NewBlob = CarInfo(r.x,r.y,r.width,r.height);
        Blobs.push_back(NewBlob);//放入Blobs
    }

    //将当前的团块冒泡排序，由大到小
    int N=Blobs.size();
    for(int i=1; i<N; ++i)
    {
        for(int j=i; j>0; --j)
        {
            CarInfo  temp;
            float   AreaP, AreaN;
            CarInfo* pP = &Blobs.at(j-1);
            CarInfo* pN = &Blobs.at(j);
            AreaP = pP->getArea();
            AreaN = pN->getArea();
            if(AreaN < AreaP)break;
            temp = *pN;
            *pN = *pP;
            *pP = temp;
        }
    }

    //只保留最大的m_Maxinone(默认是8，但不超过8)个团块
    for(int i=0; i<MIN(N,m_Maxinone); ++i)
    {
        m_BlobListNow.push_back(Blobs.at(i));
    }
}

//----------【BlobDetector::DetectANamedNewBlob函数】-----------------------
//  描述：在DetectFrBlob的基础之上整理团块的序号，旧的团块沿袭序号，
//新的团块增加新的编号
//-----------------------------------------------------------------------------------------------
void BlobDetector::DetectANamedNewBlob(){
    //将pNowBlobList到pOldBlobList距离最近的距离存进Now[i][0]
    const int NowSize=m_BlobListNow.size();
    const int OldSize=m_BlobListOld.size();

    if(NowSize==0){
#ifdef QT_DEBUG
        qDebug()<<"当前图像中没有检测到车辆";
#endif
        return ;
    }

    int Now[m_Maxinone+1][m_Maxinone+1];//临时存储距离数据
    //清空操作
    for(int i=0;i<m_Maxinone+1;i++)
        for(int j=0;j<m_Maxinone+1;j++)
            Now[i][j]=m_SourceSize.height+m_SourceSize.width;

    for(int i = NowSize;i>0;i--){
        CarInfo pBNow=m_BlobListNow.at(i-1);
        Now[i][0]=m_SourceSize.height+m_SourceSize.width;//图像的宽高之和（只是为了防止数据为空）
        for(int j = OldSize;j>0;j--){
            CarInfo pBOld=m_BlobListOld.at(j-1);
            Now[i][j]=(int)pBNow.BlobDistance(pBOld);//计算两个团块的最小外接矩形中心距离
            Now[i][0]=Now[i][0]>Now[i][j]?Now[i][j]:Now[i][0];//最小的距离放在每一行开头
        }
    }

    //求出当前帧团块中与以前的最近团块
    int TempNow[m_Maxinone+1];//用来记录最近团块的序号
    for(int i=1;i<m_Maxinone+1;i++)
        for(int j=1;j<m_Maxinone+1;j++)
            if(Now[i][j]==Now[i][0])
                TempNow[i]=j;//离i最近的团块在序号为j

    //如果某两个pNowBlobList中的团块的到pOldBlobList中的最近团块是同一个，则保留最小的
    //这里可以用一个一维数组记录比较的结果
    for(int i =NowSize;i>0;i--){
        //下面循环作用是找之前已存在的距离防止出现两个最近是都是的情况
        for(int j=i-1;j>0;j--){
            //如果k==l，则删除比较远的那个，保留最近的，远的视为新团块
            if(TempNow[i]==TempNow[j]&&Now[i][0]<Now[j][0]){
                Now[j][0]=m_SourceSize.height+m_SourceSize.width;
            }
            else if(TempNow[i]==TempNow[j]&&Now[i][0]>=Now[j][0]){
                Now[i][0]=m_SourceSize.height+m_SourceSize.width;
            }
            if(Now[i][0]>20)Now[i][0]=m_SourceSize.height+m_SourceSize.width;//舍弃当前距离大于20的，视为新团块
        }
    }

    //为所有的团块编号
    for(int i = NowSize;i>0;i--){
        //将"没有"最近距离的团块视为新团块，编号
        if(Now[i][0]==m_SourceSize.height+m_SourceSize.width){
            CarInfo *pB=&m_BlobListNow.at(i-1);
            pB->setID (m_NextBlobID);//这个是首次检测到的团块，不是当前图像的团块（即“出现”的新团块）
            m_NextBlobID++;
        }
        //将有最近团块的团块ID统一
        else{
            CarInfo *pBNow=&m_BlobListNow.at(i-1);
            CarInfo *pBOld=&m_BlobListOld.at(TempNow[i]-1);
            pBNow->setID(pBOld->getID());//旧的团块编号沿袭到新的
        }
    }
}

//------------------------【BlobDetector::Process函数】------------------------------
//  描述：先查找本帧轮廓DetectFrBlob，再进行比对处理以更新编号
//DetectANamedNewBlob 
//-----------------------------------------------------------------------------------------------
void BlobDetector::Process(const Mat &FrMask){
    //检测当前团块DetectFrBlob
    DetectFrBlob(FrMask);

    if(isnFrmNumZero){
        m_BlobListOld=m_BlobListNow;
        isnFrmNumZero=false;
    }

    //比较pNowBlobList和pOldBlobList，将相同的团块ID统一，pNewBlobList指向新团块
    DetectANamedNewBlob();
    //***************************团块处理开始********************************

    if(m_BlobListOld.size()!=0){
        /*if(m_NextBlobID==0 || m_NextBlobID>ProcblobList.size()){
            ProcblobList.resize(m_NextBlobID);
        }*/
        for(int i=m_BlobListOld.size();i>0;i--){
            HistoryBlobList.push_back(m_BlobListOld.at(i-1));
            //if(m_isDetectSuccess)
                //ProcblobList.at(m_BlobListOld.at(i-1).ID).push_back(m_BlobListOld.at(i-1));
            //上面的语句会有问题，比如说新检测到第99辆车，但是现在vector里面只有98辆，
            //没有99这个位置，at访问会失败。
        }
    }

    m_BlobListOld.clear();
    m_BlobListOld=m_BlobListNow;
    //*************************团块处理结束*********************************
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//以下是开发人员可以调用的函数，可以根据自己的流程选择适合自己的
//	函数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------【BlobDetector::Draw函数】----------------------------------  
//  描述：进行绘图处理，绘制出当前图像上所有的团块位置，运动轨迹以及
//ID号
//-----------------------------------------------------------------------------------------------  
void BlobDetector::Draw(cv::Mat &pImg){
    //标ID
    pI = pImg.clone();//彩色原图
    char        str1[1024];

    for(int i=m_BlobListNow.size();i>0;i--)
    {
        CarInfo *pB=&m_BlobListNow.at(i-1);
        sprintf(str1,"%03d",pB->getID());
        Point p(pB->getCentroid());
        //if( areaImg.at<cv::Vec3b>(p.y,p.x)[0]==255) //区域判断
        //输出文本内容
        //分别是图像矩阵，文字字符串，起始点（左下），字体，缩放比例，颜色，粗细。
        cv::putText(pI, str1,p, FONT_HERSHEY_SIMPLEX,1.0,Scalar(0,255,255),1);
        //p.y-=8;//换个位置
        //cvPutText( pI, str2,p, &font, CV_RGB(255,0,0));//输出信息2
    }

    //画轨迹线
    for(int i=m_BlobListNow.size();i>0;i--)
    {
        CarInfo *pB=&m_BlobListNow.at(i-1);
        TempGetBlobByID(pB->getID());
        //SameblobList=GetBlobByID(pB->ID);
        for(int j=SameblobList.size();j>1;j--){
            CarInfo *pB1=&SameblobList.at(j-1);
            CarInfo *pB2=&SameblobList.at(j-2);
            //cvLine(pI,cvPoint(pB1->x+pB1->w/2,pB1->y),cvPoint(pB2->x+pB2->w/2,pB2->y),CV_RGB(255,0,0),1,8,0);
            pB1->Line(pB2,pI);
        }
    }

    //画框
    for(int i= m_BlobListNow.size();i>0;i--)
    {
        CarInfo *pB=&m_BlobListNow.at(i-1);
        //cvRectangle(pI,cvPoint(pB->x,pB->y),cvPoint(pB->x+pB->w,pB->y+pB->h),cvScalar(255),1,8, 0);
        pB->Rectangle(pI);
    }

    //判断是否在区域内
    /*for(int i= m_BlobListNow.GetBlobNum();i>0;i--)
    {
    CvBlob *pB=m_BlobListNow.GetBlob(i-1);
    if( ((uchar *)(areaImg->imageData + (int)pB->y*areaImg->widthStep))[(int)pB->x*areaImg->nChannels + 0]==255)
    cvRectangle(pI,cvPoint(pB->x,pB->y),cvPoint(pB->x+pB->w,pB->y+pB->h),CV_RGB(255,0,0),1,8, 0);
    }*/

}


//--------------------------【TempGetBlobByID函数】-------------------------------------
//  描述：将所有同一ID号的团块装进一个Vector容器以供使用
//-----------------------------------------------------------------------------------------------
void BlobDetector::TempGetBlobByID(int BlobID)
{
    //临时使用一个vector来记录运行信息，当找到符合ID号的团块便加入到列表中
    SameblobList.clear();
    for(vector<int>::size_type i=0; i<HistoryBlobList.size(); i++)
        if(BlobID == HistoryBlobList.at(i).getID()){
            CarInfo *pB=&HistoryBlobList.at(i);
            SameblobList.push_back(*pB);
        }
}

//-------------------------【BlobDetector::Sort函数】----------------------------------  
//  描述：进行数据处理，确保所有的数据被整理成ProcblobList然后进行处理
//-----------------------------------------------------------------------------------------------
void BlobDetector::Sort(){
    //ProcblobList.resize(m_NextBlobID);
    for(vector<int>::size_type i=0; i<HistoryBlobList.size(); i++)	{
        TempGetBlobByID(i);
        if(0<SameblobList.size())
            ProcblobList.push_back(SameblobList);
    }
}

//---------------------【BlobDetector::GetBlobByID函数】----------------------------  
//  描述：可以调用这个函数得到某一个ID的所有信息，使用之前至少要运行
//一次sort函数来整理
//-----------------------------------------------------------------------------------------------
vector<CarInfo> BlobDetector::GetBlobByID(int BlobID)
{ 
    //使用vector<vector<CarInfo>>来记录运行信息，找到符合ID号的团块便返回该车辆的各个检测到的位置
    if(0<=BlobID && (unsigned)BlobID<=ProcblobList.size()){
        return ProcblobList.at(BlobID);
    }
    else{
        vector<CarInfo>  null(0);
        return null;
    }
}

