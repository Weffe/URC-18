//
// Created by matthew on 10/09/17.
//

#ifndef ROVERIK_IK_JOINT_CONTROLLER_H
#define ROVERIK_IK_JOINT_CONTROLLER_H

#include <controller_interface/controller.h>
#include <hardware_interface/joint_command_interface.h>
#include "../../src/JointPositionControl.h"
#include <kdl/chain.hpp>
#include <kdl/jntarray.hpp>
#include <kdl/frames.hpp>
#include <trac_ik/trac_ik.hpp>
#include <kdl_conversions/kdl_msg.h>
#include <geometry_msgs/Pose.h>
#include <ros/ros.h>
#include <pluginlib/class_list_macros.h>
#include <kdl/chainiksolver.hpp>

namespace rover_ik {

    class IKJointController : public controller_interface::Controller<hardware_interface::EffortJointInterface> {
    public:
        IKJointController() {
            
        }
        ~IKJointController() {
            sub.shutdown();
            delete solver;
        };

        bool init(hardware_interface::EffortJointInterface* hw, ros::NodeHandle& nh_);

        void update(const ros::Time& time, const ros::Duration& period) {
            ctrls->update(period);
        }

        void posCB(const geometry_msgs::Pose &pose) {
            this->retrievePositions();
            KDL::Frame frame;
            tf::poseMsgToKDL(pose, frame);

            KDL::JntArray output;
            int rc = solver->CartToJnt(seed, frame, output);
            if (rc < 0) {
                // do error
                ROS_ERROR_STREAM("Invalid location");
                return;
            }

            std::vector<double> data(output.rows(), 0.0);

            for (int i = 0; i < output.rows(); i++) {
                data[i] = output(i, 0);
            }

            ctrls->updateSetpoints(data);
        }

        void

    private:

        ros::Subscriber sub;

        void retrievePositions() {
            std::vector<double> d = ctrls->getPositions();
            for (int i = 0; i < d.size(); i++) {
                this->seed(i, 0) = d[i];
            }
        };


        KDL::Chain chain;
        TRAC_IK::TRAC_IK *solver;
        KDL::JntArray seed;
        KDL::ChainFkSolverPos *solver2;

        rover_ik::JointPositionControl *ctrls;
    };

    PLUGINLIB_EXPORT_CLASS( rover_ik::IKJointController, controller_interface::ControllerBase);

}

#endif //ROVERIK_IK_JOINT_CONTROLLER_H
