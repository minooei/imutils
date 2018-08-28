#include <iostream>
#include "imutils.h"

void sorting_contours();

void rotate_demo();

void translate_demo();

using namespace std;
using namespace cv;

int main() {
    std::cout << "Hello, World!" << std::endl;
//    sorting_contours();
//    rotate_demo();
    translate_demo();
    return 0;
}

void translate_demo() {
    Mat workspace = imread("shapes.png");
    Mat translated = imutils::translate(workspace, -50, 100);
    imshow("Translated", translated);
    waitKey(0);

}

void rotate_demo() {
    for (int angle :  {0, 360, 90}) {
        //# rotate the image and display it
        Mat bridge = imread("shapes.png");
        if (bridge.empty()) {
            cout << "err" << endl;
            return;
        }
        Mat rotated = imutils::rotate(bridge, angle);
        imshow("Angle=%d", rotated);
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