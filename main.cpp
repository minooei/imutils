#include <iostream>
#include "imutils.h"

void sorting_contours();

void rotate_demo();

void translate_demo();

void rotate_bound_demo();

void skeletonize();

void resizing_demo();

void auto_canny_demo();

void four_point_transform_demo();

using namespace std;
using namespace cv;

int main() {
    std::cout << "Hello, World!" << std::endl;
//    sorting_contours();
//    rotate_demo();
//    rotate_demo();
//    skeletonize();
//    translate_demo();
//    resizing_demo();
//    auto_canny_demo();
//    four_point_transform_demo();
    return 0;
}

void four_point_transform_demo() {
    Mat notecard = imread("notecard.png");
    vector<Point2f> pts;
    pts.push_back(Point2f(73, 239));
    pts.push_back(Point2f(356, 117));
    pts.push_back(Point2f(475, 265));
    pts.push_back(Point2f(187, 443));
    Mat res = imutils::four_point_transform(notecard, pts);
    imshow("four_point_transform", res);
    waitKey(0);

}

void auto_canny_demo() {
    Mat gray = imread("pyimagesearch_logo.jpg", 0);
    Mat edgeMap = imutils::auto_canny(gray);
    imshow("Automatic Edge Map", edgeMap);
    waitKey(0);
}

void resizing_demo() {
    //# loop over varying widths to resize the image to
    for (int width : {400, 300, 200, 100}) {
        //    # resize the image and display it
        Mat image = imread("pyimagesearch_logo.jpg");

        Mat resized = imutils::resize(image, width);
        imshow("Width= " + (width), resized);
    }
    //# wait for a keypress, then close all the windows
    waitKey(0);

}

void skeletonize() {
    Mat gray = imread("pyimagesearch_logo.jpg", 0);
    Mat skeleton = imutils::skeletonize(gray, cv::Size(3, 3));
    imshow("Skeleton", skeleton);
    waitKey(0);
}

void rotate_bound_demo() {
    for (int angle :  {50, 360, 90}) {
        //# rotate the image and display it
        Mat bridge = imread("shapes.png");
        if (bridge.empty()) {
            cout << "err" << endl;
            return;
        }
        Mat rotated = imutils::rotate_bound(bridge, angle);
        imshow("Angle=%d", rotated);
        waitKey(0);
    }
}

void translate_demo() {
    Mat workspace = imread("shapes.png");
    Mat translated = imutils::translate(workspace, -50, 100);
    imshow("Translated", translated);
    waitKey(0);

}

void rotate_demo() {
    for (int angle :  {50, 360, 90}) {
        //# rotate the image and display it
        Mat bridge = imread("shapes.png");
        if (bridge.empty()) {
            cout << "err" << endl;
            return;
        }
        Mat rotated = imutils::rotate(bridge, angle);
        imshow("Angle="+angle, rotated);
        waitKey(0);
    }
}

void sorting_contours() {
    Mat image = imread("shapes.png");
    Mat orig = image.clone();
    Mat gray;
    cvtColor(image, gray, CV_BGR2GRAY);
    Mat edged = imutils::auto_canny(gray);
    vector<Vec4i> hierarchy;

    vector<vector<Point>> contours;
    cv::findContours(edged, contours, hierarchy, CV_RETR_EXTERNAL,
                     CV_CHAIN_APPROX_SIMPLE);
    vector<Rect> boundRect;
    contours = imutils::sort_contours(contours, boundRect, imutils::SortContoursMethods::left_to_right);
    Mat sortedImage = image.clone();
    for (int i = 0; i < contours.size(); i++) {
        sortedImage = imutils::label_contour(sortedImage, vector<vector<Point> >(1, contours[i]), i,
                                             cv::Scalar(240, 0, 159));
    }
    imshow("left_to_right", sortedImage);
    waitKey(0);
}