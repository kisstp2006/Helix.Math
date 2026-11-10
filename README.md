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
| `helix/math/mathf.h` | Scalar functions (`Lerp`, `SmoothDamp`, `DeltaAngle`, `PerlinNoise`, color space conversion ...) |
| `helix/math/vector.h` | `Vector2`, `Vector3`, `Vector4`, `Vector2Int`, `Vector3Int` |
| `helix/math/quaternion.h` | `Quaternion` |
| `helix/math/matrix4x4.h` | `Matrix4x4` (TRS, projections, inverse) |
| `helix/math/color.h` | `Color`, `Color32`, HSV conversion |
| `helix/math/geometry.h` | `Rect`, `Bounds`, `Ray`, `Plane`, frustum planes, SIMD frustum culling of SoA bounds, batch point transform |
| `helix/math/random.h` | `Random` |
| `helix/math/simd.h` | `simd::float4` over SSE2, NEON, wasm simd128, scalar fallback |

## Build

```
git clone --recursive https://github.com/kisstp2006/Helix.Math
cmake --preset windows-release
cmake --build --preset windows-release
ctest --preset windows-release
```

## License

GPL-3.0, see `LICENSE`.
