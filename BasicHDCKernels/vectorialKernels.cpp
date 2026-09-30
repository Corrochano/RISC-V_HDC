/*
Copyright 2026 Álvaro Corrochano López

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include "vectorialKernels.h"

using namespace std;

template <typename WordT, typename ScoreT>
void hdc_query_impl(
    const WordT *M,
    const WordT *q,
    ScoreT *scores,
    size_t nvec,
    size_t words,
    size_t alignment,
    size_t alloc_size,
    void (*hamming)(const WordT *, const WordT *, ScoreT *, size_t, size_t, size_t))
{
    size_t i = 0;
    while (i < nvec) {
        hamming(&M[i * words], q, &scores[i], words, alignment, alloc_size);
        i++;
    }
}

template <int LMUL, typename WordT>
struct RvvOps;

#define HDC_DEFINE_RVV_OPS(LMUL, WORD_T, VECTOR_T, MASK_T, GET_VL, LOAD, XOR, STORE, CMP, POPCOUNT) \
template <> struct RvvOps<LMUL, WORD_T> { \
    using word_type = WORD_T; \
    using vector_type = VECTOR_T; \
    using mask_type = MASK_T; \
    static size_t vl(size_t remaining) { return GET_VL(remaining, sizeof(WORD_T) * 8); } \
    static vector_type load(const word_type *src, size_t vl) { return LOAD(src, vl); } \
    static vector_type bit_xor(vector_type x, vector_type y, size_t vl) { return XOR(x, y, vl); } \
    static void store(word_type *dst, vector_type value, size_t vl) { STORE(dst, value, vl); } \
    static mask_type nonzero(vector_type value, size_t vl) { return CMP(value, 0, vl); } \
    static size_t popcount(mask_type value, size_t vl) { return POPCOUNT(value, vl); } \
};

HDC_DEFINE_RVV_OPS(
    1, hdc_word_t, vuint64m1_t, vbool64_t, get_rvv_vl_m1,
    __riscv_vle64_v_u64m1, __riscv_vxor_vv_u64m1, __riscv_vse64_v_u64m1,
    __riscv_vmsne_vx_u64m1_b64, __riscv_vcpop_m_b64)
HDC_DEFINE_RVV_OPS(
    1, hdc_word_t_32, vuint32m1_t, vbool32_t, get_rvv_vl_m1,
    __riscv_vle32_v_u32m1, __riscv_vxor_vv_u32m1, __riscv_vse32_v_u32m1,
    __riscv_vmsne_vx_u32m1_b32, __riscv_vcpop_m_b32)
HDC_DEFINE_RVV_OPS(
    1, hdc_word_t_16, vuint16m1_t, vbool16_t, get_rvv_vl_m1,
    __riscv_vle16_v_u16m1, __riscv_vxor_vv_u16m1, __riscv_vse16_v_u16m1,
    __riscv_vmsne_vx_u16m1_b16, __riscv_vcpop_m_b16)
HDC_DEFINE_RVV_OPS(
    1, hdc_word_t_8, vuint8m1_t, vbool8_t, get_rvv_vl_m1,
    __riscv_vle8_v_u8m1, __riscv_vxor_vv_u8m1, __riscv_vse8_v_u8m1,
    __riscv_vmsne_vx_u8m1_b8, __riscv_vcpop_m_b8)
HDC_DEFINE_RVV_OPS(
    2, hdc_word_t, vuint64m2_t, vbool32_t, get_rvv_vl_m2,
    __riscv_vle64_v_u64m2, __riscv_vxor_vv_u64m2, __riscv_vse64_v_u64m2,
    __riscv_vmsne_vx_u64m2_b32, __riscv_vcpop_m_b32)
HDC_DEFINE_RVV_OPS(
    2, hdc_word_t_32, vuint32m2_t, vbool16_t, get_rvv_vl_m2,
    __riscv_vle32_v_u32m2, __riscv_vxor_vv_u32m2, __riscv_vse32_v_u32m2,
    __riscv_vmsne_vx_u32m2_b16, __riscv_vcpop_m_b16)
HDC_DEFINE_RVV_OPS(
    2, hdc_word_t_16, vuint16m2_t, vbool8_t, get_rvv_vl_m2,
    __riscv_vle16_v_u16m2, __riscv_vxor_vv_u16m2, __riscv_vse16_v_u16m2,
    __riscv_vmsne_vx_u16m2_b8, __riscv_vcpop_m_b8)
HDC_DEFINE_RVV_OPS(
    2, hdc_word_t_8, vuint8m2_t, vbool4_t, get_rvv_vl_m2,
    __riscv_vle8_v_u8m2, __riscv_vxor_vv_u8m2, __riscv_vse8_v_u8m2,
    __riscv_vmsne_vx_u8m2_b4, __riscv_vcpop_m_b4)
HDC_DEFINE_RVV_OPS(
    4, hdc_word_t, vuint64m4_t, vbool16_t, get_rvv_vl_m4,
    __riscv_vle64_v_u64m4, __riscv_vxor_vv_u64m4, __riscv_vse64_v_u64m4,
    __riscv_vmsne_vx_u64m4_b16, __riscv_vcpop_m_b16)
HDC_DEFINE_RVV_OPS(
    4, hdc_word_t_32, vuint32m4_t, vbool8_t, get_rvv_vl_m4,
    __riscv_vle32_v_u32m4, __riscv_vxor_vv_u32m4, __riscv_vse32_v_u32m4,
    __riscv_vmsne_vx_u32m4_b8, __riscv_vcpop_m_b8)
HDC_DEFINE_RVV_OPS(
    4, hdc_word_t_16, vuint16m4_t, vbool4_t, get_rvv_vl_m4,
    __riscv_vle16_v_u16m4, __riscv_vxor_vv_u16m4, __riscv_vse16_v_u16m4,
    __riscv_vmsne_vx_u16m4_b4, __riscv_vcpop_m_b4)
HDC_DEFINE_RVV_OPS(
    4, hdc_word_t_8, vuint8m4_t, vbool2_t, get_rvv_vl_m4,
    __riscv_vle8_v_u8m4, __riscv_vxor_vv_u8m4, __riscv_vse8_v_u8m4,
    __riscv_vmsne_vx_u8m4_b2, __riscv_vcpop_m_b2)
HDC_DEFINE_RVV_OPS(
    8, hdc_word_t, vuint64m8_t, vbool8_t, get_rvv_vl_m8,
    __riscv_vle64_v_u64m8, __riscv_vxor_vv_u64m8, __riscv_vse64_v_u64m8,
    __riscv_vmsne_vx_u64m8_b8, __riscv_vcpop_m_b8)
HDC_DEFINE_RVV_OPS(
    8, hdc_word_t_32, vuint32m8_t, vbool4_t, get_rvv_vl_m8,
    __riscv_vle32_v_u32m8, __riscv_vxor_vv_u32m8, __riscv_vse32_v_u32m8,
    __riscv_vmsne_vx_u32m8_b4, __riscv_vcpop_m_b4)
HDC_DEFINE_RVV_OPS(
    8, hdc_word_t_16, vuint16m8_t, vbool2_t, get_rvv_vl_m8,
    __riscv_vle16_v_u16m8, __riscv_vxor_vv_u16m8, __riscv_vse16_v_u16m8,
    __riscv_vmsne_vx_u16m8_b2, __riscv_vcpop_m_b2)
HDC_DEFINE_RVV_OPS(
    8, hdc_word_t_8, vuint8m8_t, vbool1_t, get_rvv_vl_m8,
    __riscv_vle8_v_u8m8, __riscv_vxor_vv_u8m8, __riscv_vse8_v_u8m8,
    __riscv_vmsne_vx_u8m8_b1, __riscv_vcpop_m_b1)

#undef HDC_DEFINE_RVV_OPS

template <typename Ops>
void hdc_bind_impl(
    const typename Ops::word_type *x,
    const typename Ops::word_type *y,
    typename Ops::word_type *z,
    size_t words)
{
    size_t i = 0;
    while (i < words) {
        size_t vl = Ops::vl(words - i);
        auto vx = Ops::load(&x[i], vl);
        auto vy = Ops::load(&y[i], vl);
        Ops::store(&z[i], Ops::bit_xor(vx, vy, vl), vl);
        i += vl;
    }
}

template <typename Ops, typename ScoreT>
void hdc_hamming_impl(
    const typename Ops::word_type *x,
    const typename Ops::word_type *y,
    ScoreT *acc,
    size_t words,
    size_t alignment,
    size_t alloc_size)
{
    using WordT = typename Ops::word_type;
    WordT *z = static_cast<WordT *>(aligned_alloc(alignment, alloc_size));
    hdc_bind_impl<Ops>(x, y, z, words);

    size_t i = 0;
    while (i < words) {
        size_t vl = Ops::vl(words - i);
        auto vz = Ops::load(&z[i], vl);
        *acc += Ops::popcount(Ops::nonzero(vz, vl), vl);
        i += vl;
    }

    free(z);
}

////////////////////////////////////////////////////////////////////////////////////////
//                                      LMUL = 1                                      //
////////////////////////////////////////////////////////////////////////////////////////

//////////////////
// Bind Section //
//////////////////

void hdc_bind_m1(
    const hdc_word_t *x,
    const hdc_word_t *y,
    hdc_word_t *z,
    size_t words)
{
    hdc_bind_impl<RvvOps<1, hdc_word_t>>(x, y, z, words);
}

void hdc_bind_m1(
    const hdc_word_t_32 *x,
    const hdc_word_t_32 *y,
    hdc_word_t_32 *z,
    size_t words)
{
    hdc_bind_impl<RvvOps<1, hdc_word_t_32>>(x, y, z, words);
}

void hdc_bind_m1(
    const hdc_word_t_16 *x,
    const hdc_word_t_16 *y,
    hdc_word_t_16 *z,
    size_t words)
{
    hdc_bind_impl<RvvOps<1, hdc_word_t_16>>(x, y, z, words);
}

void hdc_bind_m1(
    const hdc_word_t_8 *x,
    const hdc_word_t_8 *y,
    hdc_word_t_8 *z,
    size_t words)
{
    hdc_bind_impl<RvvOps<1, hdc_word_t_8>>(x, y, z, words);
}

/////////////////////
// Hamming Section //
/////////////////////

void hdc_hamming_m1(
    const hdc_word_t *x,
    const hdc_word_t *y,
    hdc_score_t *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_hamming_impl<RvvOps<1, hdc_word_t>, hdc_score_t>(x, y, acc, words, alignment, alloc_size);
}

void hdc_hamming_m1(
    const hdc_word_t_32 *x,
    const hdc_word_t_32 *y,
    hdc_score_t_32 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_hamming_impl<RvvOps<1, hdc_word_t_32>, hdc_score_t_32>(x, y, acc, words, alignment, alloc_size);
}

void hdc_hamming_m1(
    const hdc_word_t_16 *x,
    const hdc_word_t_16 *y,
    hdc_score_t_16 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_hamming_impl<RvvOps<1, hdc_word_t_16>, hdc_score_t_16>(x, y, acc, words, alignment, alloc_size);
}

void hdc_hamming_m1(
    const hdc_word_t_8 *x,
    const hdc_word_t_8 *y,
    hdc_score_t_8 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_hamming_impl<RvvOps<1, hdc_word_t_8>, hdc_score_t_8>(x, y, acc, words, alignment, alloc_size);
}

///////////////////
// Query Section //
///////////////////

void hdc_query_m1(
    const hdc_word_t *M,
    const hdc_word_t *q,
    hdc_score_t *scores,
    size_t nvec,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_query_impl<hdc_word_t, hdc_score_t>(M, q, scores, nvec, words, alignment, alloc_size, hdc_hamming_m1);
}

void hdc_query_m1(
    const hdc_word_t_32 *M,
    const hdc_word_t_32 *q,
    hdc_score_t_32 *scores,
    size_t nvec,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_query_impl<hdc_word_t_32, hdc_score_t_32>(M, q, scores, nvec, words, alignment, alloc_size, hdc_hamming_m1);
}

void hdc_query_m1(
    const hdc_word_t_16 *M,
    const hdc_word_t_16 *q,
    hdc_score_t_16 *scores,
    size_t nvec,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_query_impl<hdc_word_t_16, hdc_score_t_16>(M, q, scores, nvec, words, alignment, alloc_size, hdc_hamming_m1);
}

void hdc_query_m1(
    const hdc_word_t_8 *M,
    const hdc_word_t_8 *q,
    hdc_score_t_8 *scores,
    size_t nvec,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_query_impl<hdc_word_t_8, hdc_score_t_8>(M, q, scores, nvec, words, alignment, alloc_size, hdc_hamming_m1);
}

////////////////////////////////////////////////////////////////////////////////////////
//                                      LMUL = 2                                      //
////////////////////////////////////////////////////////////////////////////////////////

//////////////////
// Bind Section //
//////////////////

void hdc_bind_m2(
    const hdc_word_t *x,
    const hdc_word_t *y,
    hdc_word_t *z,
    size_t words)
{
    hdc_bind_impl<RvvOps<2, hdc_word_t>>(x, y, z, words);
}

void hdc_bind_m2(
    const hdc_word_t_32 *x,
    const hdc_word_t_32 *y,
    hdc_word_t_32 *z,
    size_t words)
{
    hdc_bind_impl<RvvOps<2, hdc_word_t_32>>(x, y, z, words);
}

void hdc_bind_m2(
    const hdc_word_t_16 *x,
    const hdc_word_t_16 *y,
    hdc_word_t_16 *z,
    size_t words)
{
    hdc_bind_impl<RvvOps<2, hdc_word_t_16>>(x, y, z, words);
}

void hdc_bind_m2(
    const hdc_word_t_8 *x,
    const hdc_word_t_8 *y,
    hdc_word_t_8 *z,
    size_t words)
{
    hdc_bind_impl<RvvOps<2, hdc_word_t_8>>(x, y, z, words);
}

/////////////////////
// Hamming Section //
/////////////////////

void hdc_hamming_m2(
    const hdc_word_t *x,
    const hdc_word_t *y,
    hdc_score_t *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_hamming_impl<RvvOps<2, hdc_word_t>, hdc_score_t>(x, y, acc, words, alignment, alloc_size);
}

void hdc_hamming_m2(
    const hdc_word_t_32 *x,
    const hdc_word_t_32 *y,
    hdc_score_t_32 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_hamming_impl<RvvOps<2, hdc_word_t_32>, hdc_score_t_32>(x, y, acc, words, alignment, alloc_size);
}

void hdc_hamming_m2(
    const hdc_word_t_16 *x,
    const hdc_word_t_16 *y,
    hdc_score_t_16 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_hamming_impl<RvvOps<2, hdc_word_t_16>, hdc_score_t_16>(x, y, acc, words, alignment, alloc_size);
}

void hdc_hamming_m2(
    const hdc_word_t_8 *x,
    const hdc_word_t_8 *y,
    hdc_score_t_8 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_hamming_impl<RvvOps<2, hdc_word_t_8>, hdc_score_t_8>(x, y, acc, words, alignment, alloc_size);
}

///////////////////
// Query Section //
///////////////////

void hdc_query_m2(
    const hdc_word_t *M,
    const hdc_word_t *q,
    hdc_score_t *scores,
    size_t nvec,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_query_impl<hdc_word_t, hdc_score_t>(M, q, scores, nvec, words, alignment, alloc_size, hdc_hamming_m2);
}

void hdc_query_m2(
    const hdc_word_t_32 *M,
    const hdc_word_t_32 *q,
    hdc_score_t_32 *scores,
    size_t nvec,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_query_impl<hdc_word_t_32, hdc_score_t_32>(M, q, scores, nvec, words, alignment, alloc_size, hdc_hamming_m2);
}

void hdc_query_m2(
    const hdc_word_t_16 *M,
    const hdc_word_t_16 *q,
    hdc_score_t_16 *scores,
    size_t nvec,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_query_impl<hdc_word_t_16, hdc_score_t_16>(M, q, scores, nvec, words, alignment, alloc_size, hdc_hamming_m2);
}

void hdc_query_m2(
    const hdc_word_t_8 *M,
    const hdc_word_t_8 *q,
    hdc_score_t_8 *scores,
    size_t nvec,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_query_impl<hdc_word_t_8, hdc_score_t_8>(M, q, scores, nvec, words, alignment, alloc_size, hdc_hamming_m2);
}

////////////////////////////////////////////////////////////////////////////////////////
//                                      LMUL = 4                                      //
////////////////////////////////////////////////////////////////////////////////////////

//////////////////
// Bind Section //
//////////////////

void hdc_bind_m4(
    const hdc_word_t *x,
    const hdc_word_t *y,
    hdc_word_t *z,
    size_t words)
{
    hdc_bind_impl<RvvOps<4, hdc_word_t>>(x, y, z, words);
}

void hdc_bind_m4(
    const hdc_word_t_32 *x,
    const hdc_word_t_32 *y,
    hdc_word_t_32 *z,
    size_t words)
{
    hdc_bind_impl<RvvOps<4, hdc_word_t_32>>(x, y, z, words);
}

void hdc_bind_m4(
    const hdc_word_t_16 *x,
    const hdc_word_t_16 *y,
    hdc_word_t_16 *z,
    size_t words)
{
    hdc_bind_impl<RvvOps<4, hdc_word_t_16>>(x, y, z, words);
}

void hdc_bind_m4(
    const hdc_word_t_8 *x,
    const hdc_word_t_8 *y,
    hdc_word_t_8 *z,
    size_t words)
{
    hdc_bind_impl<RvvOps<4, hdc_word_t_8>>(x, y, z, words);
}

/////////////////////
// Hamming Section //
/////////////////////

void hdc_hamming_m4(
    const hdc_word_t *x,
    const hdc_word_t *y,
    hdc_score_t *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_hamming_impl<RvvOps<4, hdc_word_t>, hdc_score_t>(x, y, acc, words, alignment, alloc_size);
}

void hdc_hamming_m4 (
    const hdc_word_t_32 *x,
    const hdc_word_t_32 *y,
    hdc_score_t_32 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_hamming_impl<RvvOps<4, hdc_word_t_32>, hdc_score_t_32>(x, y, acc, words, alignment, alloc_size);
}

void hdc_hamming_m4(
    const hdc_word_t_16 *x,
    const hdc_word_t_16 *y,
    hdc_score_t_16 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_hamming_impl<RvvOps<4, hdc_word_t_16>, hdc_score_t_16>(x, y, acc, words, alignment, alloc_size);
}

void hdc_hamming_m4(
    const hdc_word_t_8 *x,
    const hdc_word_t_8 *y,
    hdc_score_t_8 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_hamming_impl<RvvOps<4, hdc_word_t_8>, hdc_score_t_8>(x, y, acc, words, alignment, alloc_size);
}

///////////////////
// Query Section //
///////////////////

void hdc_query_m4(
    const hdc_word_t *M,
    const hdc_word_t *q,
    hdc_score_t *scores,
    size_t nvec,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_query_impl<hdc_word_t, hdc_score_t>(M, q, scores, nvec, words, alignment, alloc_size, hdc_hamming_m4);
}

void hdc_query_m4(
    const hdc_word_t_32 *M,
    const hdc_word_t_32 *q,
    hdc_score_t_32 *scores,
    size_t nvec,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_query_impl<hdc_word_t_32, hdc_score_t_32>(M, q, scores, nvec, words, alignment, alloc_size, hdc_hamming_m4);
}

void hdc_query_m4(
    const hdc_word_t_16 *M,
    const hdc_word_t_16 *q,
    hdc_score_t_16 *scores,
    size_t nvec,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_query_impl<hdc_word_t_16, hdc_score_t_16>(M, q, scores, nvec, words, alignment, alloc_size, hdc_hamming_m4);
}

void hdc_query_m4(
    const hdc_word_t_8 *M,
    const hdc_word_t_8 *q,
    hdc_score_t_8 *scores,
    size_t nvec,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_query_impl<hdc_word_t_8, hdc_score_t_8>(M, q, scores, nvec, words, alignment, alloc_size, hdc_hamming_m4);
}

////////////////////////////////////////////////////////////////////////////////////////
//                                      LMUL = 8                                      //
////////////////////////////////////////////////////////////////////////////////////////

//////////////////
// Bind Section //
//////////////////

void hdc_bind_m8(
    const hdc_word_t *x,
    const hdc_word_t *y,
    hdc_word_t *z,
    size_t words)
{
    hdc_bind_impl<RvvOps<8, hdc_word_t>>(x, y, z, words);
}

void hdc_bind_m8(
    const hdc_word_t_32 *x,
    const hdc_word_t_32 *y,
    hdc_word_t_32 *z,
    size_t words)
{
    hdc_bind_impl<RvvOps<8, hdc_word_t_32>>(x, y, z, words);
}

void hdc_bind_m8(
    const hdc_word_t_16 *x,
    const hdc_word_t_16 *y,
    hdc_word_t_16 *z,
    size_t words)
{
    hdc_bind_impl<RvvOps<8, hdc_word_t_16>>(x, y, z, words);
}

void hdc_bind_m8(
    const hdc_word_t_8 *x,
    const hdc_word_t_8 *y,
    hdc_word_t_8 *z,
    size_t words)
{
    hdc_bind_impl<RvvOps<8, hdc_word_t_8>>(x, y, z, words);
}

/////////////////////
// Hamming Section //
/////////////////////

void hdc_hamming_m8(
    const hdc_word_t *x,
    const hdc_word_t *y,
    hdc_score_t *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_hamming_impl<RvvOps<8, hdc_word_t>, hdc_score_t>(x, y, acc, words, alignment, alloc_size);
}

void hdc_hamming_m8(
    const hdc_word_t_32 *x,
    const hdc_word_t_32 *y,
    hdc_score_t_32 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_hamming_impl<RvvOps<8, hdc_word_t_32>, hdc_score_t_32>(x, y, acc, words, alignment, alloc_size);
}

void hdc_hamming_m8(
    const hdc_word_t_16 *x,
    const hdc_word_t_16 *y,
    hdc_score_t_16 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_hamming_impl<RvvOps<8, hdc_word_t_16>, hdc_score_t_16>(x, y, acc, words, alignment, alloc_size);
}

void hdc_hamming_m8(
    const hdc_word_t_8 *x,
    const hdc_word_t_8 *y,
    hdc_score_t_8 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_hamming_impl<RvvOps<8, hdc_word_t_8>, hdc_score_t_8>(x, y, acc, words, alignment, alloc_size);
}

///////////////////
// Query Section //
///////////////////

void hdc_query_m8(
    const hdc_word_t *M,
    const hdc_word_t *q,
    hdc_score_t *scores,
    size_t nvec,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_query_impl<hdc_word_t, hdc_score_t>(M, q, scores, nvec, words, alignment, alloc_size, hdc_hamming_m8);
}

void hdc_query_m8(
    const hdc_word_t_32 *M,
    const hdc_word_t_32 *q,
    hdc_score_t_32 *scores,
    size_t nvec,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_query_impl<hdc_word_t_32, hdc_score_t_32>(M, q, scores, nvec, words, alignment, alloc_size, hdc_hamming_m8);
}

void hdc_query_m8(
    const hdc_word_t_16 *M,
    const hdc_word_t_16 *q,
    hdc_score_t_16 *scores,
    size_t nvec,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_query_impl<hdc_word_t_16, hdc_score_t_16>(M, q, scores, nvec, words, alignment, alloc_size, hdc_hamming_m8);
}

void hdc_query_m8(
    const hdc_word_t_8 *M,
    const hdc_word_t_8 *q,
    hdc_score_t_8 *scores,
    size_t nvec,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_query_impl<hdc_word_t_8, hdc_score_t_8>(M, q, scores, nvec, words, alignment, alloc_size, hdc_hamming_m8);
}
