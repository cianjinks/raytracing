#include <gtest/gtest.h>

#include "Math/BBox.h"
#include "Math/Ray.h"

namespace raytracing {

/* Test ray intersection with an "empty" bounding box. */
TEST(BBox, Empty) {
    BBox empty;
    Ray ray;
    ray.origin = glm::vec3(0.0f, 0.0f, -1.0f);
    ray.direction = glm::vec3(0.0f, 0.0f, 1.0f);

    EXPECT_FALSE(empty.Hit(ray, 0.001f, Constant::FInfinity));
}

TEST(BBox, BasicHit) {
    BBox bbox;
    bbox.min = glm::vec3(-2.0f);
    bbox.max = glm::vec3(2.0f);

    Ray ray;
    ray.origin = glm::vec3(0.0f, 1.0f, -1.0f);
    ray.direction = glm::vec3(0.0f, 0.0f, 1.0f);

    EXPECT_TRUE(bbox.Hit(ray, 0.001f, Constant::FInfinity));
}

}  // namespace raytracing