#pragma once

#include <vector>
#include <utility>
#include <stdint.h>

#include <Eigen/Core>

std::vector< std::pair<size_t, size_t> > align(const Eigen::MatrixXf &ref, const Eigen::MatrixXf &deg);
