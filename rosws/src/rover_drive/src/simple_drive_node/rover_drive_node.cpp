//
// Created by matthew on 21/08/17.
//

#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include "constants.h"
#include "ard_device.h"

void leftCallback(const std_msgs::Float32ConstPtr &msg);
void rightCallback(const std_msgs::Float32ConstPtr &msg);

rover_drive::ARDevice *dev;

int main(int argc, char **argv) {
    ros::init(argc, argv, "rover_drive_node");
    ros::NodeHandle nh_;

    ROS_INFO_STREAM("Starting rover_drive_node...");
    int address, bus;
    nh_.param("~pcaAddress", address, 0x40);
    nh_.param("~pcaBus", bus, 0);
    ROS_INFO_STREAM("Opening PCA9685 on bus " << bus << " address 0x" << std::hex << address);
    try {
        dev = new rover_drive::ARDevice(static_cast<uint8_t>(bus), static_cast<uint8_t>(address));
    }
    catch (std::runtime_error error) {
        ROS_FATAL_STREAM("Failed to open arduino: " << error.what());
        exit(1);
    }

    nh_.subscribe("/drive/left", 100, leftCallback);
    nh_.subscribe("/drive/right", 100, rightCallback);
    for (uint8_t pin : rover_drive::LEFT_WHEELS) {
        dev->openPin(pin);
    }
    for (uint8_t pin : rover_drive::RIGHT_WHEELS) {
        dev->openPin(pin);
    }
    ROS_INFO_STREAM("Opened PCA9685 successfully!");
    ros::waitForShutdown();

}

void leftCallback(const std_msgs::Float32ConstPtr &msg) {
    int motorValue = static_cast<int>((std::min(std::max(0.0f, msg->data), 1.0f) * rover_drive::MOTOR_OFFSET) + rover_drive::MOTOR_MID);
    for (uint8_t channel : rover_drive::LEFT_WHEELS) {
        dev->writeMicroseconds(channel, motorValue);
    }
}

void rightCallback(const std_msgs::Float32ConstPtr &msg) {
    int motorValue = static_cast<int>((std::min(std::max(0.0f, msg->data), 1.0f) * rover_drive::MOTOR_OFFSET) + rover_drive::MOTOR_MID);
    for (uint8_t channel : rover_drive::RIGHT_WHEELS) {
        dev->writeMicroseconds(channel, motorValue);
    }
}