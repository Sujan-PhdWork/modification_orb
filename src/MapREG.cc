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

MapREG::MapREG(): output(new PointCloud ()),viewer(new pcl::visualization::PCLVisualizer("3D Viewer"))
{   
    viewer->setBackgroundColor(0, 0, 0);
    viewer->addCoordinateSystem (1.0);
    viewer->setCameraPosition(
        0, 0, -1, // Camera position (x, y, z)
        0, 0, 0,  // Focal point (x, y, z)
        0, -1, 0  // Up vector (x, y, z)
    );
    viewer->initCameraParameters ();
    output->height = 1;
}


void MapREG::PCD_VIEW()
{
    
    while (!viewer->wasStopped()) {
        {
            viewer->spinOnce();
        }
    }
}



PointCloud::Ptr MapREG::image2PointCloud(KeyFrame* kF)
{
    PointCloud::Ptr cloud(new PointCloud());
    cloud->height = 1;

    const double fx = kF->fx;
    const double fy = kF->fy;
    const double cx = kF->cx;
    const double cy = kF->cy;
    int sample =1;

    cv::Mat depth,rgb, seg;
    rgb=kF->mImRGB;
    depth=kF->mDepthImg;
    seg= kF->mSegGray;


    for (int m = 0; m < depth.rows; m+=sample) {
        for (int n = 0; n < depth.cols; n+=sample) {

        ushort d = depth.ptr<ushort>(m)[n];
        int sI = (int)seg.at<uchar>(n,m);
        // double sI = seg.ptr<>(m)[n]; 
        // cout<<value<<endl;           
        if (sI == 0)
            continue;
        if (d ==0)
            continue;
        PointT p;
        p.z =double(d)/5000.0;
        p.x = (n - cx) * p.z / fx;
        p.y = (m - cy) * p.z / fy;
        
        p.b=rgb.ptr<uchar>(m)[n*3];
        p.g=rgb.ptr<uchar>(m)[n*3+1];
        p.r=rgb.ptr<uchar>(m)[n*3+2];
        cloud->points.push_back(p);
        }
    
    }
    

    // // Generate some random points
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



int MapREG::countZeroElements(KeyFrame* kF) {
    cv::Mat mat;
    
    mat= kF->mSegGray;
    int zeroCount = 0;

    // Iterate through each element in the matrix
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            // Check if the element is zero
            if (mat.at<float>(i, j) == 0.0f) {
                ++zeroCount;
            }
        }
    }

    return zeroCount;
}

void MapREG::REGISTER_PointCloud(KeyFrame* kF)
{   
    // if (kF->mnId==0)
    int zeroCount = countZeroElements(kF);
    // cout<<zeroCount<<endl;
    // if (zeroCount<80000)
    //     return;
    PointCloud::Ptr cloud=image2PointCloud(kF);
    cv::Mat T= kF->GetPose();
    // cout<<T<<endl;    
    PointCloud::Ptr Tcloud(new PointCloud);
    Tcloud->height = 1;
    Eigen::Matrix4f transform= cvMat4x4ToEigen(T);
    Eigen::Matrix4f t_inv= transform.inverse();
    pcl::transformPointCloud(*cloud,*Tcloud,t_inv.matrix());
    string id_string = to_string(kF->mnId);
    viewer->addPointCloud<PointT>(Tcloud, id_string);
    // *output+=*Tcloud;
    
    
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

}