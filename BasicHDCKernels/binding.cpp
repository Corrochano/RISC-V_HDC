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

int main(int argc, char *argv[]){
    size_t words = (atoi(argv[1]) + 63) / 64;
    hdc_word_t *x = (hdc_word_t*)aligned_alloc(64, words * sizeof(hdc_word_t));
    hdc_word_t *y = (hdc_word_t*)aligned_alloc(64, words * sizeof(hdc_word_t));
    hdc_word_t *z = (hdc_word_t*)aligned_alloc(64, words * sizeof(hdc_word_t));

    size_t vl = get_rvv_vl();

    x[0] = ((hdc_word_t)rand() << 8) | rand();
    y[0] = ((hdc_word_t)rand() << 8) | rand(); 

    hdc_bind(x,y,z,vl);

    printf("Result: %lu\n", *z);

    return 0;
}