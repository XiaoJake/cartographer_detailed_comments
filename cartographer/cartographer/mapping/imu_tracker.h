/*
 * Copyright 2016 The Cartographer Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CARTOGRAPHER_MAPPING_IMU_TRACKER_H_
#define CARTOGRAPHER_MAPPING_IMU_TRACKER_H_

#include "Eigen/Geometry"
#include "cartographer/common/time.h"

namespace cartographer {
namespace mapping {

// Keeps track of the orientation using angular velocities and linear
// accelerations from an IMU. Because averaged linear acceleration (assuming
// slow movement) is a direct measurement of gravity, roll/pitch does not drift,
// though yaw does.
/**
 * @brief ImuTracker 的主要作用就是根据 IMU 的读数维护传感器当前的姿态、线加速度(经过重力校正的)、
 * 当前姿态、重力方向、角速度等量。这些量都是以 ImuTracker 刚建立时的那一时刻 IMU 本身的坐标系为
 * 基准坐标系。
 */
class ImuTracker {
 public:
  ImuTracker(double imu_gravity_time_constant, common::Time time);

  // Advances to the given 'time' and updates the orientation to reflect this.
  void Advance(common::Time time);

  // Updates from an IMU reading (in the IMU frame).
  /**
   * @brief 根据传感器读数更新传感器的最新状态，得到经过重力校正的线加速度、角速度等。
   * @param[in] imu_linear_acceleration 
   */
  void AddImuLinearAccelerationObservation(
      const Eigen::Vector3d& imu_linear_acceleration);
  void AddImuAngularVelocityObservation(
      const Eigen::Vector3d& imu_angular_velocity);

  // Query the current time.
  // 返回当前时间
  common::Time time() const { return time_; }

  // Query the current orientation estimate.
  // 返回当前方向角
  Eigen::Quaterniond orientation() const { return orientation_; }

 private:
  //align 重力的时间间隔
  const double imu_gravity_time_constant_;
  //当前时间
  common::Time time_;
  //记录最新的线加速度的时刻
  common::Time last_linear_acceleration_time_;
  //方向角
  Eigen::Quaterniond orientation_;
  //重力方向
  Eigen::Vector3d gravity_vector_;
  //角速度
  Eigen::Vector3d imu_angular_velocity_;
};

}  // namespace mapping
}  // namespace cartographer

#endif  // CARTOGRAPHER_MAPPING_IMU_TRACKER_H_
