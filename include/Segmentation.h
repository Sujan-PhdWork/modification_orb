
#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <Python.h>
#include <pybind11/embed.h>
#include <pybind11/numpy.h>
#include<opencv2/core/core.hpp>



namespace py = pybind11;

class Segmentation
{
public:
    py::object segment;

    
    Segmentation();


    cv::Mat result(cv::Mat Img);

    void exit();
};

#endif // SEGMENTATION_H