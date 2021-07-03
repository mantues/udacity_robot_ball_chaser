#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include <algorithm>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// Define a global client that can request services
ros::ServiceClient client;

// put text to image 
void put_text_img(cv::Mat img, int number){
    cv::putText(img, "Math: "+std::to_string(number), cv::Point(50,50), cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(255,255,255), 2, CV_AA);
}

// std::max function
bool comp(int a, int b){
    return (a < b);
}

// calculate area in cropped image
int calc_image(cv::Mat img, int originx, int originy, int cropx, int cropy){
    //crop image
    cv::Mat crop_image(img, cv::Rect(int(originx),int(originy),
     int(cropx), int(cropy)));
    //convert gray image
    cv::Mat crop_gray_img;
    cvtColor(crop_image, crop_gray_img, cv::COLOR_BGR2GRAY);
    //convert binary image
    cv::Mat crop_bin_img;
    threshold(crop_gray_img, crop_bin_img, 240, 255, cv::THRESH_BINARY);
    // calculate area in binary image
    int num = cv::countNonZero(crop_bin_img);
    // put text(area) image
    put_text_img(crop_image, num);
    //cv::imshow("crop_image", crop_image);
    //cv::waitKey(1);
    
    return crop_bin_img, num;
    
}


// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z){
    // TODO: Request a service and pass the velocities to it to drive the robot
    // Create a ROS NodeHandle object
    ros::NodeHandle nh;
    ros::ServiceClient client = nh.serviceClient<ball_chaser::DriveToTarget>("command_robot");
    ball_chaser::DriveToTarget srv;
    //request
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;
    // call service
    if (client.call(srv)) {
    // success
    ROS_INFO("Call Succeed");
    }
    else {
    // error
    ROS_ERROR("Faild to call service wheel");
    }
    
}


// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::ImageConstPtr& msg)
{

    int white_pixel = 255;

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
    cv::Mat image, l_image, c_image, r_image;
    try {
        //convert image message to opencv image using cv_bridge
       image = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image;
       }
    catch (cv_bridge::Exception& e) {
        //error
       ROS_ERROR("cv_bridge exception: %s", e.what());
       }
    // calculate width and height
    int w = image.size().width;
    int h = image.size().height;
    int calc_l, calc_c, calc_r;
    //left area
    l_image, calc_l = calc_image(image, 0,0, int(0.333*w), h);
    //center area
    c_image, calc_c = calc_image(image, int(0.333*w),0, int(0.333*w),h);
    //right area
    r_image, calc_r = calc_image(image, int(0.666*w),0, int(0.333*w),h);

    //setting movement
    //linx:Forward and Backward, rotation:angz
    float linx, angz=0.25;
    // moving robot(keep distance from ball)
    if(std::max({calc_r,calc_l,calc_c})< 6500){
        linx=2.0;
    }
    else if(7000 < std::max({calc_r,calc_l,calc_c})){
        linx=-2.0;
    }
    else{ linx=0.0;}
    //trun left
    if(calc_r < calc_l && calc_c < calc_l){
        drive_robot(linx, angz*-1);
    }
    //move forward and backward
    else if(calc_l < calc_c && calc_r < calc_c){
        drive_robot(linx, 0);
    }
    //turn right
    else if(calc_l < calc_r && calc_c < calc_r){
        drive_robot(linx, angz);
    }
    //stop robot
    else{ drive_robot(0, 0);}
    //check values
    ROS_INFO("L: %d C: %d R: %d", calc_l, calc_c, calc_r);
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    //ros::NodeHandle nh;
    ros::NodeHandle nh("~");
    image_transport::ImageTransport it(nh);
    image_transport::Subscriber image_sub = it.subscribe("/camera/rgb/image_raw", 10, process_image_callback);
    
    

    // Define a client service capable of requesting services from command_robot
    //client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    //ros::Subscriber sub1 = nh.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
