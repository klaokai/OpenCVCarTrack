#ifndef BLOBTRACKER_H
#define BLOBTRACKER_H

#include <vector>
#include "CarInfo.h"
#include<QString>

class BlobTracker
{
    int m_TotalOfCars;
    int m_Time;//视频时间
    std::vector<std::vector <CarInfo> > ProcblobList;//运行完成后把全部车辆信息以ID编号保存轨迹
    std::vector<float> m_SpeedOfVehicles;
    std::vector<ushort> m_idxOfSpeedingCars;
public:
    void SetProbList(std::vector<std::vector <CarInfo> >  Procblob);
    void SetTimeParameter(int time);
    void ShowProbList();
    QString GetResult();
};

#endif // BLOBTRACKER_H
