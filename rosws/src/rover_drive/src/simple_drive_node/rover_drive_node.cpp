//
// Created by matthew on 21/08/17.
//

#include <PCA9685/PCA9685.h>
#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include "constants.h"

PCA9685 *drivePCA;

void leftCallback(const std_msgs::Float32ConstPtr &msg);
void rightCallback(const std_msgs::Float32ConstPtr &msg);

int main(int argc, char **argv) {
    ros::init(argc, argv, "rover_drive_node");
    ros::NodeHandle nh_;

    ROS_INFO("Starting rover_drive_node...");
    int address, bus;
    nh_.param("~pcaAddress", address, 0x40);
    nh_.param("~pcaBus", bus, 0);
    ROS_INFO("Opening PCA9685 on bus ", bus, " address ", address);

    drivePCA = new PCA9685(address, static_cast<uint8_t>(bus));
    try {
        drivePCA->init();
    }
    catch (std::runtime_error error) {
        ROS_FATAL("Failed to open PCA9685: ", error.what());
        exit(1);
    }

    nh_.subscribe("/drive/left", 100, leftCallback);
    nh_.subscribe("/drive/right", 100, rightCallback);
    ROS_INFO("Opened PCA9685 successfully!");
    ros::spin();
}

int servoPWM(int pulse) {
    int pulse_length = 1000000 / 500 / 4096;
    int pulse2 = pulse * 1000;
    pulse2 /= pulse_length;
    return pulse2;
}

void leftCallback(const std_msgs::Float32ConstPtr &msg) {
    int motorValue = servoPWM(static_cast<int>((std::min(std::max(0.0f, msg->data), 1.0f) * MOTOR_OFFSET) + MOTOR_MID));
    for (uint8_t channel : LEFT_WHEELS) {
        drivePCA->setPWM(channel, 0, motorValue);
    }
}

void rightCallback(const std_msgs::Float32ConstPtr &msg) {
    int motorValue = servoPWM(static_cast<int>((std::min(std::max(0.0f, msg->data), 1.0f) * MOTOR_OFFSET) + MOTOR_MID));
    for (uint8_t channel : RIGHT_WHEELS) {
        drivePCA->setPWM(channel, 0, motorValue);
    }
}