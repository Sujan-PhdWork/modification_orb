#include "MapREG.h"

#include<mutex>

namespace ORB_SLAM2
{

class KeyFrame;
class Map;

MapREG::MapREG(Map *pMap): mpMap(pMap)
{   
    cout<<"enter"<<endl;
}

void MapREG::Run()
{
    while(1)
    {
        cout<<"Thread Running"<<endl;

        usleep(3000);
    }
}

void MapREG::InsertKeyFrame(KeyFrame *pKF)
{
    unique_lock<mutex> lock(mMutexNewKFs);
    mlNewKeyFrames.push_back(pKF);
}


bool MapREG::CheckNewKeyFrames()
{
    unique_lock<mutex> lock(mMutexNewKFs);
    return(!mlNewKeyFrames.empty());
}

void MapREG::ProcessNewKeyFrame()
{
    {
        unique_lock<mutex> lock(mMutexNewKFs);
        mpCurrentKeyFrame = mlNewKeyFrames.front();
        mlNewKeyFrames.pop_front();
    }

}

}