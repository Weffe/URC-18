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

namespace rover_ik {

    class IKJointController : public controller_interface::Controller<hardware_interface::PositionJointInterface> {
    public:
        IKJointController();
        ~IKJointController() {
            sub.shutdown();
        };

        bool init(hardware_interface::PositionJointInterface* hw, ros::NodeHandle& nh_) {
            std::vector<std::string> jnts;
            if (!nh_.getParam("joints", jnts)) {
                ROS_FATAL_STREAM("Failed to get joints.");
                return false;
            }

            std::string tip;
            std::string base;

            if (!nh_.getParam("base_joint", base) || !nh_.getParam("tip_joint", tip)) {
                ROS_FATAL_STREAM("Please specify both base and tip joints");
                return false;
            }

            ctrls = rover_ik::JointPositionControl(jnts, nh_, hw);
            solver = TRAC_IK::TRAC_IK(base, tip);
            if (!solver.getKDLChain(chain)) {
                ROS_FATAL_STREAM("Invalid chain");
                return false;
            }
            seed.resize((unsigned int) jnts.size());
            retrievePositions();
            return true;
        }

        void update(const ros::Time& time, const ros::Duration& period) {
            ctrls.update(period);
        }

    private:

        ros::Subscriber sub;

        void posCB(const geometry_msgs::Pose &pose) {
            this->retrievePositions();
            KDL::Frame frame;
            tf::poseMsgToKDL(pose, frame);

            KDL::JntArray output;
            int rc = solver.CartToJnt(seed, frame, output);
        }

        void retrievePositions() {
            std::vector<double> d = ctrls.getPositions();
            for (int i = 0; i < d.size(); i++) {
                this->seed(i) = d[i];
            }
        };

        KDL::Chain chain;
        TRAC_IK::TRAC_IK solver;
        KDL::JntArray seed;

        rover_ik::JointPositionControl ctrls;
    };

}

#endif //ROVERIK_IK_JOINT_CONTROLLER_H
