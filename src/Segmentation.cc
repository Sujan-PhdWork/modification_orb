# include "Segmentation.h"
#include <iostream>

namespace py = pybind11;


Segmentation::Segmentation()
{   
    static py::scoped_interpreter guard{};
    py::object sys=py::module::import("sys");
    sys.attr("path").attr("append")("python/");
    auto segment_module = py::module::import("segmentation");
    segment=segment_module.attr("segment");
}




cv::Mat Segmentation::result(cv::Mat Img)
{
    cv::Mat contiguous_Img = Img.isContinuous() ? Img : Img.clone();
    // Create a numpy array from the OpenCV matrix
    py::array_t<uint8_t> Image_array(
        {Img.rows, Img.cols, Img.channels()}, // shape of the array
        contiguous_Img.data);                // pointer to the data

    

    py::array_t<uint8_t> segmented_array=segment(Image_array);
    // Create a OpenCV Matrix from the numpy array
    py::buffer_info buf = segmented_array.request();
    cv::Mat segmented_image(buf.shape[0], buf.shape[1], CV_8UC1, (unsigned char*)buf.ptr);
    // std::cout<<segmented_image<<std::endl;
    return segmented_image;
}

// void Segmentation::exit()
// {   
//    static py::scoped_interpreter guard{}; 
//    py::exec("exit()");
// }





