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

#ifndef UNIT_DEFAULT_CTPARAM_H_
#define UNIT_DEFAULT_CTPARAM_H_

#include "biodynamo/core/simulation/compile_time_param.h"

namespace bdm {

template <typename TBackend>
struct CompileTimeParam : public DefaultCompileTimeParam<TBackend> {};

}  // namespace bdm

#endif  // UNIT_DEFAULT_CTPARAM_H_
