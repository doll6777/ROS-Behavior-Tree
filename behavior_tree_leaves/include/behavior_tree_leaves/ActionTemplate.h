#ifndef __ACTION_TEMPLATE__
#define __ACTION_TEMPLATE__
#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h> // needed for actionlib
#include <behavior_tree_core/BTAction.h>
#include <boost/thread.hpp>

#ifndef __BT_STATUS__
#define __BT_STATUS__
enum Status { RUNNING, SUCCESS, FAILURE }; // BT return status
#endif

class ActionTemplate {
protected:
  ros::NodeHandle nh_;
  actionlib::SimpleActionServer<behavior_tree_core::BTAction> action_server_;
  std::string action_name_;
  behavior_tree_core::BTFeedback feedback_;
  behavior_tree_core::BTResult result_;
  double execution_frequency_;
  boost::mutex action_mtx_;

public:
  ActionTemplate(std::string name)
      : action_server_(nh_, name,
                       boost::bind(&ActionTemplate::executeCB, this, _1),
                       false),
        action_name_(name) {
    // Starts the action server
    action_server_.start();

    if (nh_.hasParam("/behavior_tree/execution_frequency")) {
      nh_.getParam("/behavior_tree/execution_frequency", execution_frequency_);
    } else {
      execution_frequency_ = 1.0;
    }
  }

  ~ActionTemplate() {}

  void executeCB(const behavior_tree_core::BTGoalConstPtr &goal);
  void setStatus(int status);

  // Code to execute if action gets preempted
  virtual void preemptionRoutine() = 0;
  // Code to be run under normal execution. Should set sucess_ to 1 for
  // SUCCESS, -1 for FAILURE.
  virtual int executionRoutine() = 0;
};
#endif
