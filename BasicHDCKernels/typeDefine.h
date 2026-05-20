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
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdint>

#include <riscv_vector.h>

typedef uint64_t hdc_word_t;
typedef uint64_t hdc_score_t;

size_t get_rvv_vl(size_t avl) {
      return __riscv_vsetvl_e64m1(avl);
}                                   

void hdc_bind(
    const hdc_word_t *x,
    const hdc_word_t *y,
    hdc_word_t *z,
    size_t words)
{   
    size_t i = 0;

    while (i < words){
        // Get VL
        size_t vl = get_rvv_vl(words - i);
        
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

void hdc_hamming(
    const hdc_word_t *x,
    const hdc_word_t *y,
    hdc_score_t *acc,
    size_t words,
    size_t alignment, 
    size_t alloc_size)
{
    size_t i = 0;

    // Get VL
    size_t vl = get_rvv_vl(words - i);

    hdc_word_t *z = (hdc_word_t*)aligned_alloc(alignment, alloc_size);
    //vuint64m1_t vz = __riscv_vle64_v_u64m1(z, vl); // Need it as a vector

    hdc_bind(x,y,z,vl); // Perform XOR

    vuint64m1_t vz = __riscv_vle64_v_u64m1(z, vl); // Need it as a vector
    vbool64_t bz = __riscv_vmsne_vx_u64m1_b64(vz, 0, vl); // Need bool argument

    *acc = __riscv_vcpop_m_b64(bz, vl); // pop count

    free(z);
}

/*
void hdc_query(
    const hdc_word_t *M,
    const hdc_word_t *q,
    hdc_score_t *scores,
    size_t nvec,
    size_t words)
{

    //*scores = 
    for (size_t v = 0; v < nvec; ++v) {
        scores[v] = hdc_hamming(&M[v * words], q, words);
    }
}
*/
