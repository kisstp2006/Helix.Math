#pragma once

#include <Helix/Math/Quaternion.h>

namespace helix
{
    // Xorshift128 generator: value and float Range are inclusive, integer Range excludes max.
    class Random
    {
    public:
        struct State
        {
            u32 s0, s1, s2, s3;
            bool operator==(const State&) const = default;
        };

        explicit Random(u32 seed = 0) { InitState(seed); }

        void InitState(u32 seed);
        State GetState() const { return state_; }
        void SetState(State s) { state_ = s; }

        u32 NextU32();
        f32 value();
        f32 Range(f32 min, f32 max);
        i32 Range(i32 min, i32 max);
        Vector3 insideUnitSphere();
        Vector3 onUnitSphere();
        Vector2 insideUnitCircle();
        Quaternion rotation();
        Quaternion rotationUniform();

    private:
        State state_{};
    };
}
