#pragma once

#include "BBox.h"
#include "Object.h"

namespace raytracing {

class BVHNode : public Hittable {
   private:
    S<Hittable> left;
    S<Hittable> right;
    BBox bbox;

   public:
    BVHNode() {} /* Empty BVH, default bbox will never hit */
    BVHNode(const std::vector<S<Hittable>>& objects, uint64_t start, uint64_t end);
    virtual ~BVHNode() = default;

    bool Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) const override;

    BBox BoundingBox() const override { return bbox; }

   private:
    static bool box_compare(
        const S<Hittable> a, const S<Hittable> b, int axis_index) {
        return a->BoundingBox().min[axis_index] < b->BoundingBox().min[axis_index];
    }

    static bool box_x_compare(const S<Hittable> a, const S<Hittable> b) {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare(const S<Hittable> a, const S<Hittable> b) {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare(const S<Hittable> a, const S<Hittable> b) {
        return box_compare(a, b, 2);
    }
};

}  // namespace raytracing