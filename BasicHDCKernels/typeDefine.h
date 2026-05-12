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
typedef uint64_t  hdc_score_t;

size_t get_rvv_vl() {
      return __riscv_vsetvlmax_e64m1();
}                                   

void hdc_bind(
    const hdc_word_t *x,
    const hdc_word_t *y,
    hdc_word_t *z,
    size_t vl)
{   
    // Load as vectors
    vuint64m1_t vx = __riscv_vle64_v_u64m1(x, vl);
    vuint64m1_t vy = __riscv_vle64_v_u64m1(y, vl);

    // Execute xor
    vuint64m1_t vz = __riscv_vxor_vv_u64m1(vx,vy,vl);

    // Save data into z
    __riscv_vse64_v_u64m1(z, vz, vl);
}

void hdc_hamming(
    const hdc_word_t *x,
    const hdc_word_t *y,
    hdc_score_t *acc,
    size_t vl)
{
    hdc_word_t *z;

    hdc_bind(x,y,z,vl); // Perform XOR
    vuint64m1_t vz = __riscv_vle64_v_u64m1(z, vl); // Need it as a vector
    
    vbool64_t bz = __riscv_vmsne_vx_u64m1_b64(vz, 0, vl); // Need bool argument 

    vuint64m1_t vacc = __riscv_viota_m_u64m1(bz, vl); // pop count

    __riscv_vse64_v_u64m1(acc, vacc, vl);

    free(z);
}
