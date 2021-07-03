#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
//TODO: Include the ball_chaser "DriveToTarget" header file
#include "ball_chaser/DriveToTarget.h"

// ROS::Publisher motor commands;
ros::Publisher motor_command_publisher;

// TODO: Create a handle_drive_request callback function that executes whenever a drive_bot service is requested
// This function should publish the requested linear x and angular velocities to the robot wheel joints
// After publishing the requested velocities, a message feedback should be returned with the requested wheel velocities

// ROS::Subscriber
//ros::Subscriber motor_command_subscriber;
//motor_command_subscriber = nh.subscribe("/ball_chaser/command_robot", 10, &Server::odomCallback, this);
// ROS::NodeHandle
//ros::NodeHandle nh;
// ROS::ServiceServer
//ros::ServiceServer service;

bool chaise(ball_chaser::DriveToTarget::Request  &req,
         ball_chaser::DriveToTarget::Response &res){
    
    // Comment
    ROS_INFO("ROS service chaise function");
    // Create a ROS NodeHandle object
    ros::NodeHandle nh;
    ROS_INFO("request:x= %f ang:z=%f",req.linear_x, req.angular_z);
    // ros::publisher
    motor_command_publisher = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    // Create a motor_command object of type geometry_msgs::Twist
    geometry_msgs::Twist motor_command;
    // Set wheel velocities, forward [0.5, 0.0]
    motor_command.linear.x = req.linear_x;
    motor_command.angular.z = req.angular_z;
    // Publish angles to drive the robot
    motor_command_publisher.publish(motor_command);
    //ROS_INFO("sending back response");
   
    return true;
 }


int main(int argc, char** argv)
{
    // Initialize a ROS node
    ROS_INFO("Initialize a ROS node");
    ros::init(argc, argv, "ball_chaser");

    // Inform ROS master that we will be publishing a message of type geometry_msgs::Twist on the robot actuation topic with a publishing queue size of 10
    //motor_command_publisher = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    // TODO: Define a drive /ball_chaser/command_robot service with a handle_drive_request callback function


    // TODO: Delete the loop, move the code to the inside of the callback function and make the necessary changes to publish the requested velocities instead of constant values
    /***while (ros::ok()) {
        // Create a motor_command object of type geometry_msgs::Twist
        geometry_msgs::Twist motor_command;
        // Set wheel velocities, forward [0.5, 0.0]
        motor_command.linear.x = 0.5;
        motor_command.angular.z = 0.0;
        // Publish angles to drive the robot
        motor_command_publisher.publish(motor_command);
    }***/

    // Create a ROS NodeHandle object
    ros::NodeHandle nh;
    // Initialize service
    ros::ServiceServer service = nh.advertiseService("command_robot", chaise);
    // check comment
    ROS_INFO("Ready on");

    // TODO: Handle ROS communication events
    ros::Rate loop_rate(20);
    ros::spin();
    //ros::spinOnce();
    //loop_rate.sleep();

    return 0;
}
