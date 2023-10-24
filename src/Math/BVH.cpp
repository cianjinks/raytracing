#include "BVH.h"

namespace raytracing {

BVHNode::BVHNode(const std::vector<S<Hittable>>& objects, uint64_t start, uint64_t end) {
    auto objects_cpy = objects;

    uint32_t axis = Random::Int(0, 2);
    auto comparator = (axis == 0)   ? box_x_compare
                      : (axis == 1) ? box_y_compare
                                    : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects_cpy[start];
    } else if (object_span == 2) {
        if (comparator(objects_cpy[start], objects_cpy[start + 1])) {
            left = objects_cpy[start];
            right = objects_cpy[start + 1];
        } else {
            left = objects_cpy[start + 1];
            right = objects_cpy[start];
        }

        /* Filter out empty bounding boxes. */
        if (left->BoundingBox().Empty()) {
            left = right;
        } else if (right->BoundingBox().Empty()) {
            right = left;
        }
    } else {
        std::sort(objects_cpy.begin() + start, objects_cpy.begin() + end, comparator);

        auto mid = start + object_span / 2;
        left = CreateS<BVHNode>(objects_cpy, start, mid);
        right = CreateS<BVHNode>(objects_cpy, mid, end);
    }

    bbox = BBox(left->BoundingBox(), right->BoundingBox());
}

bool BVHNode::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) const {
    if (!bbox.Hit(ray, t_min, t_max)) {
        return false;
    }

    bool hit_left = left->Hit(ray, t_min, t_max, hit);
    bool hit_right = right->Hit(ray, t_min, hit_left ? hit.t : t_max, hit);

    return hit_left || hit_right;
}

}  // namespace raytracing