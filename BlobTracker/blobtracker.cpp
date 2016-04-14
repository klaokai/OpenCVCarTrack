#include "blobtracker.h"
#include "common.h"
void BlobTracker::SetProbList(vector<vector<CarInfo> > Procblob)
{
    if(Procblob.size()!=0){
        ProcblobList=Procblob;
        m_TotalOfCars=Procblob.size ();
    }
}

void BlobTracker::SetTimeParameter(int time)
{
    m_Time=time;
    qDebug()<<"m_Time"<<m_Time;
}

void BlobTracker::ShowProbList()
{
    int sizeOfCars=ProcblobList.size ();
    qDebug()<<"sizeOfCars"<<sizeOfCars;
    for (int i = 0; i < sizeOfCars; ++i) {
        int sizeOfRecord=(ProcblobList.at (i)).size();
        qDebug()<<"Car"<<i+1<<"  Record:"<<sizeOfRecord;
        //对每辆车的速度进行统计
        int length=0;
        for (int j = 0; j < sizeOfRecord-1; ++j) {
            //qDebug()<<"j:"<<ProcblobList.at (i).at(j).getID();
            length+=ProcblobList.at (i).at(j).BlobDistance(ProcblobList.at (i).at(j+1));
        }
        float speedOfVehicle=length / m_Time * 3.6 ;//m/s
        qDebug()<<"speed:"<<speedOfVehicle;
        if(speedOfVehicle>20)
            m_idxOfSpeedingCars.push_back(i);
        m_SpeedOfVehicles.push_back(speedOfVehicle);
    }
}

QString BlobTracker::GetResult()
{
    QString tmp;
    tmp.append("\n本视频中共有车辆:"+QString::number(m_TotalOfCars)+"\n超速车辆有:");
    for (size_t i = 0; i < m_idxOfSpeedingCars.size(); ++i) {
       tmp.append(QString::number(m_idxOfSpeedingCars.at(i))+',');
    }
    return tmp;
}
