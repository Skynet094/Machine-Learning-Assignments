#include "core.hpp";
#include "highgui.hpp"
#include "imgcodecs.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include<bits/stdc++.h>

using namespace cv;
using namespace std;



int main()
{

    VideoCapture cap("input.MOV");

    cap.set(CV_CAP_PROP_CONVERT_RGB, 1);

    cv::Mat image, grey_image;
   double FPS = cap.get(CV_CAP_PROP_FPS);;



    Mat reference_image, grey_reference;

    reference_image = imread("reference.jpg");

    cv::cvtColor(reference_image, grey_reference, cv::COLOR_BGR2GRAY);


    int width = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_WIDTH));

    int height = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_HEIGHT));
    VideoWriter out("putOut.MOV", CV_FOURCC('m','p', '4', 'v'), FPS, cv::Size(width, height));

     cout<<"Width = "<<width<<" Height= "<<height<<endl;

     while(true) {
         cap >> image;
         if(image.empty()) {
             std::cout << "Can't read frames from your camera\n";
             break;
         }

        cv::cvtColor(image, grey_image, cv::COLOR_BGR2GRAY);


        int x= 0 , y=0;


        long long int min_difference= INT_MAX* 1.0;


        for(int i=0; i< grey_image.rows - grey_reference.rows ; i++){

                for(int j=0; j<grey_image.cols - grey_reference.cols; j++){

                    long long int difference=0;

                    for(int m= 0 ; m< grey_reference.rows ; m++){

                        for(int n=0; n< grey_reference.cols ; n++){


                            difference += abs( (int)grey_image.at<uchar>(i+m,j+n) - (int)grey_reference.at<uchar>(m,n)  );
                        }

                    }


                    if(difference <min_difference)
                    {


                       min_difference=difference;
                        x= i, y=j;
                    }


                }

        }



        const int y1= x + 40;
        const int x1= y;

        const int y2= x;
        const int x2= y + 40 ;

        cv::rectangle(
            image,
               cv::Point( x1, y1),
               cv::Point(x2, y2),
               cv::Scalar(255, 0, 0)
        );


         cv::imshow("Camera feed", image);

         out<<image;
         if(cv::waitKey(1000.0/FPS) == 27) break;

     }



    return 0;
}
