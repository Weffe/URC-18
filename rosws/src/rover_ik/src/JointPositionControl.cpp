//
// Created by matthew on 10/09/17.
//

#include "JointPositionControl.h"

rover_ik::JointPositionControl::JointPositionControl(std::vector<std::string> names, ros::NodeHandle nh_,
                                                     hardware_interface::EffortJointInterface *hw) {
    std::vector<double> dat(names.size(), 0.0);
    int i = 0;
    for (std::string s : names) {
        this->nodeHandles.push_back(ros::NodeHandle(nh_, "gains/" + s));
        this->jointHandles.push_back(hw->getHandle(s));
        this->pidLoops.push_back(control_toolbox::Pid());
        this->pidLoops[i].init(this->nodeHandles[i], false);
        dat[i] = this->jointHandles[i].getPosition();
        i++;
    }
        this->updateSetpoints(dat);
   // this->timeStamp = ros::Time::now();
}

void rover_ik::JointPositionControl::updateSetpoints(std::vector<double> chainValues) {
    ROS_INFO_STREAM("V" << chainValues[0] << " " << chainValues[2]);
    this->setPoints.writeFromNonRT(chainValues);

}

void rover_ik::JointPositionControl::update(ros::Duration duration) {
    //ros::Time n = ros::Time::now();
    std::vector<double> setpoints = *this->setPoints.readFromRT();
    for (int i = 0; i < this->jointHandles.size(); i++) {
        double a = setpoints[i];
        double v = this->pidLoops[i].computeCommand(a - this->jointHandles[i].getPosition(), duration);
       // ROS_INFO_STREAM(a);
       // ROS_INFO_STREAM("Setting " << i << " to " << v);
        this->jointHandles[i].setCommand(v);
    }
}

std::vector<double> rover_ik::JointPositionControl::getPositions() {
    std::vector<double> n = std::vector<double>();

    for (auto handle : this->jointHandles) {
        n.push_back(handle.getPosition());
    }
    return n;
}

