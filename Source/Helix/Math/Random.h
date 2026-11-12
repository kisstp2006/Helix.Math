#pragma once

#include <Helix/Math/Color.h>
#include <Helix/Math/Quaternion.h>

namespace helix
{
    class RandomGenerator;

    // The reference's static Random: one generator of the engine (both script languages share it), seeded from the
    // system at the first use unless InitState sets it. The main thread's.
    HX_CLASS(Namespace = "HelixEngine", Static)
    class Random
    {
    public:
        // The generator's state (serializable; C# keeps its words private).
        HX_STRUCT()
        struct State
        {
            HX_FIELD(Private) u32 s0 = 0;
            HX_FIELD(Private) u32 s1 = 0;
            HX_FIELD(Private) u32 s2 = 0;
            HX_FIELD(Private) u32 s3 = 0;
            bool operator==(const State&) const = default;
        };

        HX_FUNCTION(Leaf) static void InitState(i32 seed);
        HX_PROPERTY(Leaf) static State state();
        static void SetState(State value);

        HX_PROPERTY(Leaf) static f32 value();
        HX_FUNCTION(Leaf) static f32 Range(f32 minInclusive, f32 maxInclusive);
        HX_FUNCTION(Leaf) static i32 Range(i32 minInclusive, i32 maxExclusive);
        HX_PROPERTY(Leaf) static Vector3 insideUnitSphere();
        HX_PROPERTY(Leaf) static Vector3 onUnitSphere();
        HX_PROPERTY(Leaf) static Vector2 insideUnitCircle();
        HX_PROPERTY(Leaf) static Vector2 onUnitCircle();
        HX_PROPERTY(Leaf) static Quaternion rotation();
        HX_PROPERTY(Leaf) static Quaternion rotationUniform();
        HX_FUNCTION(Leaf) static Color ColorHSV();
        HX_FUNCTION(Leaf) static Color ColorHSV(f32 hueMin, f32 hueMax);
        HX_FUNCTION(Leaf) static Color ColorHSV(f32 hueMin, f32 hueMax, f32 saturationMin, f32 saturationMax);
        HX_FUNCTION(Leaf) static Color ColorHSV(f32 hueMin, f32 hueMax, f32 saturationMin, f32 saturationMax, f32 valueMin, f32 valueMax);
        HX_FUNCTION(Leaf)
        static Color ColorHSV(f32 hueMin, f32 hueMax, f32 saturationMin, f32 saturationMax, f32 valueMin, f32 valueMax, f32 alphaMin,
                              f32 alphaMax);

        // The generator itself.
        static RandomGenerator& Generator();
    };

    // A xorshift128 generator: value and the float Range include both ends, the integer Range excludes its max (with
    // the bounds swapped each keeps its kind, Range(10, 0) gives 1 to 10).
    class RandomGenerator
    {
    public:
        using State = Random::State;

        explicit RandomGenerator(u32 seed = 0) { InitState(seed); }

        void InitState(u32 seed);
        State GetState() const { return state_; }
        void SetState(State s) { state_ = s; }

        u32 NextU32();
        f32 value();
        f32 Range(f32 minInclusive, f32 maxInclusive);
        i32 Range(i32 minInclusive, i32 maxExclusive);
        Vector3 insideUnitSphere();
        Vector3 onUnitSphere();
        Vector2 insideUnitCircle();
        Vector2 onUnitCircle();
        Quaternion rotation();
        Quaternion rotationUniform();
        Color ColorHSV(f32 hueMin = 0, f32 hueMax = 1, f32 saturationMin = 0, f32 saturationMax = 1, f32 valueMin = 0, f32 valueMax = 1,
                       f32 alphaMin = 1, f32 alphaMax = 1);

    private:
        State state_{};
    };
}
