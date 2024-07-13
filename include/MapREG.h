#ifndef MAPREG_H
#define MAPREG_H


#include "KeyFrame.h"
#include "Tracking.h"
#include "Map.h"


namespace ORB_SLAM2
{   
    class MapREG
    {
    
    public:
        MapREG(Map* pMap);
        void Run();
        void InsertKeyFrame(KeyFrame *pKF);
        
        
        void RequestFinish();
        bool CheckFinish();
        

        int KeyframesInQueue()
        {
        unique_lock<std::mutex> lock(mMutexNewKFs);
        return mlNewKeyFrames.size();
        }
    
    protected:



        bool CheckNewKeyFrames();
        void ProcessNewKeyFrame();
        void print_pointcloud();
        Map* mpMap;
        bool mbFinishRequested;

        
        
        KeyFrame* mpCurrentKeyFrame;
        std::mutex mMutexNewKFs;
        std::mutex mMutexFinish;
        std::list<KeyFrame*> mlNewKeyFrames;
        
    
    };
}
#endif