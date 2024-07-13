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
        bool CheckNewKeyFrames();
        void ProcessNewKeyFrame();
    
    protected:
        Map* mpMap;
        KeyFrame* mpCurrentKeyFrame;
        std::mutex mMutexNewKFs;
        std::list<KeyFrame*> mlNewKeyFrames;
        
    
    };
}
#endif