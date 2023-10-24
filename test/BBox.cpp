#include "Math/BBox.h"

#include <gtest/gtest.h>

#include "Math/BVH.h"
#include "Math/Ray.h"

namespace raytracing {

/* Test ray intersection with an "empty" bounding box. */
TEST(BBox, Empty) {
    BBox empty;
    Ray ray;
    ray.origin = glm::vec3(0.0f, 0.0f, -1.0f);
    ray.direction = glm::vec3(0.0f, 0.0f, 1.0f);

    EXPECT_FALSE(empty.Hit(ray, 0.001f, Constant::FInfinity));
    EXPECT_TRUE(empty.Empty());

    ray.origin = glm::vec3(1.0f, 0.0f, -1.0f);
    ray.direction = glm::vec3(0.0f, -1.0f, 0.0f);

    EXPECT_FALSE(empty.Hit(ray, 0.001f, Constant::FInfinity));
}

/* Test ray intersection with a bounding box from (-2, -2, 2) to (2, 2, 2). */
TEST(BBox, BasicHit) {
    BBox bbox(glm::vec3(-2.0f), glm::vec3(2.0f));

    Ray ray;
    ray.origin = glm::vec3(0.0f, 1.0f, -1.0f);
    ray.direction = glm::vec3(0.0f, 0.0f, 1.0f);

    EXPECT_TRUE(bbox.Hit(ray, 0.001f, Constant::FInfinity));
    EXPECT_FALSE(bbox.Empty());
}

}  // namespace raytracing