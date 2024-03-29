#include "Constant.h"

namespace raytracing {
float Constant::FInfinity = std::numeric_limits<float>::infinity();
float Constant::FMax = std::numeric_limits<float>::max();
float Constant::FMin = std::numeric_limits<float>::min();
float Constant::FLowest = std::numeric_limits<float>::lowest();

double Constant::DMax = std::numeric_limits<double>::max();
double Constant::DMin = std::numeric_limits<double>::min();
}