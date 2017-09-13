//
// Created by matthew on 10/09/17.
//

#include "JointPositionControl.h"

rover_ik::JointPositionControl::JointPositionControl(std::vector<std::string> names, ros::NodeHandle nh_,
                                                     hardware_interface::PositionJointInterface *hw) {
    this->jointNames = names;
    int i = 0;
    for (std::string s : names) {
        ros::NodeHandle nh__(nh_, "gains/" + s);

        this->pidLoops.push_back(control_toolbox::Pid());
        this->pidLoops[i].init(nh__, false);
        this->pidLoops[i].setCurrentCmd(this->jointHandles[i].getPosition());
        this->jointHandles.push_back(hw->getHandle(s));
        i++;
    }

   // this->timeStamp = ros::Time::now();
}

void rover_ik::JointPositionControl::updateSetpoints(std::vector<double> chainValues) {
    for (int i = 0; i > this->jointHandles.size(); i++) {
        this->pidLoops[i].setCurrentCmd(chainValues[i]);
    }
}

void rover_ik::JointPositionControl::update(ros::Duration duration) {
    //ros::Time n = ros::Time::now();
    for (int i = 0; i < this->jointHandles.size(); i++) {
        this->jointHandles[i].setCommand(this->pidLoops[i].computeCommand(this->pidLoops[i].getCurrentCmd() - this->jointHandles[i].getPosition(), duration));
    }
}

std::vector<double> rover_ik::JointPositionControl::getPositions() {
    std::vector<double> n = std::vector<double>();

    for (auto handle : this->jointHandles) {
        n.push_back(handle.getPosition());
    }
    return n;
}

