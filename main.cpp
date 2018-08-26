#include <iostream>
#include "imutils.h"

void sorting_contours();

using namespace std;
using namespace cv;

int main() {
    std::cout << "Hello, World!" << std::endl;
    sorting_contours();

    return 0;
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
    Mat sortedImage=image.clone();
    for (int i = 0; i < contours.size(); i++) {
        sortedImage = imutils::label_contour(sortedImage, vector<vector<Point> >(1, contours[i]), i,
                                             cv::Scalar(240, 0, 159));
    }
    imshow("left_to_right", sortedImage);
    waitKey(0);
}