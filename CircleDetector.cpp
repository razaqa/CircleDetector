#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
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
		
		Mat gray;
		cvtColor(Original, gray, COLOR_BGR2GRAY);
		medianBlur(gray, gray, 5);
		vector<Vec3f> circles;
		HoughCircles(gray, circles, HOUGH_GRADIENT, 1,
					 gray.rows/16, // min circles distance
					 100, 30,
					 1, 100); //min, max radius
		for (size_t i = 0; i < circles.size(); i++ )
		{
			Vec3i c = circles[i];
			Point center = Point(c[0], c[1]);
			std::string strX = patch::to_string(c[0]);
			std::string strY = patch::to_string(c[1]);
			// circle center
			circle(Original, center, 1, Scalar(0,100,100), 3, LINE_AA);
			// circle outline
			int radius = c[2];
			circle(Original, center, radius, Scalar(255,0,255), 3, LINE_AA);
			putText(Original, strX + ", " + strY, Point(c[0], c[1]), 
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
