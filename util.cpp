//
// Created by mohamad on 5/8/17.
//
#include "util.h"

using namespace cv;
using namespace std;

namespace util {

    bool x_sort(const Point2f &a, const Point2f &b) {
        return a.x < b.x;
    }

    bool y_sort(const Point2f &a, const Point2f &b) {
        return a.y < b.y;
    }

    double dist(Point p, Point q) {
        return std::sqrt((p.x - q.x) * (p.x - q.x) +
                         (p.y - q.y) * (p.y - q.y));
    }

    Mat resize(Mat image, int width, int height, int inter) {
        // initialize the dimensions of the image to be resized and
        // grab the image size
        Size dim, hw;
        hw = image.size();

        // if both the width and height are None, then return the
        // original image
        if (width == 0 && height == 0)
            return image;

        // check to see if the width is None
        if (width == 0) {
            // calculate the ratio of the height and construct the
            // dimensions
            float r = float(height) / float(hw.height);
            dim = Size((hw.width * r), height);
        } else {
            // calculate the ratio of the width and construct the
            // dimensions
            float r = float(width) / float(hw.width);
            dim = Size(width, (hw.height * r));
        }
        Mat resized;
        resize(image, resized, dim, 0, 0, inter);

        return resized;
    }

    vector<Point2f> order_points(vector<Point2f> points) {

        //sort the points based on their x-coordinates
        std::sort(points.begin(), points.end(), x_sort);
        //grab the left-most and right-most points from the sorted x-roodinate points
        std::vector<Point2f> leftMost(points.begin(), points.begin() + points.size() / 2),
                rightMost(points.begin() + points.size() / 2, points.end());
        //now, sort the left-most coordinates according to their
        //y-coordinates so we can grab the top-left and bottom-left
        //points, respectively
        std::sort(leftMost.begin(), leftMost.end(), y_sort);
        Point2f tl = leftMost[0];
        Point2f bl = leftMost[1];
        //now, sort the right-most coordinates according to their
        //y-coordinates so we can grab the top-right and bottom-right
        //points, respectively
        std::sort(rightMost.begin(), rightMost.end(), y_sort);
        Point2f tr = rightMost[0];
        Point2f br = rightMost[1];
        //return the coordinates in top-left, top-right,bottom-right, and bottom-left order
        vector<Point2f> re_order;
        re_order.reserve(points.size());
        re_order.push_back(tl);
        re_order.push_back(tr);
        re_order.push_back(br);
        re_order.push_back(bl);
        return re_order;
    }

    Mat four_point_transform(Mat image, vector<Point2f> pts) {
        //obtain a consistent order of the points
        vector<Point2f> sorted_pts = order_points(pts);

        //compute the width of the new image, which will be the
        //maximum distance between bottom-right and bottom-left
        //x-coordiates or the top-right and top-left x-coordinates
        double widthA = dist(sorted_pts[0], sorted_pts[1]);
        double widthB = dist(sorted_pts[2], sorted_pts[3]);
        double maxWidth = max(int(widthA), int(widthB));

        //compute the height of the new image, which will be the
        //maximum distance between the top-right and bottom-right
        //y-coordinates or the top-left and bottom-left y-coordinates
        double heightA = dist(sorted_pts[0], sorted_pts[3]);
        double heightB = dist(sorted_pts[2], sorted_pts[1]);
        double maxHeight = max(int(heightA), int(heightB));

        //now that we have the dimensions of the new image, construct
        //the set of destination points to obtain a "birds eye view",
        //(i.e. top-down view) of the image, again specifying points
        //in the top-left, top-right, bottom-right, and bottom-left
        //order
        vector<Point2f> dst;
        dst.push_back(Point2f(0, 0));
        dst.push_back(Point2f(maxWidth - 1, 0));
        dst.push_back(Point2f(maxWidth - 1, maxHeight - 1));
        dst.push_back(Point2f(0, maxHeight - 1));

        //compute the util transform matrix and then apply it
        Mat M = cv::getPerspectiveTransform(sorted_pts, dst);
        Mat warped;
        cv::warpPerspective(image, warped, M, Size(maxWidth, maxHeight));
        //return the warped image
        return warped;
    }
}