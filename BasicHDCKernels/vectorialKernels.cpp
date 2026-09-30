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
    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m1(words - i, 64);
        
        // Load as vectors
        vuint64m1_t vx = __riscv_vle64_v_u64m1(&x[i], vl);
        vuint64m1_t vy = __riscv_vle64_v_u64m1(&y[i], vl);

        // Execute xor
        vuint64m1_t vz = __riscv_vxor_vv_u64m1(vx,vy,vl);

        // Save data into z
        __riscv_vse64_v_u64m1(&z[i], vz, vl);

        // Advance words
        i += vl;
    }
}

void hdc_bind_m1(
    const hdc_word_t_32 *x,
    const hdc_word_t_32 *y,
    hdc_word_t_32 *z,
    size_t words)
{   
    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m1(words - i, 32);
        
        // Load as vectors
        vuint32m1_t vx = __riscv_vle32_v_u32m1(&x[i], vl);
        vuint32m1_t vy = __riscv_vle32_v_u32m1(&y[i], vl);

        // Execute xor
        vuint32m1_t vz = __riscv_vxor_vv_u32m1(vx,vy,vl);

        // Save data into z
        __riscv_vse32_v_u32m1(&z[i], vz, vl);

        // Advance words
        i += vl;
    }
}

void hdc_bind_m1(
    const hdc_word_t_16 *x,
    const hdc_word_t_16 *y,
    hdc_word_t_16 *z,
    size_t words)
{   
    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m1(words - i, 16);
        
        // Load as vectors
        vuint16m1_t vx = __riscv_vle16_v_u16m1(&x[i], vl);
        vuint16m1_t vy = __riscv_vle16_v_u16m1(&y[i], vl);

        // Execute xor
        vuint16m1_t vz = __riscv_vxor_vv_u16m1(vx,vy,vl);

        // Save data into z
        __riscv_vse16_v_u16m1(&z[i], vz, vl);

        // Advance words
        i += vl;
    }
}

void hdc_bind_m1(
    const hdc_word_t_8 *x,
    const hdc_word_t_8 *y,
    hdc_word_t_8 *z,
    size_t words)
{   
    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m1(words - i, 8);
        
        // Load as vectors
        vuint8m1_t vx = __riscv_vle8_v_u8m1(&x[i], vl);
        vuint8m1_t vy = __riscv_vle8_v_u8m1(&y[i], vl);

        // Execute xor
        vuint8m1_t vz = __riscv_vxor_vv_u8m1(vx,vy,vl);

        // Save data into z
        __riscv_vse8_v_u8m1(&z[i], vz, vl);

        // Advance words
        i += vl;
    }
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
    hdc_word_t *z = (hdc_word_t*)aligned_alloc(alignment, alloc_size);
    //vuint64m1_t vz = __riscv_vle64_v_u64m1(z, vl); // Need it as a vector

    hdc_bind_m1(x,y,z,words); // Perform XOR

    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m1(words - i, 64);
        vuint64m1_t vz = __riscv_vle64_v_u64m1(z, vl); // Need it as a vector    
        vbool64_t bz = __riscv_vmsne_vx_u64m1_b64(vz, 0, vl); // Need bool argument

        *acc += __riscv_vcpop_m_b64(bz, vl); // pop count

        i += vl;
    }

    free(z);
}

void hdc_hamming_m1(
    const hdc_word_t_32 *x,
    const hdc_word_t_32 *y,
    hdc_score_t_32 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_word_t_32 *z = (hdc_word_t_32*)aligned_alloc(alignment, alloc_size);

    hdc_bind_m1(x,y,z,words); // Perform XOR

    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m1(words - i, 32);
        vuint32m1_t vz = __riscv_vle32_v_u32m1(z, vl); // Need it as a vector    
        vbool32_t bz = __riscv_vmsne_vx_u32m1_b32(vz, 0, vl); // Need bool argument

        *acc += __riscv_vcpop_m_b32(bz, vl); // pop count

        i += vl;
    }

    free(z);
}

void hdc_hamming_m1(
    const hdc_word_t_16 *x,
    const hdc_word_t_16 *y,
    hdc_score_t_16 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_word_t_16 *z = (hdc_word_t_16*)aligned_alloc(alignment, alloc_size);

    hdc_bind_m1(x,y,z,words); // Perform XOR

    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m1(words - i, 16);
        vuint16m1_t vz = __riscv_vle16_v_u16m1(z, vl); // Need it as a vector    
        vbool16_t bz = __riscv_vmsne_vx_u16m1_b16(vz, 0, vl); // Need bool argument

        *acc += __riscv_vcpop_m_b16(bz, vl); // pop count

        i += vl;
    }

    free(z);
}

void hdc_hamming_m1(
    const hdc_word_t_8 *x,
    const hdc_word_t_8 *y,
    hdc_score_t_8 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_word_t_8 *z = (hdc_word_t_8*)aligned_alloc(alignment, alloc_size);

    hdc_bind_m1(x,y,z,words); // Perform XOR

    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m1(words - i, 8);
        vuint8m1_t vz = __riscv_vle8_v_u8m1(z, vl); // Need it as a vector    
        vbool8_t bz = __riscv_vmsne_vx_u8m1_b8(vz, 0, vl); // Need bool argument

        *acc += __riscv_vcpop_m_b8(bz, vl); // pop count

        i += vl;
    }

    free(z);
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
    size_t i = 0;
    while (i < nvec){
        hdc_hamming_m1(&M[i * words], q, &scores[i], words, alignment, alloc_size);
        
        i++;
    }
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
    size_t i = 0;
    while (i < nvec){
        hdc_hamming_m1(&M[i * words], q, &scores[i], words, alignment, alloc_size);
        
        i++;
    }
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
    size_t i = 0;
    while (i < nvec){
        hdc_hamming_m1(&M[i * words], q, &scores[i], words, alignment, alloc_size);
        
        i++;
    }
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
    size_t i = 0;
    while (i < nvec){
        hdc_hamming_m1(&M[i * words], q, &scores[i], words, alignment, alloc_size);
        
        i++;
    }
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
    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m2(words - i, 64);
        
        // Load as vectors
        vuint64m2_t vx = __riscv_vle64_v_u64m2(&x[i], vl);
        vuint64m2_t vy = __riscv_vle64_v_u64m2(&y[i], vl);

        // Execute xor
        vuint64m2_t vz = __riscv_vxor_vv_u64m2(vx,vy,vl);

        // Save data into z
        __riscv_vse64_v_u64m2(&z[i], vz, vl);

        // Advance words
        i += vl;
    }
}

void hdc_bind_m2(
    const hdc_word_t_32 *x,
    const hdc_word_t_32 *y,
    hdc_word_t_32 *z,
    size_t words)
{   
    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m2(words - i, 32);
        
        // Load as vectors
        vuint32m2_t vx = __riscv_vle32_v_u32m2(&x[i], vl);
        vuint32m2_t vy = __riscv_vle32_v_u32m2(&y[i], vl);

        // Execute xor
        vuint32m2_t vz = __riscv_vxor_vv_u32m2(vx,vy,vl);

        // Save data into z
        __riscv_vse32_v_u32m2(&z[i], vz, vl);

        // Advance words
        i += vl;
    }
}

void hdc_bind_m2(
    const hdc_word_t_16 *x,
    const hdc_word_t_16 *y,
    hdc_word_t_16 *z,
    size_t words)
{   
    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m2(words - i, 16);
        
        // Load as vectors
        vuint16m2_t vx = __riscv_vle16_v_u16m2(&x[i], vl);
        vuint16m2_t vy = __riscv_vle16_v_u16m2(&y[i], vl);

        // Execute xor
        vuint16m2_t vz = __riscv_vxor_vv_u16m2(vx,vy,vl);

        // Save data into z
        __riscv_vse16_v_u16m2(&z[i], vz, vl);

        // Advance words
        i += vl;
    }
}

void hdc_bind_m2(
    const hdc_word_t_8 *x,
    const hdc_word_t_8 *y,
    hdc_word_t_8 *z,
    size_t words)
{   
    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m2(words - i, 8);
        
        // Load as vectors
        vuint8m2_t vx = __riscv_vle8_v_u8m2(&x[i], vl);
        vuint8m2_t vy = __riscv_vle8_v_u8m2(&y[i], vl);

        // Execute xor
        vuint8m2_t vz = __riscv_vxor_vv_u8m2(vx,vy,vl);

        // Save data into z
        __riscv_vse8_v_u8m2(&z[i], vz, vl);

        // Advance words
        i += vl;
    }
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
    hdc_word_t *z = (hdc_word_t*)aligned_alloc(alignment, alloc_size);
    //vuint64m2_t vz = __riscv_vle64_v_u64m2(z, vl); // Need it as a vector

    hdc_bind_m2(x,y,z,words); // Perform XOR

    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m2(words - i, 64);
        vuint64m2_t vz = __riscv_vle64_v_u64m2(z, vl); // Need it as a vector    
        vbool32_t bz = __riscv_vmsne_vx_u64m2_b32(vz, 0, vl);

        *acc += __riscv_vcpop_m_b32(bz, vl); // pop count

        i += vl;
    }

    free(z);
}

void hdc_hamming_m2(
    const hdc_word_t_32 *x,
    const hdc_word_t_32 *y,
    hdc_score_t_32 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_word_t_32 *z = (hdc_word_t_32*)aligned_alloc(alignment, alloc_size);

    hdc_bind_m2(x,y,z,words); // Perform XOR

    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m2(words - i, 32);
        vuint32m2_t vz = __riscv_vle32_v_u32m2(z, vl); // Need it as a vector    
        vbool16_t bz = __riscv_vmsne_vx_u32m2_b16(vz, 0, vl);

        *acc += __riscv_vcpop_m_b16(bz, vl); // pop count

        i += vl;
    }

    free(z);
}

void hdc_hamming_m2(
    const hdc_word_t_16 *x,
    const hdc_word_t_16 *y,
    hdc_score_t_16 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_word_t_16 *z = (hdc_word_t_16*)aligned_alloc(alignment, alloc_size);

    hdc_bind_m2(x,y,z,words); // Perform XOR

    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m2(words - i, 16);
        vuint16m2_t vz = __riscv_vle16_v_u16m2(z, vl); // Need it as a vector    
        vbool8_t bz = __riscv_vmsne_vx_u16m2_b8(vz, 0, vl);

        *acc += __riscv_vcpop_m_b8(bz, vl); // pop count

        i += vl;
    }

    free(z);
}

void hdc_hamming_m2(
    const hdc_word_t_8 *x,
    const hdc_word_t_8 *y,
    hdc_score_t_8 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_word_t_8 *z = (hdc_word_t_8*)aligned_alloc(alignment, alloc_size);

    hdc_bind_m2(x,y,z,words); // Perform XOR

    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m2(words - i, 8);
        vuint8m2_t vz = __riscv_vle8_v_u8m2(z, vl); // Need it as a vector    
        vbool4_t bz = __riscv_vmsne_vx_u8m2_b4(vz, 0, vl);

        *acc += __riscv_vcpop_m_b4(bz, vl); // pop count

        i += vl;
    }

    free(z);
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
    size_t i = 0;
    while (i < nvec){
        hdc_hamming_m2(&M[i * words], q, &scores[i], words, alignment, alloc_size);
        
        i++;
    }
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
    size_t i = 0;
    while (i < nvec){
        hdc_hamming_m2(&M[i * words], q, &scores[i], words, alignment, alloc_size);
        
        i++;
    }
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
    size_t i = 0;
    while (i < nvec){
        hdc_hamming_m2(&M[i * words], q, &scores[i], words, alignment, alloc_size);
        
        i++;
    }
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
    size_t i = 0;
    while (i < nvec){
        hdc_hamming_m2(&M[i * words], q, &scores[i], words, alignment, alloc_size);
        
        i++;
    }
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
    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m4(words - i, 64);
        
        // Load as vectors
        vuint64m4_t vx = __riscv_vle64_v_u64m4(&x[i], vl);
        vuint64m4_t vy = __riscv_vle64_v_u64m4(&y[i], vl);

        // Execute xor
        vuint64m4_t vz = __riscv_vxor_vv_u64m4(vx,vy,vl);

        // Save data into z
        __riscv_vse64_v_u64m4(&z[i], vz, vl);

        // Advance words
        i += vl;
    }
}

void hdc_bind_m4(
    const hdc_word_t_32 *x,
    const hdc_word_t_32 *y,
    hdc_word_t_32 *z,
    size_t words)
{   
    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m4(words - i, 32);
        
        // Load as vectors
        vuint32m4_t vx = __riscv_vle32_v_u32m4(&x[i], vl);
        vuint32m4_t vy = __riscv_vle32_v_u32m4(&y[i], vl);

        // Execute xor
        vuint32m4_t vz = __riscv_vxor_vv_u32m4(vx,vy,vl);

        // Save data into z
        __riscv_vse32_v_u32m4(&z[i], vz, vl);

        // Advance words
        i += vl;
    }
}

void hdc_bind_m4(
    const hdc_word_t_16 *x,
    const hdc_word_t_16 *y,
    hdc_word_t_16 *z,
    size_t words)
{   
    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m4(words - i, 16);
        
        // Load as vectors
        vuint16m4_t vx = __riscv_vle16_v_u16m4(&x[i], vl);
        vuint16m4_t vy = __riscv_vle16_v_u16m4(&y[i], vl);

        // Execute xor
        vuint16m4_t vz = __riscv_vxor_vv_u16m4(vx,vy,vl);

        // Save data into z
        __riscv_vse16_v_u16m4(&z[i], vz, vl);

        // Advance words
        i += vl;
    }
}

void hdc_bind_m4(
    const hdc_word_t_8 *x,
    const hdc_word_t_8 *y,
    hdc_word_t_8 *z,
    size_t words)
{   
    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m4(words - i, 8);
        
        // Load as vectors
        vuint8m4_t vx = __riscv_vle8_v_u8m4(&x[i], vl);
        vuint8m4_t vy = __riscv_vle8_v_u8m4(&y[i], vl);

        // Execute xor
        vuint8m4_t vz = __riscv_vxor_vv_u8m4(vx,vy,vl);

        // Save data into z
        __riscv_vse8_v_u8m4(&z[i], vz, vl);

        // Advance words
        i += vl;
    }
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
    hdc_word_t *z = (hdc_word_t*)aligned_alloc(alignment, alloc_size);
    //vuint64m4_t vz = __riscv_vle64_v_u64m4(z, vl); // Need it as a vector

    hdc_bind_m4(x,y,z,words); // Perform XOR

    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m4(words - i, 64);
        vuint64m4_t vz = __riscv_vle64_v_u64m4(z, vl); // Need it as a vector    
        vbool16_t bz = __riscv_vmsne_vx_u64m4_b16(vz, 0, vl);

        *acc += __riscv_vcpop_m_b16(bz, vl); // pop count

        i += vl;
    }

    free(z);
}

void hdc_hamming_m4 (
    const hdc_word_t_32 *x,
    const hdc_word_t_32 *y,
    hdc_score_t_32 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_word_t_32 *z = (hdc_word_t_32*)aligned_alloc(alignment, alloc_size);

    hdc_bind_m4(x,y,z,words); // Perform XOR

    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m4(words - i, 32);
        vuint32m4_t vz = __riscv_vle32_v_u32m4(z, vl); // Need it as a vector    
        vbool8_t bz = __riscv_vmsne_vx_u32m4_b8(vz, 0, vl);

        *acc += __riscv_vcpop_m_b8(bz, vl); // pop count

        i += vl;
    }

    free(z);
}

void hdc_hamming_m4(
    const hdc_word_t_16 *x,
    const hdc_word_t_16 *y,
    hdc_score_t_16 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_word_t_16 *z = (hdc_word_t_16*)aligned_alloc(alignment, alloc_size);

    hdc_bind_m4(x,y,z,words); // Perform XOR

    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m4(words - i, 16);
        vuint16m4_t vz = __riscv_vle16_v_u16m4(z, vl); // Need it as a vector    
        vbool4_t bz = __riscv_vmsne_vx_u16m4_b4(vz, 0, vl);

        *acc += __riscv_vcpop_m_b4(bz, vl); // pop count

        i += vl;
    }

    free(z);
}

void hdc_hamming_m4(
    const hdc_word_t_8 *x,
    const hdc_word_t_8 *y,
    hdc_score_t_8 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_word_t_8 *z = (hdc_word_t_8*)aligned_alloc(alignment, alloc_size);

    hdc_bind_m4(x,y,z,words); // Perform XOR

    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m4(words - i, 8);
        vuint8m4_t vz = __riscv_vle8_v_u8m4(z, vl); // Need it as a vector    
        vbool2_t bz = __riscv_vmsne_vx_u8m4_b2(vz, 0, vl);

        *acc += __riscv_vcpop_m_b2(bz, vl); // pop count

        i += vl;
    }

    free(z);
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
    size_t i = 0;
    while (i < nvec){
        hdc_hamming_m4(&M[i * words], q, &scores[i], words, alignment, alloc_size);
        
        i++;
    }
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
    size_t i = 0;
    while (i < nvec){
        hdc_hamming_m4(&M[i * words], q, &scores[i], words, alignment, alloc_size);
        
        i++;
    }
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
    size_t i = 0;
    while (i < nvec){
        hdc_hamming_m4(&M[i * words], q, &scores[i], words, alignment, alloc_size);
        
        i++;
    }
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
    size_t i = 0;
    while (i < nvec){
        hdc_hamming_m4(&M[i * words], q, &scores[i], words, alignment, alloc_size);
        
        i++;
    }
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
    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m8(words - i, 64);
        
        // Load as vectors
        vuint64m8_t vx = __riscv_vle64_v_u64m8(&x[i], vl);
        vuint64m8_t vy = __riscv_vle64_v_u64m8(&y[i], vl);

        // Execute xor
        vuint64m8_t vz = __riscv_vxor_vv_u64m8(vx,vy,vl);

        // Save data into z
        __riscv_vse64_v_u64m8(&z[i], vz, vl);

        // Advance words
        i += vl;
    }
}

void hdc_bind_m8(
    const hdc_word_t_32 *x,
    const hdc_word_t_32 *y,
    hdc_word_t_32 *z,
    size_t words)
{   
    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m8(words - i, 32);
        
        // Load as vectors
        vuint32m8_t vx = __riscv_vle32_v_u32m8(&x[i], vl);
        vuint32m8_t vy = __riscv_vle32_v_u32m8(&y[i], vl);

        // Execute xor
        vuint32m8_t vz = __riscv_vxor_vv_u32m8(vx,vy,vl);

        // Save data into z
        __riscv_vse32_v_u32m8(&z[i], vz, vl);

        // Advance words
        i += vl;
    }
}

void hdc_bind_m8(
    const hdc_word_t_16 *x,
    const hdc_word_t_16 *y,
    hdc_word_t_16 *z,
    size_t words)
{   
    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m8(words - i, 16);
        
        // Load as vectors
        vuint16m8_t vx = __riscv_vle16_v_u16m8(&x[i], vl);
        vuint16m8_t vy = __riscv_vle16_v_u16m8(&y[i], vl);

        // Execute xor
        vuint16m8_t vz = __riscv_vxor_vv_u16m8(vx,vy,vl);

        // Save data into z
        __riscv_vse16_v_u16m8(&z[i], vz, vl);

        // Advance words
        i += vl;
    }
}

void hdc_bind_m8(
    const hdc_word_t_8 *x,
    const hdc_word_t_8 *y,
    hdc_word_t_8 *z,
    size_t words)
{   
    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m8(words - i, 8);
        
        // Load as vectors
        vuint8m8_t vx = __riscv_vle8_v_u8m8(&x[i], vl);
        vuint8m8_t vy = __riscv_vle8_v_u8m8(&y[i], vl);

        // Execute xor
        vuint8m8_t vz = __riscv_vxor_vv_u8m8(vx,vy,vl);

        // Save data into z
        __riscv_vse8_v_u8m8(&z[i], vz, vl);

        // Advance words
        i += vl;
    }
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
    hdc_word_t *z = (hdc_word_t*)aligned_alloc(alignment, alloc_size);
    //vuint64m8_t vz = __riscv_vle64_v_u64m8(z, vl); // Need it as a vector

    hdc_bind_m4(x,y,z,words); // Perform XOR

    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m8(words - i, 64);
        vuint64m8_t vz = __riscv_vle64_v_u64m8(z, vl); // Need it as a vector    
        vbool8_t bz = __riscv_vmsne_vx_u64m8_b8(vz, 0, vl);

        *acc += __riscv_vcpop_m_b8(bz, vl); // pop count

        i += vl;
    }

    free(z);
}

void hdc_hamming_m8(
    const hdc_word_t_32 *x,
    const hdc_word_t_32 *y,
    hdc_score_t_32 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_word_t_32 *z = (hdc_word_t_32*)aligned_alloc(alignment, alloc_size);

    hdc_bind_m8(x,y,z,words); // Perform XOR

    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m8(words - i, 32);
        vuint32m8_t vz = __riscv_vle32_v_u32m8(z, vl); // Need it as a vector    
        vbool4_t bz = __riscv_vmsne_vx_u32m8_b4(vz, 0, vl);

        *acc += __riscv_vcpop_m_b4(bz, vl); // pop count

        i += vl;
    }

    free(z);
}

void hdc_hamming_m8(
    const hdc_word_t_16 *x,
    const hdc_word_t_16 *y,
    hdc_score_t_16 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_word_t_16 *z = (hdc_word_t_16*)aligned_alloc(alignment, alloc_size);

    hdc_bind_m8(x,y,z,words); // Perform XOR

    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m8(words - i, 16);
        vuint16m8_t vz = __riscv_vle16_v_u16m8(z, vl); // Need it as a vector    
        vbool2_t bz = __riscv_vmsne_vx_u16m8_b2(vz, 0, vl);

        *acc += __riscv_vcpop_m_b2(bz, vl); // pop count

        i += vl;
    }

    free(z);
}

void hdc_hamming_m8(
    const hdc_word_t_8 *x,
    const hdc_word_t_8 *y,
    hdc_score_t_8 *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    hdc_word_t_8 *z = (hdc_word_t_8*)aligned_alloc(alignment, alloc_size);

    hdc_bind_m8(x,y,z,words); // Perform XOR

    size_t i = 0;
    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl_m8(words - i, 8);
        vuint8m8_t vz = __riscv_vle8_v_u8m8(z, vl); // Need it as a vector    
        vbool1_t bz = __riscv_vmsne_vx_u8m8_b1(vz, 0, vl);

        *acc += __riscv_vcpop_m_b1(bz, vl); // pop count

        i += vl;
    }

    free(z);
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
    size_t i = 0;
    while (i < nvec){
        hdc_hamming_m8(&M[i * words], q, &scores[i], words, alignment, alloc_size);
        
        i++;
    }
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
    size_t i = 0;
    while (i < nvec){
        hdc_hamming_m8(&M[i * words], q, &scores[i], words, alignment, alloc_size);
        
        i++;
    }
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
    size_t i = 0;
    while (i < nvec){
        hdc_hamming_m8(&M[i * words], q, &scores[i], words, alignment, alloc_size);
        
        i++;
    }
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
    size_t i = 0;
    while (i < nvec){
        hdc_hamming_m8(&M[i * words], q, &scores[i], words, alignment, alloc_size);
        
        i++;
    }
}
