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

/* Author: Ioan Sucan, Sachin Chitta */

#ifndef MOVEIT_PICK_PLACE_APPROACH_AND_TRANSLATE_STAGE_
#define MOVEIT_PICK_PLACE_APPROACH_AND_TRANSLATE_STAGE_

#include <moveit/pick_place/manipulation_stage.h>
#include <moveit/planning_pipeline/planning_pipeline.h>
#include <moveit/trajectory_processing/iterative_time_parameterization.h>

namespace pick_place
{

class ApproachAndTranslateStage : public ManipulationStage
{
public:  
  
  ApproachAndTranslateStage(const planning_scene::PlanningSceneConstPtr &pre_grasp_scene,
                            const planning_scene::PlanningSceneConstPtr &post_grasp_scene,
                            const collision_detection::AllowedCollisionMatrixConstPtr &collision_matrix);
  
  virtual bool evaluate(const ManipulationPlanPtr &plan) const;
  
private:
  
  planning_scene::PlanningSceneConstPtr pre_grasp_planning_scene_;
  planning_scene::PlanningSceneConstPtr post_grasp_planning_scene_;
  collision_detection::AllowedCollisionMatrixConstPtr collision_matrix_;
  trajectory_processing::IterativeParabolicTimeParameterization time_param_;
  
  unsigned int max_goal_count_;
  unsigned int max_fail_;
  double max_step_;
  
};

}

#endif

