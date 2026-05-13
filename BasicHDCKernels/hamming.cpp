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
#include "typeDefine.h"

using namespace std;

int main(){
    size_t words = 4;
    size_t alignment = 64;
    size_t total_bytes = words * sizeof(hdc_word_t);
    size_t alloc_size = total_bytes + alignment;

    printf("Sizes created\n");

    hdc_word_t *x = (hdc_word_t*)aligned_alloc(alignment, alloc_size);
    hdc_word_t *y = (hdc_word_t*)aligned_alloc(alignment, alloc_size);
    hdc_score_t *acc = (hdc_score_t*)aligned_alloc(alignment, alloc_size); 

    x[0] = 1; x[1] = 0; x[2] = 5; x[3] = 0;
    y[0] = 1; y[1] = 2; y[2] = 5; y[3] = 8;

    size_t vl = get_rvv_vl();

    printf("Hamming starts\n");

    hdc_hamming(x,y,acc,vl,alignment,alloc_size);

    printf("Score: %ln\n", acc);

    free(x);
    free(y);
    free(acc);

    return 0;
}
