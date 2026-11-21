#include "physics.h"

namespace Chemical {
    
    bool TriggerBoxCollider::IsColliding(const TriggerBoxCollider& col, BoxColliderConstView other_cols) {
        for(auto [entity, test] : other_cols.each()) {

            std::array<float, 6> diffs;

            diffs[0] = test.min.x - col.max.x;
            if(diffs[0] > 0)
                continue;
            diffs[3] = test.max.x - col.min.x;
            if(diffs[3]< 0)
                continue;
            diffs[1] = test.min.y - col.max.y;
            if(diffs[1] > 0)
                continue;
            diffs[4] = test.max.y - col.min.y;
            if(diffs[4] < 0)
                continue;
            diffs[2] = test.min.z - col.max.z;
            if(diffs[2] > 0)
                continue;
            diffs[5] = test.max.z - col.min.z;
            if(diffs[5] < 0)
                continue;

            return true;
        }
        return false;
    }

    CollisionResponse TriggerBoxCollider::GetCollisionResponse(const TriggerBoxCollider& col, BoxColliderConstView other_cols) {

        CollisionResponse response;
        for(auto [entity, test] : other_cols.each()) {

            std::array<float, 6> diffs;

            diffs[0] = test.min.x - col.max.x;
            if(diffs[0] > 0)
                continue;
            diffs[3] = test.max.x - col.min.x;
            if(diffs[3]< 0)
                continue;
            diffs[1] = test.min.y - col.max.y;
            if(diffs[1] > 0)
                continue;
            diffs[4] = test.max.y - col.min.y;
            if(diffs[4] < 0)
                continue;
            diffs[2] = test.min.z - col.max.z;
            if(diffs[2] > 0)
                continue;
            diffs[5] = test.max.z - col.min.z;
            if(diffs[5] < 0)
                continue;

            float v = 99999.0f;
            int idx = -1;
            for(size_t i = 0; i < diffs.size(); i++) {
                float abs_diff = std::abs(diffs[i]);
                if(abs_diff < v) {
                    idx = i;
                    v = abs_diff;
                }
            }
            if(idx != -1) {
                response.offset[idx % 3] = diffs[idx] - std::copysign(TriggerBoxCollider::COLLISION_RESPONSE_MARGIN, diffs[idx]);
                response.direction_flags.set(idx + 1);
            }
        }
        return response;
    }
    // void BoxCollider::AddTestCollidersToRegistry(entt::registry& r) {
    //     r.emplace<BoxCollider>(r.create(), BoxCollider{{0, 0, 0}, {5, 7, 5}});
    //     r.emplace<BoxCollider>(r.create(), BoxCollider{{8, 2, 2}, {13, 9, 7}});
    //     r.emplace<BoxCollider>(r.create(), BoxCollider{{5, 5, 5}, {20, 20, 20}});
    //     r.emplace<BoxCollider>(r.create(), BoxCollider{{-40, 3, -40}, {40, 4, 40}});
    // };

}
