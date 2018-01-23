//
// Created by mohamad on 5/8/17.
//

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace util {
    std::vector<cv::Point2f> order_points(std::vector<cv::Point2f> points);
    cv::Mat four_point_transform(cv::Mat image, std::vector<cv::Point2f> pts);
    cv::Mat resize(cv::Mat image, int width = 0, int height = 0, int inter = cv::INTER_AREA) ;
};
