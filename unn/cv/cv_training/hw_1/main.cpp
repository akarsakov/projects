#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
    double k = 0.2;
    Mat image = imread("lena.png");
    
    // convert to gray
    Mat gray_image;
    cvtColor(image, gray_image, 6);

    namedWindow( "Grayscale image", WINDOW_AUTOSIZE );
    imshow( "Grayscale image", gray_image );
    waitKey(0);

    // canny
    Mat edges_image;
    Canny(gray_image, edges_image, 150, 175, 3);
    namedWindow( "After Canny filter", WINDOW_AUTOSIZE );
    imshow( "After Canny filter", edges_image );
    waitKey(0);

    // invert binary image
    threshold(edges_image, edges_image, 125, 255, THRESH_BINARY_INV);

    // calc distance transform
    Mat distance_image;
    distanceTransform(edges_image, distance_image, CV_DIST_L2, 5);
    distance_image.convertTo(distance_image, CV_32S, 1, 0.5);

    // calc integral image
    Mat integral_image;
    integral(gray_image, integral_image);

    Mat result(image.size(), CV_8U);
    for (int i=0; i<image.rows; i++)
    {
        for (int j=0; j<image.cols; j++)
        {
            int aperture_size = k*distance_image.at<int>(i,j);
            if (aperture_size == 0)
            {
                result.at<uchar>(i,j) = gray_image.at<uchar>(i,j);
            } else
            {
                int x_left = max(j - (aperture_size + 1)/2, 0);
                int x_right = min(j + (aperture_size + 1)/2, image.cols - 1);
                int y_up = max(i - (aperture_size + 1)/2, 0);
                int y_down = min(i + (aperture_size + 1)/2, image.rows - 1);
                int s = integral_image.at<int>(y_up, x_left) + integral_image.at<int>(y_down, x_right) - integral_image.at<int>(y_down, x_left) - integral_image.at<int>(y_up, x_right);
                int new_val = s / ((x_right - x_left)*(y_down - y_up));
                new_val &= 255;
                result.at<uchar>(i,j) = new_val;
            }
        }
    }
    namedWindow( "After mean filetring", WINDOW_AUTOSIZE );
    imshow( "After mean filetring", result );
    waitKey(0);
    return 0;
};