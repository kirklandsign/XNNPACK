// Auto-generated file. Do not edit!
//   Template: src/f32-dwconv2d-chw/3x3p1-wasmsimd.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <wasm_simd128.h>


#include <xnnpack/dwconv.h>
#include <xnnpack/math.h>


// palign with offset of 4
static v128_t palignr4_f32(v128_t a, v128_t b) {
  return wasm_v32x4_shuffle(a, b, 1, 2, 3, 4);
}
// palign with offset of 12
static v128_t palignr12_f32(v128_t a, v128_t b) {
  return wasm_v32x4_shuffle(a, b, 3, 4, 5, 6);
}


void xnn_f32_dwconv2d_chw_ukernel_3x3p1__wasmsimd_arm_1x4(
    size_t input_height,
    size_t input_width,
    const float* input,
    const float* weights,
    const float* zero,
    float* output,
    uint32_t padding_top,
    const union xnn_f32_chw_params params[restrict XNN_MIN_ELEMENTS(1)])
{
  assert(input_height != 0);
  assert(input_width != 0);
  assert(input_width % sizeof(float) == 0);
  assert(padding_top == 1);

  const v128_t vmask = wasm_v128_load(params->scalar.mask);
  const v128_t vmax = wasm_v32x4_load_splat(&params->scalar.max);
  const v128_t vmin = wasm_v32x4_load_splat(&params->scalar.min);

  const v128_t vbias = wasm_v32x4_load_splat(weights);
  const v128_t vk00 = wasm_v32x4_load_splat(weights + 1);
  const v128_t vk01 = wasm_v32x4_load_splat(weights + 2);
  const v128_t vk02 = wasm_v32x4_load_splat(weights + 3);
  const v128_t vk10 = wasm_v32x4_load_splat(weights + 4);
  const v128_t vk11 = wasm_v32x4_load_splat(weights + 5);
  const v128_t vk12 = wasm_v32x4_load_splat(weights + 6);
  const v128_t vk20 = wasm_v32x4_load_splat(weights + 7);
  const v128_t vk21 = wasm_v32x4_load_splat(weights + 8);
  const v128_t vk22 = wasm_v32x4_load_splat(weights + 9);

  const v128_t vzero = wasm_f32x4_splat(0.0f);

  const size_t input_decrement = round_up_po2(input_width, 4 * sizeof(float));

  const float* i0 = zero;
  const float* i1 = input;
  const float* i2 = (const float*) ((uintptr_t) i1 + input_width);

  float* o0 = output;

  size_t output_height = input_height;
  do {
    if XNN_UNPREDICTABLE(output_height < 2) {
      i2 = zero;
    }

    v128_t vi0x0123 = vzero;
    v128_t vi1x0123 = vzero;
    v128_t vi2x0123 = vzero;

    v128_t vi0x4567 = wasm_v128_load(i0);
    i0 += 4;
    v128_t vi1x4567 = wasm_v128_load(i1);
    i1 += 4;
    v128_t vi2x4567 = wasm_v128_load(i2);
    i2 += 4;

    size_t w = input_width;
    for (; w > 4 * sizeof(float); w -= 4 * sizeof(float)) {
      const v128_t vi0x89AB = wasm_v128_load(i0);
      i0 += 4;
      const v128_t vi1x89AB = wasm_v128_load(i1);
      i1 += 4;
      const v128_t vi2x89AB = wasm_v128_load(i2);
      i2 += 4;

      v128_t vo0p0 = wasm_f32x4_add(vbias, wasm_f32x4_mul(vi0x4567, vk01));
      vo0p0 = wasm_f32x4_add(vo0p0, wasm_f32x4_mul(vi1x4567, vk11));
      vo0p0 = wasm_f32x4_add(vo0p0, wasm_f32x4_mul(vi2x4567, vk21));

      const v128_t vi0x3456 = palignr12_f32(vi0x0123, vi0x4567);
      const v128_t vi1x3456 = palignr12_f32(vi1x0123, vi1x4567);
      const v128_t vi2x3456 = palignr12_f32(vi2x0123, vi2x4567);

      vo0p0 = wasm_f32x4_add(vo0p0, wasm_f32x4_mul(vi0x3456, vk00));
      vo0p0 = wasm_f32x4_add(vo0p0, wasm_f32x4_mul(vi1x3456, vk10));
      vo0p0 = wasm_f32x4_add(vo0p0, wasm_f32x4_mul(vi2x3456, vk20));

      vi0x0123 = vi0x4567;
      vi1x0123 = vi1x4567;
      vi2x0123 = vi2x4567;

      const v128_t vi0x5678 = palignr4_f32(vi0x4567, vi0x89AB);
      const v128_t vi1x5678 = palignr4_f32(vi1x4567, vi1x89AB);
      const v128_t vi2x5678 = palignr4_f32(vi2x4567, vi2x89AB);

      vo0p0 = wasm_f32x4_add(vo0p0, wasm_f32x4_mul(vi0x5678, vk02));
      vo0p0 = wasm_f32x4_add(vo0p0, wasm_f32x4_mul(vi1x5678, vk12));
      vo0p0 = wasm_f32x4_add(vo0p0, wasm_f32x4_mul(vi2x5678, vk22));

      vi0x4567 = vi0x89AB;
      vi1x4567 = vi1x89AB;
      vi2x4567 = vi2x89AB;


      v128_t vo0 = wasm_f32x4_max(vo0p0, vmin);
      vo0 = wasm_f32x4_min(vo0, vmax);

      wasm_v128_store(o0, vo0);
      o0 += 4;
    }
    // Always process the last block of 1..4 pixels.
    assert(w >= 1 * sizeof(float));
    assert(w <= 4 * sizeof(float));
    {
      vi0x4567 = wasm_v128_and(vmask, vi0x4567);
      vi1x4567 = wasm_v128_and(vmask, vi1x4567);
      vi2x4567 = wasm_v128_and(vmask, vi2x4567);

      v128_t vo0p0 = wasm_f32x4_add(vbias, wasm_f32x4_mul(vi0x4567, vk01));
      vo0p0 = wasm_f32x4_add(vo0p0, wasm_f32x4_mul(vi1x4567, vk11));
      vo0p0 = wasm_f32x4_add(vo0p0, wasm_f32x4_mul(vi2x4567, vk21));

      const v128_t vi0x3456 = palignr12_f32(vi0x0123, vi0x4567);
      const v128_t vi1x3456 = palignr12_f32(vi1x0123, vi1x4567);
      const v128_t vi2x3456 = palignr12_f32(vi2x0123, vi2x4567);

      vo0p0 = wasm_f32x4_add(vo0p0, wasm_f32x4_mul(vi0x3456, vk00));
      vo0p0 = wasm_f32x4_add(vo0p0, wasm_f32x4_mul(vi1x3456, vk10));
      vo0p0 = wasm_f32x4_add(vo0p0, wasm_f32x4_mul(vi2x3456, vk20));

      const v128_t vi0x5678 = palignr4_f32(vi0x4567, vzero);
      const v128_t vi1x5678 = palignr4_f32(vi1x4567, vzero);
      const v128_t vi2x5678 = palignr4_f32(vi2x4567, vzero);

      vo0p0 = wasm_f32x4_add(vo0p0, wasm_f32x4_mul(vi0x5678, vk02));
      vo0p0 = wasm_f32x4_add(vo0p0, wasm_f32x4_mul(vi1x5678, vk12));
      vo0p0 = wasm_f32x4_add(vo0p0, wasm_f32x4_mul(vi2x5678, vk22));


      v128_t vo0 = wasm_f32x4_max(vo0p0, vmin);
      vo0 = wasm_f32x4_min(vo0, vmax);

      if XNN_LIKELY(w == 4 * sizeof(float)) {
        wasm_v128_store(o0, vo0);
        o0 += 4;
      } else {
        if (w & (2 * sizeof(float))) {
          *((double*) o0) = wasm_f64x2_extract_lane(vo0, 0);
          o0 += 2;

          vo0 = wasm_v32x4_shuffle(vo0, vo0, 2, 3, 0, 1);
        }
        if (w & (1 * sizeof(float))) {
          *o0 = wasm_f32x4_extract_lane(vo0, 0);
          o0 += 1;
        }
      }
    }

    i0 = (const float*) ((uintptr_t) i1 - input_decrement);
    i1 = (const float*) ((uintptr_t) i2 - input_decrement);
    i2 = (const float*) ((uintptr_t) i1 + input_width);


  } while (--output_height != 0);
}
