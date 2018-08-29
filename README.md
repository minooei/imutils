# imutils
A series of convenience functions to make basic image processing operations such as translation, rotation, resizing and skeletonization with OpenCV and C++.

## Translation
Translation is the shifting of an image in either the *x* or *y* direction. To translate an image in OpenCV you would need to supply the *(x, y)*-shift, denoted as *(t<sub>x</sub>, t<sub>y</sub>)* to construct the translation matrix *M*

And from there, you would need to apply the `warpAffine` function.

Instead of manually constructing the translation matrix *M* and calling `warpAffine`, you can simply make a call to the `translate` function of `imutils`.

#### Example:
<pre>// translate the image x=25 pixels to the right and y=75 pixels up
Mat translated = imutils::translate(workspace, 25, -75);</pre>

## Rotation
Rotating an image in OpenCV is accomplished by making a call to `getRotationMatrix2D` and `warpAffine`. Further care has to be taken to supply the *(x, y)*-coordinate of the point the image is to be rotated about. These calculation calls can quickly add up and make your code bulky and less readable. The `rotate` function in `imutils` helps resolve this problem.

#### Example:
<pre>  // loop over the angles to rotate the image
for (int angle :  {50, 360, 90}) {
        //# rotate the image and display it
        Mat bridge = imread("shapes.png");
        if (bridge.empty()) {
            cout << "err" << endl;
            return;
        }
        Mat rotated = imutils::rotate(bridge, angle);
        imshow("Angle="+angle, rotated);</pre>

## Resizing
Resizing an image in OpenCV is accomplished by calling the `resize` function. However, special care needs to be taken to ensure that the aspect ratio is maintained.  This `resize` function of `imutils` maintains the aspect ratio and provides the keyword arguments `width` and `height` so the image can be resized to the intended width/height while (1) maintaining aspect ratio and (2) ensuring the dimensions of the image do not have to be explicitly computed by the developer.

Another optional keyword argument, `inter`, can be used to specify interpolation method as well.

#### Example:
<pre>  // loop over varying widths to resize the image to
for (int width : {400, 300, 200, 100}) {
    //    # resize the image and display it
    Mat image = imread("pyimagesearch_logo.jpg");

    Mat resized = imutils::resize(image, width);
    imshow("Width= " + (width), resized);
}</pre>


## Skeletonization
Skeletonization is the process of constructing the "topological skeleton" of an object in an image, where the object is presumed to be white on a black background. OpenCV does not provide a function to explicitly construct the skeleton, but does provide the morphological and binary functions to do so.

For convenience, the `skeletonize` function of `imutils` can be used to construct the topological skeleton of the image.

The first argument, `size` is the size of the structuring element kernel. An optional argument, `structuring`, can be used to control the structuring element -- it defaults to `MORPH_RECT`	, but can be any valid structuring element.

#### Example:
<pre>   // skeletonize the image
    Mat gray = imread("pyimagesearch_logo.jpg", 0);
    Mat skeleton = imutils::skeletonize(gray, cv::Size(3, 3));
    imshow("Skeleton", skeleton);</pre>


## Automatic Canny Edge Detection
The Canny edge detector requires two parameters when performing hysteresis. However, tuning these two parameters to obtain an optimal edge map is non-trivial, especially when working with a dataset of images. Instead, we can use the `auto_canny` function which uses the median of the grayscale pixel intensities to derive the upper and lower thresholds. You can read more about the `auto_canny` function [here](http://www.pyimagesearch.com/2015/04/06/zero-parameter-automatic-canny-edge-detection-with-python-and-opencv/).

#### Example:
<pre>    Mat gray = imread("pyimagesearch_logo.jpg", 0);
    Mat edgeMap = imutils::auto_canny(gray);
    imshow("Automatic Edge Map", edgeMap);</pre>

## 4-point Perspective Transform
A common task in computer vision and image processing is to perform a 4-point perspective transform of a ROI in an image and obtain a top-down, "birds eye view" of the ROI. The `perspective` module takes care of this for you. A real-world example of applying a 4-point perspective transform can be bound in this blog on on [building a kick-ass mobile document scanner](http://www.pyimagesearch.com/2014/09/01/build-kick-ass-mobile-document-scanner-just-5-minutes/).

#### Example
<pre>    Mat notecard = imread("notecard.png");
    vector<Point2f> pts;
    pts.push_back(Point2f(73, 239));
    pts.push_back(Point2f(356, 117));
    pts.push_back(Point2f(475, 265));
    pts.push_back(Point2f(187, 443));
    Mat res = imutils::four_point_transform(notecard, pts);
    imshow("four_point_transform", res);</pre>

## Sorting Contours
The contours returned from `findContours` are unsorted. By using the `contours` module the the `sort_contours` function we can sort a list of contours from left-to-right, right-to-left, top-to-bottom, and bottom-to-top, respectively.

#### Example:
<pre>    Mat image = imread("shapes.png");
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
    imshow("left_to_right", sortedImage);</pre>



