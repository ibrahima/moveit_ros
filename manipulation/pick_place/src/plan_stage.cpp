/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2012, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Willow Garage nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

/* Author: Ioan Sucan */

#include <moveit/pick_place/plan_stage.h>
#include <moveit/kinematic_constraints/utils.h>
#include <ros/console.h>

namespace pick_place
{

PlanStage::PlanStage(const planning_scene::PlanningSceneConstPtr &scene,
                     const planning_pipeline::PlanningPipelinePtr &planning_pipeline) :
  ManipulationStage("plan"),
  planning_scene_(scene),
  planning_pipeline_(planning_pipeline)
{
}

void PlanStage::signalStop(void)
{
  ManipulationStage::signalStop();
  planning_pipeline_->terminate();
}

bool PlanStage::evaluate(const ManipulationPlanPtr &plan) const
{
  moveit_msgs::MotionPlanRequest req;
  moveit_msgs::MotionPlanResponse res;
  req.group_name = plan->planning_group_;
  req.num_planning_attempts = 1;
  req.allowed_planning_time = ros::Duration((plan->timeout_ - ros::WallTime::now()).toSec());

  req.goal_constraints.resize(1, kinematic_constraints::constructGoalConstraints(plan->approach_state_->getJointStateGroup(plan->planning_group_)));
  
  if (!signal_stop_ && planning_pipeline_->generatePlan(planning_scene_, req, res) && res.error_code.val == moveit_msgs::MoveItErrorCodes::SUCCESS)
  {
    plan->trajectories_.insert(plan->trajectories_.begin(), res.trajectory);
    plan->trajectory_start_ = res.trajectory_start;
    plan->trajectory_descriptions_.insert(plan->trajectory_descriptions_.begin(), name_);
    plan->error_code_ = res.error_code;
    return true;
  }
  else
    plan->error_code_ = res.error_code;
  return false;
}

}
