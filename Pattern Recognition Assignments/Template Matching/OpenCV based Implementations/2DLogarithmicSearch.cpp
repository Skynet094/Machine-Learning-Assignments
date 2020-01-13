#include "core.hpp";
#include "highgui.hpp"
#include "imgcodecs.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include<bits/stdc++.h>

using namespace cv;
using namespace std;

struct point{

    int first;
    int second;

    point(){
    }

    point(int x, int y){
        first= x;
        second =y;
    }

};





point Log_Search(int P, point X, Mat grey_image, Mat grey_reference){


    int x_center = X.first;
    int y_center= X.second;

    int K = ceil(log2(P));

    cout<<"Value of K= "<<K<<endl;

    int d1= pow(2, K-1);
    int p=P;
    int min_difference = INT_MAX;
    int newX , newY;

         while(d1 != 1){
  //              cout<<" I AM HERE DOGGY"<<endl;


//                cout<<x_center<<" "<<y_center<<endl;

              //  cout<<d1<<endl;

       //         cout<<"WHY YOU ENTER?"<<endl;

                int dx[9]={0,0,0,d1,-d1,d1,-d1,-d1,d1};
                int dy[9]={0,d1,-d1,0,0,d1,-d1,d1,-d1};

                for(int i=0; i<9; i++){
                    int x_=x_center+dx[i];
                    int y_=y_center + dy[i];
                    int difference=0;

                    if(x_<0 || y_<0 || x_ + grey_reference.rows > grey_image.rows || y_  + grey_reference.cols > grey_image.cols)
                        continue;

                    for(int m= 0 ; m< grey_reference.rows ; m++)
                        for(int n=0; n< grey_reference.cols ; n++){

                               long long int val=  abs((int)grey_image.at<uchar>(m+x_,n+y_) - (int)grey_reference.at<uchar>(m,n));
                                difference += val * val ;
                        }

                //    cout<<difference<<endl;
                    if(difference < min_difference){

                        min_difference= difference;
                        newX= x_, newY=y_;
         //               cout<<"Inside the main loop: "<<x_<<" "<<y_<<endl;

                    }

                }

                x_center = newX;
                y_center= newY;
        // find new center, chop
                d1=d1/2;

                //cout<<"VALUE OF K= "<<K<<endl;
         }

    X.first= x_center;
    X.second = y_center;

    return X;


}



Mat down_sample(int factor, Mat matrix){

    int row =  round( (matrix.rows) * 1.0/factor* 1.0 );
    int col =  round( (matrix.cols) * 1.0 /factor* 1.0 ) ;

    cout<<"row= " << matrix.rows<<" col= "<< matrix.cols<<" max_i= "<< 2*row- 1<<" max_j= "<< 2*col - 1 <<endl;

    cout<<"row= " << row<<" col= "<< col<<endl;


    Mat new_matrix = Mat(row, col, matrix.type());

    for(int i=0; i<row; i++){


        for(int j=0; j<col; j++){

                if( !( (matrix.rows % 2 ==1 && (2*i -1) == matrix.rows) && ((matrix.cols % 2 ==1) && (2* j - 1)== matrix.cols)))
                    new_matrix.at<uchar>(i,j)=matrix.at<uchar>(i*2 + 1,j*2 + 1);
        }


    }

    return new_matrix;

}


int main()
{
//    Mat img;
  //  img = imread("reference.jpg");

   // imshow("Original Image", img);
   // waitKey();

    int P,p, L;
    cout<<"Enter value of P: ";
    cin>>P;
    cout<<endl;
    cout<<"Enter value of L: ";
    cin>>L;
    cout<<endl;

    p=P;
// (0,p ) , (0,-p), (p,0) , (-p, 0), (p, p ) , (-p,-p), (-p, p) ,(p,-p)
    VideoCapture cap("input.MOV");

    cap.set(CV_CAP_PROP_CONVERT_RGB, 1);

    cv::Mat image, grey_image;
   double FPS = cap.get(CV_CAP_PROP_FPS);;



    Mat reference_image, grey_reference;

    reference_image = imread("reference.jpg");



    int width = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_WIDTH));

    int height = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_HEIGHT));
    VideoWriter out("putOut.MOV", CV_FOURCC('m','p', '4', 'v'), FPS, cv::Size(width, height));

     cout<<"Width = "<<width<<" Height= "<<height<<endl;

     cap>>image;



        int x_center= 0 , y_center=0;
        int difference;



        long long int min_difference= INT_MAX* 1.0;

        // initial center

        cv::cvtColor(image, grey_image, cv::COLOR_BGR2GRAY);
        cv::cvtColor(reference_image, grey_reference, cv::COLOR_BGR2GRAY);

  //      cout<<grey_image.rows<<" "<<grey_image.cols<<endl;


        for(int i=0; i< grey_image.rows - grey_reference.rows ; i++){


                for(int j=0; j<grey_image.cols - grey_reference.cols; j++){

                    long long int difference=0;

                for(int m= 0 ; m< grey_reference.rows ; m++)
                        for(int n=0; n< grey_reference.cols ; n++)
                                difference += abs((int)grey_image.at<uchar>(m+i,n+j) - (int)grey_reference.at<uchar>(m,n));



                    if(difference <min_difference)
                    {


                       min_difference=difference;
                        x_center= i, y_center=j;
                    }


                }

        }


    int iter =0;


    stack<Mat> image_array, reference_array;
    Mat filtered_image, filtered_reference;


    Mat search_image, search_reference;
    int dx[9]={0,0,0,1,-1,1,-1,-1,1};
    int dy[9]={0,1,-1,0,0,1,-1,1,-1};


     while(true) {


         cap >> image;
         if(image.empty()) {
             std::cout << "Can't read frames from your camera\n";
             break;
         }

        cv::cvtColor(image, grey_image, cv::COLOR_BGR2GRAY);
        image_array.push(grey_image);
        reference_array.push(grey_reference);

      //  cout<<"IMAGE ROWS: "<<grey_image.rows<< "  IMAGE COLS: "<< grey_image.cols<<endl;

         /// STEP 1: DOWN SAMPLE, PRODUCE LEVEL 2 IMAGE

        // level 0 : got the P and x,y


        //level 1: i) pass level-0 image through low pass filter, ii) down

        Mat prev_image, prev_reference ;

        prev_image= grey_image;
        prev_reference = grey_reference;

        for(int l=0; l< L ; l++){



            Mat sampled_image,sampled_reference;

            GaussianBlur( prev_image , filtered_image , Size( 5, 5 ), 0 ,0 );
            GaussianBlur(prev_reference, filtered_reference, Size(5,5), 0, 0);


        //    sampled_image= down_sample(2, filtered_image);
          //  sampled_reference= down_sample(2, filtered_reference);
        //    cout<< sampled_image.cols * 2  - filtered_image.cols <<" "<< sampled_image.rows * 2 - filtered_image.rows<<endl;

          //  cout<< sampled_reference.cols * 2  - filtered_reference.cols <<" "<< sampled_reference.rows * 2 - filtered_reference.rows<<endl;

  //          cout<< filtered_image.rows<<" "<<filtered_image.cols<<" "<<endl;
//
//             pyrDown( filtered_image, sampled_image, Size(filtered_image.rows/2 , filtered_image.cols/2));
            // cout<<"Rows and cols:  "<< sampled_reference.rows<<" "<< sampled_reference.cols<<endl;

  //           pyrDown( filtered_reference, sampled_reference, Size(filtered_reference.rows/2 , filtered_reference.cols/2));


            image_array.push(sampled_image); // level 1 image   LEVEL 1 IMAGE
            reference_array.push(sampled_reference); // level 1 reference

            prev_image = sampled_image;
            prev_reference= sampled_reference;

         //   cout<<"LOLA?"<<endl;

        }

        point X1, center_X1;

        center_X1.first= x_center;
        center_X1.second= y_center;

        //cout<<"After step 1"<<endl;

        ///STEP 2 : SEARCH IN LEVEL 2 IMAGE

        // i) search in , [-P/4, P/4] X [-P/4, P/4], use log search to get point, find optimum x1, y1

            search_image= image_array.top();
            image_array.pop();

            search_reference= reference_array.top();
            reference_array.pop();


//            cv::imshow("Camera feed", search_image);
  //          cv::imshow("reference feed", search_reference);

    //        if(cv::waitKey( 100 * 1000.0/FPS) == 27) break;


      //  cout<<"search_image.rows=  "<<search_image.rows<<" "<<"Search_image.cols: "<<search_image.cols<<endl;
      //  cout<<"search_reference.rows=  "<<search_reference.rows<<" "<<"Search_reference.cols: "<<search_reference.cols<<endl;

        //cv::imshow("image feed", search_image);

      //   if(cv::waitKey(100* 1000) == 27) break;


        // cv::imshow("reference feed", search_reference);


  //       if(cv::waitKey(100* 1000) == 27) break;
//

        X1 = Log_Search( ceil(P/(2*L*1.0)), point( ceil(x_center/(2*L*1.0)), (y_center/(2*L*1.0)) ), search_image, search_reference);

        //  X1 = exhaustive_search(ceil(P/(2*L*1.0)), search_image, search_reference);

        cout<<"x/4 = "<<x_center/(2*L)<<" "<<"y/4= "<<y_center/(2*L)<<endl;

        cout<<"After step 2"<<" "<<X1.first<<" "<<X1.second<<endl;

        /// STEP 3 : SEARCH IN LEVEL 1 IMAGE

        point center_X2, X2;
        center_X2.first= 2* X1.first;
        center_X2.second=  2* X1.second;

        cout<<"Center_X2 : "<<center_X2.first<<" Center_X2: "<<center_X2.second<<endl;

        for(int l=0; l<L ; l++){




            search_image= image_array.top();
            image_array.pop();
            search_reference= reference_array.top();
            reference_array.pop();


      //      cv::imshow("Camera feed", search_image);
        //    cv::imshow("reference feed", search_reference);

          //  if(cv::waitKey( 100 * 1000.0/FPS) == 27) break;


           min_difference= INT_MAX;

        //    cout<<" Rows "<< search_image.rows <<" Cols "<< search_image.cols<<" rows "<< search_reference.rows<<" "<<search_reference.cols<<endl;

            for(int i=0; i<9; i++){

                    int x_=center_X2.first+  dx[i];
                    int y_=center_X2.second + dy[i];
                    int difference=0;

                    if(x_<0 || y_<0 || x_ + search_reference.rows > search_image.rows || y_  + search_reference.cols > search_image.cols)
                        continue;

                    for(int m= 0 ; m< search_reference.rows ; m++)
                        for(int n=0; n< search_reference.cols ; n++){

                               long long int val=  abs((int)search_image.at<uchar>(m+x_,n+y_) - (int)search_reference.at<uchar>(m,n));
                                difference += val * val ;
                        }

                //    cout<<difference<<endl;
                    if(difference < min_difference){

                        min_difference= difference;
                        X2.first= x_, X2.second=y_;
         //               cout<<"Inside the main loop: "<<x_<<" "<<y_<<endl;

                    }

                }

                center_X2.first= X2.first * 2;
                center_X2.second = X2.second*2;

                cout<<"Optima:  "<<X2.first<< "  "<< X2.second<<endl;

        }


      //  cout<<"After step 3"<<" X_center: "<< X2.first<<" "<<X2.second<<endl;


        // find new center , using old center, (via scaling), do a search [-1, 1] X [-1 X 1]



        x_center = X2.first;
        y_center= X2.second;


//        cout<<"After step 4"<<" "<<x_center<<" "<<y_center<<endl;


       // implement logarithmic search , using that P as a parameter

      //  cout<<x_center<<" "<<y_center<<endl;


     //   cout<<min_difference<<endl;

        const int y1= x_center + 40;
        const int x1= y_center;

        const int y2= x_center;
        const int x2= y_center + 40 ;

        cv::rectangle(
            image,
               cv::Point( x1, y1),
               cv::Point(x2, y2),
               cv::Scalar(0, 0, 255)
        );

         cv::imshow("Camera feed", image);

         out<<image;

         if(cv::waitKey(1000) == 27) break;

        iter++;

   //     if(iter==2)
     //       break;
     }

    return 0;
}
