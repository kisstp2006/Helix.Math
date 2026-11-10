# Helix.Math

Math library of the Helix engine: vectors, quaternions, matrices, colors, geometry, random
numbers and a small SIMD layer.

## Conventions

- Left-handed coordinates, Y up, Z forward.
- `Matrix4x4` is column-major with column vectors (`m<row><column>`), projection matrices map depth to -1..1.
- `Quaternion::Euler(x, y, z)` rotates around Z, then X, then Y; `eulerAngles` returns degrees in 0..360.
- Vector and quaternion equality is approximate (`1e-5` distance, `1 - 1e-6` dot).
- `mathf::Round` rounds half to even, `mathf::Sign(0)` is 1.
- `Random` is a xorshift128 generator; `value` and the float `Range` include both ends, the integer `Range` excludes the max.

## Layout

| Header | Content |
|---|---|
| `Helix/Math/Mathf.h` | Scalar functions (`Lerp`, `SmoothDamp`, `DeltaAngle`, `PerlinNoise`, color space conversion ...) |
| `Helix/Math/Vector.h` | `Vector2`, `Vector3`, `Vector4`, `Vector2Int`, `Vector3Int` |
| `Helix/Math/Quaternion.h` | `Quaternion` |
| `Helix/Math/Matrix4x4.h` | `Matrix4x4` (TRS, projections, inverse) |
| `Helix/Math/Color.h` | `Color`, `Color32`, HSV conversion |
| `Helix/Math/Geometry.h` | `Rect`, `Bounds`, `Ray`, `Plane`, frustum planes, SIMD frustum culling of SoA bounds, batch point transform |
| `Helix/Math/Random.h` | `Random` |
| `Helix/Math/Simd.h` | `simd::float4` over SSE2, NEON, wasm simd128, scalar fallback |
Sources live in `Source/Helix/Math/Implementation/`, tests in `Tests/`, benchmarks in `Benchmarks/`.

## Build

```
git clone --recursive https://github.com/kisstp2006/Helix.Math
cmake --preset windows-release
cmake --build --preset windows-release
ctest --preset windows-release
```

## License

GPL-3.0, see `LICENSE`.
