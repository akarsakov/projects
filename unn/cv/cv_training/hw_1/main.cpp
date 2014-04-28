#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat image = imread("lena.png");
    namedWindow( "Source image", WINDOW_AUTOSIZE );
    imshow( "Source image", image );
    waitKey(0);
    
    // convert to gray
    Mat gray_image;
    cvtColor(image, gray_image, 6);

    namedWindow( "Grayscale image", WINDOW_AUTOSIZE );
    imshow( "Grayscale image", gray_image );
    waitKey(0);

    // canny
    Mat edges_image;
    Canny(gray_image, edges_image, 100, 150, 3);
    namedWindow( "After Canny filter", WINDOW_AUTOSIZE );
    imshow( "After Canny filter", edges_image );
    waitKey(0);

    // calc distance transform
    Mat distance_image;
    distanceTransform(edges_image, distance_image, 2, 5);

    // calc integral image
    Mat integral_image;
    integral(image, integral_image);

    Mat result(image.size(), CV_8U);
    for (int i=0; i<image.rows; i++)
    {
        for (int j=0; j<image.cols; j++)
        {
            int aperture_size = distance_image.at<int>(i,j);
            int new_val = 0;
            for (int y=0; y<aperture_size; y++)
            {
                int y_coord = borderInterpolate(y - (aperture_size + 1)/2, image.rows, BORDER_REFLECT_101);
                for (int x=0; x<aperture_size; x++)
                {
                    int x_coord = borderInterpolate(x - (aperture_size + 1)/2, image.cols, BORDER_REFLECT_101);
                }
            }
        }
    }

    

    return 0;
};