// -----------------------------------------------------------------------------
//
// Copyright (C) The BioDynaMo Project.
// All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//
// See the LICENSE file distributed with this work for details.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership.
//
// -----------------------------------------------------------------------------

#ifndef DISPLACEMENT_OP_CPU_H_
#define DISPLACEMENT_OP_CPU_H_

#include <array>
#include <cmath>
#include <vector>

#include "bound_space_op.h"
#include "math_util.h"
#include "param.h"
#include "simulation.h"

namespace bdm {

template <typename TSimulation = Simulation<>>
class DisplacementOpCpu {
 public:
  DisplacementOpCpu() {}
  ~DisplacementOpCpu() {}

  void Init() {
    // sim_object_movements.reserve(sim_objects->size());
    auto* grid = TSimulation::GetActive()->GetGrid();
    auto search_radius = grid->GetLargestObjectSize();
    squared_radius = search_radius * search_radius;
  }

  // std::vector<std::array<double, 3>> sim_object_movements;
  double squared_radius;

  template <typename TSimObject>
  void operator()(TSimObject&& sim_object) const {
    const auto& displacement = sim_object.CalculateDisplacement(squared_radius);
    // FIXME changes applied directly
    sim_object.ApplyDisplacement(displacement);
    auto* param = TSimulation::GetActive()->GetParam();
    if (param->bound_space_) {
      ApplyBoundingBox(&sim_object, param->min_bound_, param->max_bound_);
      // grid->SetDimensionThresholds(param->min_bound_, param->max_bound_); FIXME
    }
  }
};

}  // namespace bdm

#endif  // DISPLACEMENT_OP_CPU_H_
