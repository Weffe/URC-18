//
// Created by matthew on 10/09/17.
//

#ifndef ROVER_IK_JOINTPOSITIONCONTROL_H
#define ROVER_IK_JOINTPOSITIONCONTROL_H

#include <control_toolbox/pid.h>

namespace rover_ik {

    class JointPositionControl {

        std::vector<control_toolbox::Pid> pidLoops;

        JointPositionControl(std::vector<std::string> names, ros::NodeHandle nh_);

        void updateSetpoints(std::vector<double> chainValues);

    };

}


#endif //ROVER_IK_JOINTPOSITIONCONTROL_H
