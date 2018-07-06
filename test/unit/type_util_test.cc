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

#include "biodynamo/core/util/type_util.h"
#include <string>
#include <typeinfo>
#include "biodynamo/core/simulation/cell.h"
#include "gtest/gtest.h"

namespace bdm {

TEST(TypeTernaryOperatorTest, True) {
  type_ternary_operator<true, int, double>::type data;
  EXPECT_EQ(std::string("i"), typeid(data).name());
}

TEST(TypeTernaryOperatorTest, False) {
  type_ternary_operator<false, int, double>::type data;
  EXPECT_EQ(std::string("d"), typeid(data).name());
}

TEST(is_soaTest, All) {
  EXPECT_FALSE(is_soa<Scalar>::value);
  EXPECT_TRUE(is_soa<Soa>::value);
  EXPECT_TRUE(is_soa<SoaRef>::value);
}

}  // namespace bdm
