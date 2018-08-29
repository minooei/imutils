//
// Created by mohamad on 5/8/17.
//

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace imutils {


    struct SortContoursMethods {
        enum {
            left_to_right = 0,
            right_to_left = 1,
            bottom_to_top = 2,
            top_to_bottom = 3
        };
    };


    std::vector<cv::Point2f> order_points(std::vector<cv::Point2f> points);

    cv::Mat four_point_transform(cv::Mat image, std::vector<cv::Point2f> pts);

    cv::Mat translate(cv::Mat image, float x, float y);

    cv::Mat skeletonize(cv::Mat image, cv::Size size, int structuring = cv::MORPH_RECT);

    cv::Mat rotate_bound(cv::Mat image, double angle);

    cv::Mat rotate(cv::Mat image, double angle, cv::Point center = cv::Point(0, 0), double scale = 1.0);

    cv::Mat resize(cv::Mat image, int width = 0, int height = 0, int inter = cv::INTER_AREA);

    cv::Mat auto_canny(cv::Mat image, double sigma = 0.33);

    std::vector<std::vector<cv::Point>>
    sort_contours(std::vector<std::vector<cv::Point> > contours, std::vector<cv::Rect> &boundRect,
                  int method = SortContoursMethods::left_to_right);

    cv::Mat
    label_contour(cv::Mat image, std::vector<std::vector<cv::Point> > c, int i, cv::Scalar color = (0, 255, 0),
                  int thickness = 2);
}