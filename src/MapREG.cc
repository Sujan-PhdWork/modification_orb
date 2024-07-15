#include "MapREG.h"
#include<opencv2/core/core.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include<mutex>


using namespace cv;

namespace ORB_SLAM2
{

class KeyFrame;
class Map;

MapREG::MapREG(Map *pMap): mpMap(pMap),mbFinishRequested(false),viewer("Viewer"),output(new PointCloud ()),counter(0)
{   
    output->height = 1;

    // output = new PointCloud();
    // vis = new pcl::visualization::PCLVisualizer("cloud viewer");
    // vis->setBackgroundColor (0, 0, 0);
    // viewer.setBackgroundColor(0, 0, 0);

}

void MapREG::Run()
{
    while(!viewer.wasStopped())
    {   
        
        if(CheckNewKeyFrames())
        {
            ProcessNewKeyFrame();
            
            REGISTER_PointCloud();
            counter++;
            
            if (CheckFinish())
                // pcl::io::savePCDFile ("output.pcd", *output, true);
                break; 
        }

        usleep(3000);
    }
    // pcl::io::savePCDFile ("output.pcd", *output, true);
}




PointCloud::Ptr MapREG::image2PointCloud()
{
    PointCloud::Ptr cloud(new PointCloud());
    cloud->height = 1;

    const double fx = mpCurrentKeyFrame->fx;
    const double fy = mpCurrentKeyFrame->fy;
    const double cx = mpCurrentKeyFrame->cx;
    const double cy = mpCurrentKeyFrame->cy;
    int sample =5;

    for (int m = 0; m < mpCurrentKeyFrame->mDepthImg.rows; m+=sample) {
        for (int n = 0; n < mpCurrentKeyFrame->mDepthImg.cols; n+=sample) {

        double d = mpCurrentKeyFrame->mDepthImg.ptr<double>(m)[n];

        if (d == 0)
            continue;
        PointT p;
        p.z = double(d);
        p.x = (n - cx) * p.z / fx;
        p.y = (m - cy) * p.z / fy;
        cloud->points.push_back(p);
        }
    
  }

    // Generate some random points
    // cloud->width = 1000;
    // cloud->height = 1;
    // cloud->points.resize(cloud->width * cloud->height);
    // for (size_t i = 0; i < cloud->points.size(); ++i) {
    //     cloud->points[i].x = 1024 * rand() / (RAND_MAX + 1.0f);
    //     cloud->points[i].y = 1024 * rand() / (RAND_MAX + 1.0f);
    //     cloud->points[i].z = 1024 * rand() / (RAND_MAX + 1.0f);
    // }

  return cloud;
}



void MapREG::REGISTER_PointCloud()
{   
    PointCloud::Ptr cloud=image2PointCloud();
    cv::Mat T = mpCurrentKeyFrame->GetPose();
    // cout<<T<<endl;

    // cv::Mat transformation_matrix = cv::Mat::eye(4, 4, CV_32F);
    Eigen::Matrix4f transform = cvMat4x4ToEigen(T.inv());

    // Eigen::Isometry3d T = Eigen::Isometry3d::Identity();
    PointCloud::Ptr Tcloud(new PointCloud);
    Tcloud->height = 1;
    pcl::transformPointCloud(*cloud,*Tcloud,transform);
    // pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> source_cloud_color_handler (cloud, 100, 255, 255);
    // viewer.addPointCloud (cloud,source_cloud_color_handler, "original_cloud");
    *output+=*Tcloud;
    viewer.showCloud(output);
    cout<<output->width<<endl;
    
    // viewer.showCloud(output);
    // cv::Mat mGray=mpCurrentKeyFrame->mGray;
    // cv::imshow("new",mGray);
    // cv::waitKey(1);
    // std::this_thread::sleep_for(100ms);
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

Eigen::Matrix4f MapREG::cvMat4x4ToEigen(const cv::Mat& cv_mat) {
    Eigen::Matrix4f eigen_mat;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            eigen_mat(i, j) = cv_mat.at<float>(i, j);
        }
    }
    return eigen_mat;
}

void MapREG::SavePCD()
{
    pcl::io::savePCDFile ("output.pcd", *output, true);
}

}