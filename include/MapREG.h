#ifndef MAPREG_H
#define MAPREG_H


#include <unistd.h>
#include<string>

#include <pcl/memory.h>  // for pcl::make_shared

#include <pcl/point_types.h>

#include <pcl/point_cloud.h>

#include <pcl/point_representation.h>


#include <pcl/io/pcd_io.h>

#include <pcl/common/transforms.h>


#include <pcl/visualization/cloud_viewer.h>
#include <pcl/visualization/pcl_visualizer.h>

#include "KeyFrame.h"
#include "Tracking.h"
#include "Map.h"

typedef pcl::PointXYZ PointT;
typedef pcl::PointCloud<PointT> PointCloud;

using namespace std;
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
        void SavePCD();


        int KeyframesInQueue()
        {
        unique_lock<std::mutex> lock(mMutexNewKFs);
        return mlNewKeyFrames.size();
        }
        pcl::visualization::CloudViewer viewer;
        PointCloud::Ptr output;

        int counter;


    
    protected:



        bool CheckNewKeyFrames();
        void ProcessNewKeyFrame();
        PointCloud::Ptr image2PointCloud();
        void REGISTER_PointCloud();
        Eigen::Matrix4f cvMat4x4ToEigen(const cv::Mat& cv_mat);
        Map* mpMap;
        bool mbFinishRequested;

        
        
        KeyFrame* mpCurrentKeyFrame;
        std::mutex mMutexNewKFs;
        std::mutex mMutexFinish;
        std::list<KeyFrame*> mlNewKeyFrames;
        
    
    };
}
#endif