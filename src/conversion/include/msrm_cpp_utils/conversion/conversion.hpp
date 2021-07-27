// Copyright (c) 2020 - present, Lars Johannsmeier
// All rights reserved.
// contact: lars.johannsmeier@gmail.com

#pragma once

#include <array>

#include "Eigen/Core"

namespace msrm_utils {

/**
 * Converts an Eigen::Matrix type into an std::array type assuming column major.
 * @param[in] m Variable of Eigen::Matrix type.
 * @return An std::array variable with stacked columns from input m.
 */
template<typename T,int S1,int S2> std::array<T,S1*S2> convert_to_array(const Eigen::Matrix<T,S1,S2>& m){
    std::array<T,S1*S2> d_arr;
    Eigen::Matrix<T,S1,S2>::Map(&d_arr[0], m.rows(),m.cols()) = m;
    return d_arr;
}

}
