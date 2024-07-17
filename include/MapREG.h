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

typedef pcl::PointXYZRGBA PointT;
typedef pcl::PointCloud<PointT> PointCloud;


using namespace std;
namespace ORB_SLAM2
{   
    class KeyFrame;

    class MapREG
    {
    
    public:
        MapREG();
        PointCloud::Ptr output;        
        PointCloud::Ptr image2PointCloud(KeyFrame* kF);
        void REGISTER_PointCloud(KeyFrame *kF);
        void PCD_VIEW();
        Eigen::Matrix4f cvMat4x4ToEigen(const cv::Mat& cv_mat);
        int countZeroElements(KeyFrame* kF);
        Map* mpMap;
        KeyFrame* mpCurrentKeyFrame;
        pcl::visualization::PCLVisualizer::Ptr viewer;
        
        
    
    };
}
#endif