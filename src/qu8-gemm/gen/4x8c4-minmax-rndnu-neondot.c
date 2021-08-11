// Auto-generated file. Do not edit!
//   Template: src/qu8-gemm/c4-neondot.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <arm_neon.h>

#include <xnnpack/gemm.h>
#include <xnnpack/math.h>


void xnn_qu8_gemm_minmax_rndnu_ukernel_4x8c4__neondot(
    size_t mr,
    size_t nc,
    size_t kc,
    const uint8_t* restrict a,
    size_t a_stride,
    const void* restrict w,
    uint8_t* restrict c,
    size_t cm_stride,
    size_t cn_stride,
    const union xnn_qu8_conv_minmax_params params[restrict XNN_MIN_ELEMENTS(1)]) XNN_DISABLE_TSAN XNN_DISABLE_MSAN
{
  assert(mr != 0);
  assert(mr <= 4);
  assert(nc != 0);
  assert(kc != 0);
  assert(kc % sizeof(uint8_t) == 0);
  assert(a != NULL);
  assert(w != NULL);
  assert(c != NULL);

  kc = round_up_po2(kc, 4 * sizeof(uint8_t));
  const uint8_t* a0 = a;
  uint8_t* c0 = c;
  const uint8_t* a1 = (const uint8_t*) ((uintptr_t) a0 + a_stride);
  uint8_t* c1 = (uint8_t*) ((uintptr_t) c0 + cm_stride);
  if XNN_UNPREDICTABLE(mr < 2) {
    a1 = a0;
    c1 = c0;
  }
  const uint8_t* a2 = (const uint8_t*) ((uintptr_t) a1 + a_stride);
  uint8_t* c2 = (uint8_t*) ((uintptr_t) c1 + cm_stride);
  if XNN_UNPREDICTABLE(mr <= 2) {
    a2 = a1;
    c2 = c1;
  }
  const uint8_t* a3 = (const uint8_t*) ((uintptr_t) a2 + a_stride);
  uint8_t* c3 = (uint8_t*) ((uintptr_t) c2 + cm_stride);
  if XNN_UNPREDICTABLE(mr != 4) {
    a3 = a2;
    c3 = c2;
  }

  const uint8x16_t vb_zero_point = vld1q_dup_u8(&params->rndnu_neon.kernel_zero_point[0]);

  // Loop over groups of 8 columns.
  do {
    // Initialize accumulators with bias. 8 bias values are loaded from the
    // weight matrix, at the start of the group of 8 columns.
    uint32x4_t vpacc0x0123 = vld1q_u32(w); w = (const void*) ((const uint32_t*) w + 4);
    uint32x4_t vpacc0x4567 = vld1q_u32(w); w = (const void*) ((const uint32_t*) w + 4);
    uint32x4_t vpacc1x0123 = vpacc0x0123;
    uint32x4_t vpacc1x4567 = vpacc0x4567;
    uint32x4_t vpacc2x0123 = vpacc0x0123;
    uint32x4_t vpacc2x4567 = vpacc0x4567;
    uint32x4_t vpacc3x0123 = vpacc0x0123;
    uint32x4_t vpacc3x4567 = vpacc0x4567;
    uint32x4_t vnacc0x0123 = vmovq_n_u32(0);
    uint32x4_t vnacc0x4567 = vmovq_n_u32(0);
    uint32x4_t vnacc1x0123 = vmovq_n_u32(0);
    uint32x4_t vnacc1x4567 = vmovq_n_u32(0);
    uint32x4_t vnacc2x0123 = vmovq_n_u32(0);
    uint32x4_t vnacc2x4567 = vmovq_n_u32(0);
    uint32x4_t vnacc3x0123 = vmovq_n_u32(0);
    uint32x4_t vnacc3x4567 = vmovq_n_u32(0);

    // Inner accumulation loop along the 8 columns.
    size_t k = kc;
    // 2x partial unrolled loop to load 8 bytes at a time.
    while (k >= 8 * sizeof(uint8_t)) {
      // Load a 4x8 block of activations.
      const uint8x8_t va0x01234567 = vld1_u8(a0); a0 += 8;
      const uint8x8_t va1x01234567 = vld1_u8(a1); a1 += 8;
      const uint8x8_t va2x01234567 = vld1_u8(a2); a2 += 8;
      const uint8x8_t va3x01234567 = vld1_u8(a3); a3 += 8;

      // Load a 8x8 block of weights.
      const uint8x16_t vb0123x0123 = vld1q_u8(w); w = (const void*) ((const uint8_t*) w + 16);
      const uint8x16_t vb0123x4567 = vld1q_u8(w); w = (const void*) ((const uint8_t*) w + 16);
      const uint8x16_t vb4567x0123 = vld1q_u8(w); w = (const void*) ((const uint8_t*) w + 16);
      const uint8x16_t vb4567x4567 = vld1q_u8(w); w = (const void*) ((const uint8_t*) w + 16);

      // Multiply-accumulate: 4x8 * 8x8 --> 4x8.
      vpacc0x0123 = vdotq_lane_u32(vpacc0x0123, vb0123x0123, va0x01234567, 0);
      vnacc0x0123 = vdotq_lane_u32(vnacc0x0123, vb_zero_point, va0x01234567, 0);
      vpacc0x4567 = vdotq_lane_u32(vpacc0x4567, vb0123x4567, va0x01234567, 0);
      vnacc0x4567 = vdotq_lane_u32(vnacc0x4567, vb_zero_point, va0x01234567, 0);
      vpacc1x0123 = vdotq_lane_u32(vpacc1x0123, vb0123x0123, va1x01234567, 0);
      vnacc1x0123 = vdotq_lane_u32(vnacc1x0123, vb_zero_point, va1x01234567, 0);
      vpacc1x4567 = vdotq_lane_u32(vpacc1x4567, vb0123x4567, va1x01234567, 0);
      vnacc1x4567 = vdotq_lane_u32(vnacc1x4567, vb_zero_point, va1x01234567, 0);
      vpacc2x0123 = vdotq_lane_u32(vpacc2x0123, vb0123x0123, va2x01234567, 0);
      vnacc2x0123 = vdotq_lane_u32(vnacc2x0123, vb_zero_point, va2x01234567, 0);
      vpacc2x4567 = vdotq_lane_u32(vpacc2x4567, vb0123x4567, va2x01234567, 0);
      vnacc2x4567 = vdotq_lane_u32(vnacc2x4567, vb_zero_point, va2x01234567, 0);
      vpacc3x0123 = vdotq_lane_u32(vpacc3x0123, vb0123x0123, va3x01234567, 0);
      vnacc3x0123 = vdotq_lane_u32(vnacc3x0123, vb_zero_point, va3x01234567, 0);
      vpacc3x4567 = vdotq_lane_u32(vpacc3x4567, vb0123x4567, va3x01234567, 0);
      vnacc3x4567 = vdotq_lane_u32(vnacc3x4567, vb_zero_point, va3x01234567, 0);
      vpacc0x0123 = vdotq_lane_u32(vpacc0x0123, vb4567x0123, va0x01234567, 1);
      vnacc0x0123 = vdotq_lane_u32(vnacc0x0123, vb_zero_point, va0x01234567, 1);
      vpacc0x4567 = vdotq_lane_u32(vpacc0x4567, vb4567x4567, va0x01234567, 1);
      vnacc0x4567 = vdotq_lane_u32(vnacc0x4567, vb_zero_point, va0x01234567, 1);
      vpacc1x0123 = vdotq_lane_u32(vpacc1x0123, vb4567x0123, va1x01234567, 1);
      vnacc1x0123 = vdotq_lane_u32(vnacc1x0123, vb_zero_point, va1x01234567, 1);
      vpacc1x4567 = vdotq_lane_u32(vpacc1x4567, vb4567x4567, va1x01234567, 1);
      vnacc1x4567 = vdotq_lane_u32(vnacc1x4567, vb_zero_point, va1x01234567, 1);
      vpacc2x0123 = vdotq_lane_u32(vpacc2x0123, vb4567x0123, va2x01234567, 1);
      vnacc2x0123 = vdotq_lane_u32(vnacc2x0123, vb_zero_point, va2x01234567, 1);
      vpacc2x4567 = vdotq_lane_u32(vpacc2x4567, vb4567x4567, va2x01234567, 1);
      vnacc2x4567 = vdotq_lane_u32(vnacc2x4567, vb_zero_point, va2x01234567, 1);
      vpacc3x0123 = vdotq_lane_u32(vpacc3x0123, vb4567x0123, va3x01234567, 1);
      vnacc3x0123 = vdotq_lane_u32(vnacc3x0123, vb_zero_point, va3x01234567, 1);
      vpacc3x4567 = vdotq_lane_u32(vpacc3x4567, vb4567x4567, va3x01234567, 1);
      vnacc3x4567 = vdotq_lane_u32(vnacc3x4567, vb_zero_point, va3x01234567, 1);

      k -= 8 * sizeof(uint8_t);
    }
    // Handle up to 4 final positions of `k`
    if XNN_UNLIKELY(k != 0) {
      // Load a 4x4 block of activations.
      const uint8x8_t va0x01234567 = vld1_u8(a0); a0 += 4;
      const uint8x8_t va1x01234567 = vld1_u8(a1); a1 += 4;
      const uint8x8_t va2x01234567 = vld1_u8(a2); a2 += 4;
      const uint8x8_t va3x01234567 = vld1_u8(a3); a3 += 4;

      // Load a 4x8 block of weights.
      const uint8x16_t vb0123x0123 = vld1q_u8(w); w = (const void*) ((const uint8_t*) w + 16);
      const uint8x16_t vb0123x4567 = vld1q_u8(w); w = (const void*) ((const uint8_t*) w + 16);

      // Multiply-accumulate: 4x4 * 4x8 --> 4x8.
      vpacc0x0123 = vdotq_lane_u32(vpacc0x0123, vb0123x0123, va0x01234567, 0);
      vnacc0x0123 = vdotq_lane_u32(vnacc0x0123, vb_zero_point, va0x01234567, 0);
      vpacc0x4567 = vdotq_lane_u32(vpacc0x4567, vb0123x4567, va0x01234567, 0);
      vnacc0x4567 = vdotq_lane_u32(vnacc0x4567, vb_zero_point, va0x01234567, 0);
      vpacc1x0123 = vdotq_lane_u32(vpacc1x0123, vb0123x0123, va1x01234567, 0);
      vnacc1x0123 = vdotq_lane_u32(vnacc1x0123, vb_zero_point, va1x01234567, 0);
      vpacc1x4567 = vdotq_lane_u32(vpacc1x4567, vb0123x4567, va1x01234567, 0);
      vnacc1x4567 = vdotq_lane_u32(vnacc1x4567, vb_zero_point, va1x01234567, 0);
      vpacc2x0123 = vdotq_lane_u32(vpacc2x0123, vb0123x0123, va2x01234567, 0);
      vnacc2x0123 = vdotq_lane_u32(vnacc2x0123, vb_zero_point, va2x01234567, 0);
      vpacc2x4567 = vdotq_lane_u32(vpacc2x4567, vb0123x4567, va2x01234567, 0);
      vnacc2x4567 = vdotq_lane_u32(vnacc2x4567, vb_zero_point, va2x01234567, 0);
      vpacc3x0123 = vdotq_lane_u32(vpacc3x0123, vb0123x0123, va3x01234567, 0);
      vnacc3x0123 = vdotq_lane_u32(vnacc3x0123, vb_zero_point, va3x01234567, 0);
      vpacc3x4567 = vdotq_lane_u32(vpacc3x4567, vb0123x4567, va3x01234567, 0);
      vnacc3x4567 = vdotq_lane_u32(vnacc3x4567, vb_zero_point, va3x01234567, 0);
    }

    // Subtract zero point accumulators with accumulators.
    int32x4_t vacc0x0123 = vreinterpretq_s32_u32(vsubq_u32(vpacc0x0123, vnacc0x0123));
    int32x4_t vacc0x4567 = vreinterpretq_s32_u32(vsubq_u32(vpacc0x4567, vnacc0x4567));
    int32x4_t vacc1x0123 = vreinterpretq_s32_u32(vsubq_u32(vpacc1x0123, vnacc1x0123));
    int32x4_t vacc1x4567 = vreinterpretq_s32_u32(vsubq_u32(vpacc1x4567, vnacc1x4567));
    int32x4_t vacc2x0123 = vreinterpretq_s32_u32(vsubq_u32(vpacc2x0123, vnacc2x0123));
    int32x4_t vacc2x4567 = vreinterpretq_s32_u32(vsubq_u32(vpacc2x4567, vnacc2x4567));
    int32x4_t vacc3x0123 = vreinterpretq_s32_u32(vsubq_u32(vpacc3x0123, vnacc3x0123));
    int32x4_t vacc3x4567 = vreinterpretq_s32_u32(vsubq_u32(vpacc3x4567, vnacc3x4567));

    const int32x4_t vright_pre_shift = vld1q_dup_s32(&params->rndnu_neon.right_pre_shift);
    const int32x4_t vmultiplier = vld1q_dup_s32(&params->rndnu_neon.multiplier);
    const int32x4_t vright_post_shift = vld1q_dup_s32(&params->rndnu_neon.right_post_shift);

    vacc0x0123 = vshlq_s32(vacc0x0123, vright_pre_shift);
    vacc0x4567 = vshlq_s32(vacc0x4567, vright_pre_shift);
    vacc1x0123 = vshlq_s32(vacc1x0123, vright_pre_shift);
    vacc1x4567 = vshlq_s32(vacc1x4567, vright_pre_shift);
    vacc2x0123 = vshlq_s32(vacc2x0123, vright_pre_shift);
    vacc2x4567 = vshlq_s32(vacc2x4567, vright_pre_shift);
    vacc3x0123 = vshlq_s32(vacc3x0123, vright_pre_shift);
    vacc3x4567 = vshlq_s32(vacc3x4567, vright_pre_shift);

    vacc0x0123 = vqdmulhq_s32(vacc0x0123, vmultiplier);
    vacc0x4567 = vqdmulhq_s32(vacc0x4567, vmultiplier);
    vacc1x0123 = vqdmulhq_s32(vacc1x0123, vmultiplier);
    vacc1x4567 = vqdmulhq_s32(vacc1x4567, vmultiplier);
    vacc2x0123 = vqdmulhq_s32(vacc2x0123, vmultiplier);
    vacc2x4567 = vqdmulhq_s32(vacc2x4567, vmultiplier);
    vacc3x0123 = vqdmulhq_s32(vacc3x0123, vmultiplier);
    vacc3x4567 = vqdmulhq_s32(vacc3x4567, vmultiplier);

    vacc0x0123 = vrshlq_s32(vacc0x0123, vright_post_shift);
    vacc0x4567 = vrshlq_s32(vacc0x4567, vright_post_shift);
    vacc1x0123 = vrshlq_s32(vacc1x0123, vright_post_shift);
    vacc1x4567 = vrshlq_s32(vacc1x4567, vright_post_shift);
    vacc2x0123 = vrshlq_s32(vacc2x0123, vright_post_shift);
    vacc2x4567 = vrshlq_s32(vacc2x4567, vright_post_shift);
    vacc3x0123 = vrshlq_s32(vacc3x0123, vright_post_shift);
    vacc3x4567 = vrshlq_s32(vacc3x4567, vright_post_shift);

    const int16x8_t voutput_zero_point = vld1q_dup_s16(&params->rndnu_neon.output_zero_point);
#if XNN_ARCH_ARM64
    const int16x8_t vacc0x01234567 = vqaddq_s16(vqmovn_high_s32(vqmovn_s32(vacc0x0123), vacc0x4567), voutput_zero_point);
    const int16x8_t vacc1x01234567 = vqaddq_s16(vqmovn_high_s32(vqmovn_s32(vacc1x0123), vacc1x4567), voutput_zero_point);
    const int16x8_t vacc2x01234567 = vqaddq_s16(vqmovn_high_s32(vqmovn_s32(vacc2x0123), vacc2x4567), voutput_zero_point);
    const int16x8_t vacc3x01234567 = vqaddq_s16(vqmovn_high_s32(vqmovn_s32(vacc3x0123), vacc3x4567), voutput_zero_point);

    uint8x16_t vout0x01234567_1x01234567 = vqmovun_high_s16(vqmovun_s16(vacc0x01234567), vacc1x01234567);
    uint8x16_t vout2x01234567_3x01234567 = vqmovun_high_s16(vqmovun_s16(vacc2x01234567), vacc3x01234567);
#else
    const int16x8_t vacc0x01234567 = vqaddq_s16(vcombine_s16(vqmovn_s32(vacc0x0123), vqmovn_s32(vacc0x4567)), voutput_zero_point);
    const int16x8_t vacc1x01234567 = vqaddq_s16(vcombine_s16(vqmovn_s32(vacc1x0123), vqmovn_s32(vacc1x4567)), voutput_zero_point);
    const int16x8_t vacc2x01234567 = vqaddq_s16(vcombine_s16(vqmovn_s32(vacc2x0123), vqmovn_s32(vacc2x4567)), voutput_zero_point);
    const int16x8_t vacc3x01234567 = vqaddq_s16(vcombine_s16(vqmovn_s32(vacc3x0123), vqmovn_s32(vacc3x4567)), voutput_zero_point);

    uint8x16_t vout0x01234567_1x01234567 = vcombine_u8(vqmovun_s16(vacc0x01234567), vqmovun_s16(vacc1x01234567));
    uint8x16_t vout2x01234567_3x01234567 = vcombine_u8(vqmovun_s16(vacc2x01234567), vqmovun_s16(vacc3x01234567));
#endif
    const uint8x16_t voutput_min = vld1q_dup_u8(&params->rndnu_neon.output_min);
    const uint8x16_t voutput_max = vld1q_dup_u8(&params->rndnu_neon.output_max);

    vout0x01234567_1x01234567 = vmaxq_u8(vout0x01234567_1x01234567, voutput_min);
    vout2x01234567_3x01234567 = vmaxq_u8(vout2x01234567_3x01234567, voutput_min);

    vout0x01234567_1x01234567 = vminq_u8(vout0x01234567_1x01234567, voutput_max);
    vout2x01234567_3x01234567 = vminq_u8(vout2x01234567_3x01234567, voutput_max);

    if (nc >= 8) {
      vst1_u8(c0 + 0, vget_low_u8(vout0x01234567_1x01234567));
      vst1_u8(c1 + 0, vget_high_u8(vout0x01234567_1x01234567));
      vst1_u8(c2 + 0, vget_low_u8(vout2x01234567_3x01234567));
      vst1_u8(c3 + 0, vget_high_u8(vout2x01234567_3x01234567));

      c0 = (uint8_t*) ((uintptr_t) c0 + cn_stride);
      c1 = (uint8_t*) ((uintptr_t) c1 + cn_stride);
      c2 = (uint8_t*) ((uintptr_t) c2 + cn_stride);
      c3 = (uint8_t*) ((uintptr_t) c3 + cn_stride);

      a0 = (const uint8_t*) ((uintptr_t) a0 - kc);
      a1 = (const uint8_t*) ((uintptr_t) a1 - kc);
      a2 = (const uint8_t*) ((uintptr_t) a2 - kc);
      a3 = (const uint8_t*) ((uintptr_t) a3 - kc);

      nc -= 8;
    } else {
      if (nc & 4) {
        vst1q_lane_u32(__builtin_assume_aligned(c0, 1), vreinterpretq_u32_u8(vout0x01234567_1x01234567), 0); c0 += 4;
        vst1q_lane_u32(__builtin_assume_aligned(c1, 1), vreinterpretq_u32_u8(vout0x01234567_1x01234567), 2); c1 += 4;
        vst1q_lane_u32(__builtin_assume_aligned(c2, 1), vreinterpretq_u32_u8(vout2x01234567_3x01234567), 0); c2 += 4;
        vst1q_lane_u32(__builtin_assume_aligned(c3, 1), vreinterpretq_u32_u8(vout2x01234567_3x01234567), 2); c3 += 4;
        vout0x01234567_1x01234567 = vextq_u8(vout0x01234567_1x01234567, vout0x01234567_1x01234567, 4);
        vout2x01234567_3x01234567 = vextq_u8(vout2x01234567_3x01234567, vout2x01234567_3x01234567, 4);
      }
      if (nc & 2) {
        vst1q_lane_u16(__builtin_assume_aligned(c0, 1), vreinterpretq_u16_u8(vout0x01234567_1x01234567), 0); c0 += 2;
        vst1q_lane_u16(__builtin_assume_aligned(c1, 1), vreinterpretq_u16_u8(vout0x01234567_1x01234567), 4); c1 += 2;
        vst1q_lane_u16(__builtin_assume_aligned(c2, 1), vreinterpretq_u16_u8(vout2x01234567_3x01234567), 0); c2 += 2;
        vst1q_lane_u16(__builtin_assume_aligned(c3, 1), vreinterpretq_u16_u8(vout2x01234567_3x01234567), 4); c3 += 2;
        vout0x01234567_1x01234567 = vextq_u8(vout0x01234567_1x01234567, vout0x01234567_1x01234567, 2);
        vout2x01234567_3x01234567 = vextq_u8(vout2x01234567_3x01234567, vout2x01234567_3x01234567, 2);
      }
      if (nc & 1) {
        vst1q_lane_u8(c0, vout0x01234567_1x01234567, 0);
        vst1q_lane_u8(c1, vout0x01234567_1x01234567, 8);
        vst1q_lane_u8(c2, vout2x01234567_3x01234567, 0);
        vst1q_lane_u8(c3, vout2x01234567_3x01234567, 8);
      }

      nc = 0;
    }
  } while (nc != 0);
}
