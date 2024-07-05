# include "Segmentation.h"


pybind11::object display() {
    static pybind11::scoped_interpreter guard{}; // Ensure the interpreter is only initialized once

    auto sys = pybind11::module::import("sys");

    sys.attr("path").attr("append")("python/");
    
    auto display_image = pybind11::module::import("display");
    auto show = display_image.attr("plot");

    return show;
}