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
#include "randomUtils.h"

int main(){
    size_t words = 8;
    size_t alignment = 64;
    size_t total_bytes = words * sizeof(hdc_word_t);

    size_t alloc_size = ((total_bytes + (alignment - 1)) / alignment) * alignment;

    hdc_word_t *x = (hdc_word_t*)aligned_alloc(alignment, alloc_size);
    hdc_word_t *y = (hdc_word_t*)aligned_alloc(alignment, alloc_size);
    hdc_word_t *z = (hdc_word_t*)aligned_alloc(alignment, alloc_size);    

    randomize_hdc_vector(x, words);
    randomize_hdc_vector(y, words);

    hdc_bind(x, y, z, words);
    for (size_t i = 0; i < words; i++) {
        printf("random x[%lu]: %lu | y[%lu]: %lu -> z[%lu]: %lu\n", 
               i, x[i], i, y[i], i, z[i]);
    }

    free(x);
    free(y);
    free(z);   

    return 0;
}