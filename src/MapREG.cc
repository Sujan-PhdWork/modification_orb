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
        
        if(CheckNewKeyFrames())
        {
            ProcessNewKeyFrame();
            print_pointcloud();
            if (CheckFinish())
                break; 
        }

        usleep(3000);
    }
}



void MapREG::print_pointcloud()
{
    cv::Mat mGray=mpCurrentKeyFrame->mGray;
    cv::imshow("new",mGray);
    cv::waitKey(1);
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


void MapREG::RequestFinish()
{
    unique_lock<mutex> lock(mMutexFinish);
    mbFinishRequested = true;
}

bool MapREG::CheckFinish()
{
    unique_lock<mutex> lock(mMutexFinish);
    return mbFinishRequested;
}

}