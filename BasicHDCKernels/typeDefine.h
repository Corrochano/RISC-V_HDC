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
#pragma once

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdint>

#include <riscv_vector.h>

const size_t ALIGNMENT = 64; 

struct benchmarkResult{
    double seconds;
    double gbs;
    double sseconds;
    double sgbs;
    double speedup;
};

typedef uint64_t hdc_word_t;
typedef uint64_t hdc_score_t;

typedef uint32_t hdc_word_t_32;
typedef uint32_t hdc_score_t_32;

typedef uint16_t hdc_word_t_16;
typedef uint16_t hdc_score_t_16;

typedef uint8_t hdc_word_t_8;
typedef uint8_t hdc_score_t_8;

inline size_t get_rvv_vl_m1(size_t avl, int bits) {
    switch (bits) {
        case 8:
            return __riscv_vsetvl_e8m1(avl);
        case 16:
            return __riscv_vsetvl_e16m1(avl);
        case 32:
            return __riscv_vsetvl_e32m1(avl);
        case 64:
            return __riscv_vsetvl_e64m1(avl);
        default:
            std::cerr << "Unsupported bit width: " << bits << std::endl;
            std::exit(EXIT_FAILURE);    
    }
}

inline size_t get_rvv_vl_m2(size_t avl, int bits) {
    switch (bits) {
        case 8:
            return __riscv_vsetvl_e8m2(avl);
        case 16:
            return __riscv_vsetvl_e16m2(avl);
        case 32:
            return __riscv_vsetvl_e32m2(avl);
        case 64:
            return __riscv_vsetvl_e64m2(avl);
        default:
            std::cerr << "Unsupported bit width: " << bits << std::endl;
            std::exit(EXIT_FAILURE);    
    }
}

inline size_t get_rvv_vl_m3(size_t avl, int bits) {
    switch (bits) {
        case 8:
            return __riscv_vsetvl_e8m3(avl);
        case 16:
            return __riscv_vsetvl_e16m3(avl);
        case 32:
            return __riscv_vsetvl_e32m3(avl);
        case 64:
            return __riscv_vsetvl_e64m3(avl);
        default:
            std::cerr << "Unsupported bit width: " << bits << std::endl;
            std::exit(EXIT_FAILURE);    
    }
}

inline size_t get_rvv_vl_m4(size_t avl, int bits) {
    switch (bits) {
        case 8:
            return __riscv_vsetvl_e8m4(avl);
        case 16:
            return __riscv_vsetvl_e16m4(avl);
        case 32:
            return __riscv_vsetvl_e32m4(avl);
        case 64:
            return __riscv_vsetvl_e64m4(avl);
        default:
            std::cerr << "Unsupported bit width: " << bits << std::endl;
            std::exit(EXIT_FAILURE);    
    }
}

inline size_t get_rvv_vl_m5(size_t avl, int bits) {
    switch (bits) {
        case 8:
            return __riscv_vsetvl_e8m5(avl);
        case 16:
            return __riscv_vsetvl_e16m5(avl);
        case 32:
            return __riscv_vsetvl_e32m5(avl);
        case 64:
            return __riscv_vsetvl_e64m5(avl);
        default:
            std::cerr << "Unsupported bit width: " << bits << std::endl;
            std::exit(EXIT_FAILURE);    
    }
}

inline size_t get_rvv_vl_m6(size_t avl, int bits) {
    switch (bits) {
        case 8:
            return __riscv_vsetvl_e8m6(avl);
        case 16:
            return __riscv_vsetvl_e16m6(avl);
        case 32:
            return __riscv_vsetvl_e32m6(avl);
        case 64:
            return __riscv_vsetvl_e64m6(avl);
        default:
            std::cerr << "Unsupported bit width: " << bits << std::endl;
            std::exit(EXIT_FAILURE);    
    }
}

inline size_t get_rvv_vl_m7(size_t avl, int bits) {
    switch (bits) {
        case 8:
            return __riscv_vsetvl_e8m7(avl);
        case 16:
            return __riscv_vsetvl_e16m7(avl);
        case 32:
            return __riscv_vsetvl_e32m7(avl);
        case 64:
            return __riscv_vsetvl_e64m7(avl);
        default:
            std::cerr << "Unsupported bit width: " << bits << std::endl;
            std::exit(EXIT_FAILURE);    
    }
}

inline size_t get_rvv_vl_m8(size_t avl, int bits) {
    switch (bits) {
        case 8:
            return __riscv_vsetvl_e8m8(avl);
        case 16:
            return __riscv_vsetvl_e16m8(avl);
        case 32:
            return __riscv_vsetvl_e32m8(avl);
        case 64:
            return __riscv_vsetvl_e64m8(avl);
        default:
            std::cerr << "Unsupported bit width: " << bits << std::endl;
            std::exit(EXIT_FAILURE);    
    }
}
