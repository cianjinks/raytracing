#include "Math/BBox.h"

#include <gtest/gtest.h>

#include "Math/BVH.h"
#include "Math/Ray.h"

namespace raytracing {

/* Test ray intersection with an "empty" bounding box. */
TEST(BBox, Empty) {
    uint32_t seed = 0;

    BBox empty;
    Ray ray;
    ray.origin = glm::vec3(0.0f, 0.0f, -1.0f);
    ray.direction = glm::vec3(0.0f, 0.0f, 1.0f);

    EXPECT_FALSE(empty.Hit(ray, seed, 0.001f, Constant::FInfinity));
    EXPECT_TRUE(empty.Empty());

    ray.origin = glm::vec3(1.0f, 0.0f, -1.0f);
    ray.direction = glm::vec3(0.0f, -1.0f, 0.0f);

    EXPECT_FALSE(empty.Hit(ray, seed, 0.001f, Constant::FInfinity));
}

/* Test ray intersection with a bounding box from (-2, -2, 2) to (2, 2, 2). */
TEST(BBox, BasicHit) {
    uint32_t seed = 0;

    BBox bbox(glm::vec3(-2.0f), glm::vec3(2.0f));

    Ray ray;
    ray.origin = glm::vec3(0.0f, 1.0f, -1.0f);
    ray.direction = glm::vec3(0.0f, 0.0f, 1.0f);

    EXPECT_TRUE(bbox.Hit(ray, seed, 0.001f, Constant::FInfinity));
    EXPECT_FALSE(bbox.Empty());
}

TEST(BBox, Pad) {
    uint32_t seed = 0;
    
    /* Infinitely thin bounding box in X axis. */
    BBox bbox(glm::vec3(0, -1, -1), glm::vec3(0, 1, 1));

    Ray ray;
    ray.origin = glm::vec3(0, 0, -1);
    ray.direction = glm::vec3(0, 0, 1);

    EXPECT_FALSE(bbox.Hit(ray, seed, 0.001f, Constant::FInfinity));

    bbox.Pad();

    EXPECT_TRUE(bbox.Hit(ray, seed, 0.001f, Constant::FInfinity));

    /* TODO: Without padding it causes issues with the BVH construction math and that should be tested for. */
}

}  // namespace raytracing