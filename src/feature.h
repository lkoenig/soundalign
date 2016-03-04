#ifndef FEATURE_H
#define FEATURE_H

#include <Eigen/Core>
#include <Eigen/Dense>

class IFeature {
public:
    virtual Eigen::MatrixXf descriptors() = 0;
};

#endif