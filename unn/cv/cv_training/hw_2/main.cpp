#include <iostream>
#include <vector>
#include <numeric>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/video/tracking.hpp"

using namespace std;
using namespace cv;

Mat markerMask, frame;
Point prevPt(-1, -1);
bool mark_image = false;
int object_num = 0;
Scalar colors[2] = { Scalar(255, 0, 0), Scalar(0, 255, 0) };

static void onMouse( int event, int x, int y, int flags, void* )
{
    if (!mark_image || object_num > 1)
        return;

    if( x < 0 || x >= frame.cols || y < 0 || y >= frame.rows )
        return;

    if( event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON) )
        prevPt = Point(-1,-1);
    else if( event == EVENT_LBUTTONDOWN )
        prevPt = Point(x,y);
    else if( event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON) )
    {
        Point pt(x, y);
        if( prevPt.x < 0 )
            prevPt = pt;
        markerMask.at<int>(pt) = object_num + 1;
        line( markerMask, prevPt, pt, Scalar(object_num + 1), 5, 8, 0 );
        line( frame, prevPt, pt, colors[object_num], 5, 8, 0 );

        prevPt = pt;
        imshow("video", frame);
    }
}

void run_watershed(Mat frame, Mat& mask, Mat& result)
{
    watershed(frame, mask);

    // paint the watershed image
    Mat wshed(frame.size(), CV_8UC3);
    for( int i = 1; i < mask.rows-1; i++ )
        for( int j = 1; j < mask.cols-1; j++ )
        {
            int index = mask.at<int>(i,j);
            if( index == -1 )
            {
                //result.at<Vec3b>(i,j) = Vec3b(255,0,0);
                circle(result, Point(j,i), 1, Scalar(255,0,0), 1);
            }
        }
    
}

void run_DFT(Mat frame, Mat obj_dft_image, Size dftSize, Size objSize, Mat& result)
{
    Mat framef;
    frame.convertTo(framef, CV_32F);

    Mat tempA(dftSize, framef.type(), Scalar::all(0));

    Mat roiA(tempA, Rect(0,0, framef.cols, framef.rows));
    framef.copyTo(roiA);

    dft(tempA, tempA, 0, framef.rows);
    mulSpectrums(tempA, obj_dft_image, tempA, DFT_ROWS, true);

    dft(tempA, tempA, DFT_INVERSE + DFT_SCALE, framef.rows);
    
    double min, max;
    Point minLoc, maxLoc;
    minMaxLoc(tempA, &min, &max, &minLoc, &maxLoc);

    rectangle(result, maxLoc, Point(maxLoc.x + objSize.width, maxLoc.y + objSize.height), Scalar(0, 0, 255), 3);
}

Mat generate_new_mask(vector<Point2f> points, vector<Point>& hull)
{
    vector<int> hull_num;
    convexHull(Mat(points), hull_num, false);
    for (int i=0; i<hull_num.size(); i++)
    {
        hull.push_back(points[hull_num[i]]);
    }

    // calc mean point for object
    Mat mask(frame.size(), CV_32S);
    Point2f sum = accumulate( hull.begin(), hull.end(), Point(0,0));
    Point mean_point(sum.x / hull.size(), sum.y / hull.size());
    //mask.at<int>(mean_point) = 1;
    circle(mask, mean_point, 10, Scalar(1), 10);

    for (auto it=hull.begin(); it!=hull.end(); it++)
    {
        mask.at<int>(*it) = 2;
    }
    return mask;
}

Mat get_binary_image(Mat img)
{
     Mat binary_frame;
     Canny(img, binary_frame, 100, 150);
     return binary_frame;
}

int main(int argc, char* argv[])
{
    VideoCapture cap;
    if (argc >= 2) 
    {
        string filename = string(argv[1]);
        cap.open(filename);
    } else
    {
        cap.open(0);
    }

    namedWindow( "video", WINDOW_AUTOSIZE);
    setMouseCallback( "video", onMouse, 0 );
    cap >> frame;

    markerMask.create(frame.size(), CV_32S);
    Mat gray, prevGray;
    // for DFT
    Size dftSize;
    dftSize.width = getOptimalDFTSize(frame.cols);
    dftSize.height = getOptimalDFTSize(frame.rows);
    Mat obj_dft_image(dftSize, CV_32F);
    Size obj_size;


    vector<Point2f> points[2];
    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
    Size subPixWinSize(10,10), winSize(31,31);

    char key = 0;
    bool ok = false;
    int frame_num = 0;
    while (true)
    {
        cap >> frame;
        if (frame.empty())
            break;

        resize(frame, frame, Size(1280, 720));

        Mat copy_frame;
        frame.copyTo(copy_frame);
        cvtColor(frame, gray, CV_BGR2GRAY);

        if( key == 27 )
            break;

        if (key == ' ' || frame_num == 1)
        {
            mark_image = true;
            points[0].clear();
            points[1].clear();
            markerMask = Scalar::all(0);
            obj_dft_image = Scalar::all(0);
            object_num = 0;
            
            // fill mask
            while (true) 
            {
                key = waitKey(0);
                if ( key = ' ')
                {
                    object_num++;
                }
                if (object_num > 1)
                    break;
            }

            mark_image = false;

            watershed( copy_frame, markerMask );

            Mat object_mask = markerMask == 1;
            
            // find good points on object mask
            goodFeaturesToTrack(gray, points[0], 10, 0.01, 10, object_mask, 3, 0, 0.04);
            cornerSubPix(gray, points[0], subPixWinSize, Size(-1,-1), termcrit);

            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;
            findContours(object_mask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

            // calc DFT for object image
            if (contours.size() > 0)
            {
                Mat binary_image = get_binary_image(gray);

                Rect obj_rect = boundingRect(contours[0]);
                obj_size = Size(obj_rect.width, obj_rect.height);

                Mat obj_image = binary_image(obj_rect);

                Mat temp(obj_dft_image.size(), CV_8U);
                temp = Scalar::all(0);
                Mat roi_dft(temp, Rect(0,0,obj_image.cols,obj_image.rows));
                obj_image.copyTo(roi_dft);

                temp.convertTo(obj_dft_image, CV_32F);

                dft(obj_dft_image, obj_dft_image, 0, obj_image.rows);

                rectangle(frame, obj_rect, Scalar(0, 0, 255));

                ok = true;
            }
        }

        if (ok)
        {
            // optical flow
            vector<uchar> status;
            vector<float> err;
            if(prevGray.empty())
                gray.copyTo(prevGray);
            calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize, 3, termcrit, 0, 0.001);
            
            // draw points
            size_t i, k;
            for( i = k = 0; i < points[1].size(); i++ )
            {
                if( !status[i] )
                    continue;

                points[1][k++] = points[1][i];
                circle( frame, points[1][i], 3, Scalar(0,255,0), -1, 8);
            }
            points[1].resize(k);

            if (k < 5)
                ok = false;

            vector<Point> hull;
            Mat mask = generate_new_mask(points[1], hull);

            run_watershed(copy_frame, mask, frame);

            Mat binary_image = get_binary_image(gray);
            run_DFT(binary_image, obj_dft_image, dftSize, obj_size, frame);

            std::swap(points[1], points[0]);
        }

        imshow( "video", frame);
        key = waitKey(20);
        cv::swap(gray, prevGray);
        frame_num++;
    }
    return 0;
};