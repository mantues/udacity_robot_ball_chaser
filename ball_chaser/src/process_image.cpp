#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
//all
cv::Mat image;
//center
cv::Mat c_image;
//left

//right
cv::Mat r_image;
//all
cv::Mat gray_img;
cv::Mat c_gray_img;
cv::Mat l_gray_img;
cv::Mat r_gray_img;
//all
cv::Mat bin_img;
cv::Mat c_bin_img;
cv::Mat l_bin_img;
cv::Mat r_bin_img;

// Define a global client that can request services
ros::ServiceClient client;

int calc_image(cv::Mat img, int originx, int originy, int cropx, int cropy){
    cv::Mat crop_image(img, cv::Rect(int(originx),int(originy),
     int(cropx), int(cropy)));
    //convert gray
    cv::Mat crop_gray_img;
    cvtColor(crop_image, crop_gray_img, cv::COLOR_BGR2GRAY);
    //convert binary
    cv::Mat crop_bin_img;
    threshold(crop_gray_img, crop_bin_img, 240, 255, cv::THRESH_BINARY);
    int num = cv::countNonZero(crop_bin_img);
    return num;
    
}

void put_text_img(cv::Mat img, int number){
    cv::putText(img, "Math: "+std::to_string(number), cv::Point(50,50), cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(255,255,255), 2, CV_AA);
    
    
}

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::ImageConstPtr& msg)
{

    int white_pixel = 255;

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
    
    try {
       image = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image;
       }
    catch (cv_bridge::Exception& e) {
       ROS_ERROR("cv_bridge exception: %s", e.what());
       }
    int w = image.size().width;
    int h = image.size().height;
    
    cv::Mat l_image(image, cv::Rect(0,0, int(0.333*w), h));
    //convert gray
    cvtColor(l_image, l_gray_img, cv::COLOR_BGR2GRAY);
    //convert binary
    threshold(l_gray_img, l_bin_img, 240, 255, cv::THRESH_BINARY);
    int num = cv::countNonZero(l_bin_img);
    int calc = calc_image(image, 0,0, w,h);
    put_text_img(image, calc);
    put_text_img(l_bin_img, num);
    
    cv::imshow("image", image);
    cv::imshow("bin_image", l_bin_img);
    cv::waitKey(1);
    
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
