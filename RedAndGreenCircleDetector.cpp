#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include <sstream>
using namespace cv;
using namespace std;

//covert double to string
namespace patch
{
    template<typename T> std::string to_string(const T& n)
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}


int main(int argc, char** argv)
{
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cout << "CAP BROKEN" << endl;
		return -1;
	}
	namedWindow("Circle Detector", CV_WINDOW_AUTOSIZE);
	while (1)
	{
		Mat Original;
		bool yes = cap.read(Original);
		if (!yes)
		{
			cout << "MAT BROKEN" << endl;
			break;
		}
		
		Mat NewColor;
		medianBlur(NewColor, NewColor, 5);
		cvtColor(Original, NewColor, COLOR_BGR2HSV);
		
		erode(Original, Original, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));
		dilate(Original, Original, getStructuringElement(MORPH_ELLIPSE,Size(5,5)));

		dilate(Original, Original, getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
		erode(Original, Original, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));
		
		Mat lower_red_hue_range;
		Mat upper_red_hue_range;
		inRange(NewColor, Scalar(0, 100, 100), Scalar(10, 255, 255), lower_red_hue_range);
		inRange(NewColor, Scalar(160, 100, 100), Scalar(179, 255, 255), upper_red_hue_range);
		
		Mat red_hue_image;
		addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);
		
		Mat green_hue_image;
		inRange(NewColor, Scalar(70, 100, 100), Scalar(160, 255, 255), green_hue_image);
		
		GaussianBlur(red_hue_image, red_hue_image, Size(9, 9), 2, 2);
		GaussianBlur(green_hue_image, green_hue_image, Size(9, 9), 2, 2);
		
		vector<Vec3f> circlesRed;
		HoughCircles(red_hue_image, circlesRed, HOUGH_GRADIENT, 1,
					 red_hue_image.rows/16, // min circles distance
					 100, 30,
					 1, 1000); //min, max radius
		
		vector<Vec3f> circlesGreen;
		HoughCircles(green_hue_image, circlesGreen, HOUGH_GRADIENT, 1,
					 green_hue_image.rows/16, // min circles distance
					 100, 30,
					 1, 1000); //min, max radius
		
		for (size_t i = 0; i < circlesRed.size(); i++ )
		{
			int x = round(circlesRed[i][0]);
			int y = round(circlesRed[i][1]);
			Point center(x, y);
			int radius = round(circlesRed[i][2]);
			circle(Original, center, 1, Scalar(0,100,100), 3, LINE_AA);
			circle(Original, center, radius, Scalar(255,0,255), 3, LINE_AA);
			string strX = patch::to_string(x);
			string strY = patch::to_string(y);
			putText(Original, strX + " , " + strY, Point(x-radius/2, y), 
					FONT_HERSHEY_SIMPLEX, 0.3, Scalar(50, 255, 100), 1);
		}
		
		for (size_t i = 0; i < circlesGreen.size(); ++i )
		{
			int x = round(circlesGreen[i][0]);
			int y = round(circlesGreen[i][1]);
			Point center(x, y);
			int radius = round(circlesGreen[i][2]);
			circle(Original, center, 1, Scalar(0,100,100), 3, LINE_AA);
			circle(Original, center, radius, Scalar(255,0,255), 3, LINE_AA);
			string strX = patch::to_string(x);
			string strY = patch::to_string(y);
			putText(Original, strX + " , " + strY, Point(x-radius/2, y), 
					FONT_HERSHEY_SIMPLEX, 0.3, Scalar(50, 255, 100), 1);
		}
		
		imshow("Circle Detector", Original);
		if (waitKey(1) == 27)
		{
			cout << "EXIT" << endl;
			break;
		}
	}
	
	return 0;
}
