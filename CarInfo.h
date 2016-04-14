#ifndef CARINFO_H
#define CARINFO_H

#include "opencv2/highgui/highgui.hpp"
//using namespace cv;

class CarInfo{
public:
    CarInfo(){ID=0;}
    CarInfo(float X,float Y,float width,float height,int uID=0)
        :x(X),y(Y),w(width),h(height),ID(uID)	{}


private:
    float x,y; /* 车辆位置 */
    float w,h; /*车辆大小  */
    unsigned int ID;  /* 车辆ID*/

public:

    //获得位置
    cv::Point getCentroid(){
        cv::Point a((int)x,(int)y);
        return a;
    }
    //获得面积
    float getArea(){
        return w*h;
    }
    //计算两个车辆的质心距离
    float BlobDistance(CarInfo& b){
        return sqrt(pow((x+w/2 - b.x-(b.w)/2),2) + pow((y+h/2 - b.y-(b.h)/2),2));
    }
    //用直线连接两个质心
    void Line(const CarInfo*  b,cv::Mat &dst){
        cv::line(dst,cv::Point((int)(x+w/2),(int)y),cv::Point((int)(b->x+b->w/2),(int)(b->y)),CV_RGB(255,0,0),1,8,0);
    }
    //绘制CarInfo矩形
    void Rectangle(cv::Mat &dst){
        cv::Rect temp(x,y,w,h);
        cv::rectangle(dst,temp,cv::Scalar::all(255));
    }

    int getID() const{
        return ID;
    }

    void setID(int value){
        if(value>0)
            ID = value;
    }

};
#endif // CARINFO_H
