//
// Created by mohamad on 5/8/17.
//
#include "imutils.h"

using namespace cv;
using namespace std;

namespace imutils {

    bool x_sort(const Point2f &a, const Point2f &b) {
        return a.x < b.x;
    }

    bool y_sort(const Point2f &a, const Point2f &b) {
        return a.y < b.y;
    }

    double dist(Point p, Point q) {
        return sqrt((p.x - q.x) * (p.x - q.x) +
                    (p.y - q.y) * (p.y - q.y));
    }


    Mat translate(Mat image, float x, float y) {
//# define the translation matrix and perform the translation
        double m_scale[2][3] = {{1, 0, x},
                                {0, 1, y}};

        Mat M = Mat(2, 3, CV_64F, m_scale);
        Mat shifted;
        warpAffine(image, shifted, M, cv::Size(image.cols, image.rows));

//# return the translated image
        return shifted;
    }

    Mat rotate(Mat image, double angle, cv::Point center, double scale) {
        //    # grab the dimensions of the image
        int h = image.rows, w = image.cols;

        //    # if the center is None, initialize it as the center of
        //    # the image
        if (center.x == 0 && center.y == 0)
            center = cv::Point(w / 2, h / 2);

        //    # perform the rotation
        Mat M = getRotationMatrix2D(center, angle, scale);
        Mat rotated;
        warpAffine(image, rotated, M, cv::Size(w, h));

        //    # return the rotated image
        return rotated;
    }

    Mat skeletonize(Mat image, cv::Size size, int structuring) {

        //# determine the area (i.e. total number of pixels in the image),
        //# initialize the output skeletonized image, and construct the
        //# morphological structuring element
        int h = image.rows, w = image.cols;

        int area = h * w;
        Mat skeleton = cv::Mat::zeros(image.size(), image.type());
        Mat elem = getStructuringElement(structuring, size);

        //# keep looping until the erosions remove all pixels from the
        //# image
        while (1) {
            //# erode and dilate the image using the structuring element
            Mat eroded;
            erode(image, eroded, elem);
            Mat temp;
            dilate(eroded, temp, elem);

            //# subtract the temporary image from the original, eroded
            //# image, then take the bitwise 'or' between the skeleton
            //# and the temporary image
            subtract(image, temp, temp);
            bitwise_or(skeleton, temp, skeleton);
            image = eroded.clone();

            //# if there are no more 'white' pixels in the image, then
            //# break from the loop
            if (area == area - countNonZero(image))
                break;
        }
        //# return the skeletonized image
        return skeleton;
    }

    Mat rotate_bound(Mat image, double angle) {
        //# grab the dimensions of the image and then determine the# center
        int h = image.rows, w = image.cols;

        cv::Point center = cv::Point(w / 2, h / 2);


        //# grab the rotation matrix (applying the negative of the
        //# angle to rotate clockwise), then grab the sine and cosine
        //# (i.e., the     rotation components of the  matrix)
        Mat M = getRotationMatrix2D(center, -angle, 1.0);
        double cos = abs(M.at<double>(0, 0));
        double sin = abs(M.at<double>(0, 1));

        //# compute the new bounding dimensions of the image
        int nW = int((h * sin) + (w * cos));
        int nH = int((h * cos) + (w * sin));

        //# adjust the rotation matrix to take into account translation
        M.at<double>(0, 2) += (nW / 2) - center.x;
        M.at<double>(1, 2) += (nH / 2) - center.y;
        Mat rotated;

        //# perform the actual rotation and return the image
        warpAffine(image, rotated, M, cv::Size(nW, nH));
        return rotated;
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
        sort(points.begin(), points.end(), x_sort);
        //grab the left-most and right-most points from the sorted x-roodinate points
        vector<Point2f> leftMost(points.begin(), points.begin() + points.size() / 2),
                rightMost(points.begin() + points.size() / 2, points.end());
        //now, sort the left-most coordinates according to their
        //y-coordinates so we can grab the top-left and bottom-left
        //points, respectively
        sort(leftMost.begin(), leftMost.end(), y_sort);
        Point2f tl = leftMost[0];
        Point2f bl = leftMost[1];
        //now, sort the right-most coordinates according to their
        //y-coordinates so we can grab the top-right and bottom-right
        //points, respectively
        sort(rightMost.begin(), rightMost.end(), y_sort);
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

        //compute the imutil transform matrix and then apply it
        Mat M = getPerspectiveTransform(sorted_pts, dst);
        Mat warped;
        warpPerspective(image, warped, M, Size(maxWidth, maxHeight));
        //return the warped image
        return warped;
    }

    double medianMat(Mat Input) {
        Input = Input.reshape(0, 1); // spread Input Mat to single row
        vector<double> vecFromMat;
        Input.copyTo(vecFromMat); // Copy Input Mat to vector vecFromMat
        nth_element(vecFromMat.begin(), vecFromMat.begin() + vecFromMat.size() / 2, vecFromMat.end());
        return vecFromMat[vecFromMat.size() / 2];
    }

    Mat auto_canny(Mat image, double sigma) {
        // compute the median of the single channel pixel intensities
        double v = medianMat(image);

        // apply automatic Canny edge detection using the computed median
        int lower = int(max(0.0, (1.0 - sigma) * v));
        int upper = int(min(255.0, (1.0 + sigma) * v));
        Mat edged;
        Canny(image, edged, lower, upper);

        // return the edged image
        return edged;
    }

    vector<vector<Point>>
    sort_contours(vector<vector<Point>> contours, vector<Rect> &boundRect, int method) {
        // initialize the reverse flag and sort index
        bool reverse = false;
        int xy = 0;

        // handle if we need to sort in reverse
        if (method == 1 or method == 2)
            reverse = true;

        // handle if we are sorting against the y-coordinate rather than
        // the x-coordinate of the bounding box
        if (method == 3 or method == 2)
            xy = 1;

        // construct the list of bounding boxes and sort them from top to bottom
        // boundingBoxes = [cv2.boundingRect(c) for c in cnts]
        for (size_t i = 0; i < contours.size(); i++) {
            boundRect.push_back(boundingRect(contours[i]));
        }

        std::vector<int> indices(boundRect.size());
        std::size_t n(0);
        std::generate(std::begin(indices), std::end(indices), [&] { return n++; });

        std::sort(std::begin(indices),
                  std::end(indices),
                  [&](int i1, int i2) {
                      return xy ? boundRect[i1].y < boundRect[i2].y : boundRect[i1].x < boundRect[i2].x;
                  });
        if (reverse)
            std::reverse(indices.begin(), indices.end());
        vector<vector<Point>> contours_sorted;
        boundRect.clear();
        for (auto v : indices) {
            contours_sorted.push_back(contours[v]);
            boundRect.push_back(boundingRect(contours[v]));
        }
        return contours_sorted;
    }

    cv::Mat
    label_contour(cv::Mat image, std::vector<std::vector<cv::Point> > c, int i, cv::Scalar color, int thickness) {

        // compute the center of the contour area and draw a circle
        // representing the center
        Moments M = moments(c[0]);
        int cX = int(M.m10 / M.m00);
        int cY = int(M.m01 / M.m00);

        // draw the contour and label number on the image
        drawContours(image, c, -1, color, thickness);
        putText(image, to_string(i + 1), cv::Point(cX - 20, cY), FONT_HERSHEY_SIMPLEX,
                1.0, (255, 255, 255), 2);

        // return the image with the contour number drawn on it
        return image;
    }




}