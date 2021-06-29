#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

cv::Mat image;

// Define a global client that can request services
ros::ServiceClient client;

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
    cv::imshow("image", image);
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
