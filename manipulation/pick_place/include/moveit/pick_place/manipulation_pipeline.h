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

#ifndef MOVEIT_PICK_PLACE_MANIPULATION_PIPELINE_
#define MOVEIT_PICK_PLACE_MANIPULATION_PIPELINE_

#include <moveit/pick_place/manipulation_stage.h>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <vector>
#include <deque>

namespace pick_place
{

class ManipulationPipeline
{
public:
  
  ManipulationPipeline(const std::string &name, unsigned int nthreads);
  virtual ~ManipulationPipeline(void);
  
  const std::string& getName(void) const
  {
    return name_;
  }

  void setSolutionCallback(const boost::function<void(void)> &callback)
  {
    solution_callback_ = callback;
  }
  
  ManipulationPipeline& addStage(const ManipulationStagePtr &next);
  const ManipulationStagePtr& getFirstStage(void) const;
  const ManipulationStagePtr& getLastStage(void) const;
  void reset(void);
  
  void signalStop(void);
  void start(void);  
  void stop(void);  
  
  void push(const ManipulationPlanPtr &grasp);
  void clear(void);
  
  const std::vector<ManipulationPlanPtr>& getSuccessfulManipulationPlans(void) const;
  const std::vector<ManipulationPlanPtr>& getFailedPlans(void) const;
  
protected:
  
  void processingThread(unsigned int index);
  
  std::string name_;  
  unsigned int nthreads_;
  std::vector<ManipulationStagePtr> stages_;
  
  std::deque<ManipulationPlanPtr> queue_;
  std::vector<ManipulationPlanPtr> success_;
  std::vector<ManipulationPlanPtr> failed_;  

  std::vector<boost::thread*> processing_threads_;
  boost::condition_variable queue_access_cond_;
  boost::mutex queue_access_lock_;
  boost::mutex result_lock_;
  
  boost::function<void(void)> solution_callback_;
  
  bool stop_processing_;

};

}

#endif

