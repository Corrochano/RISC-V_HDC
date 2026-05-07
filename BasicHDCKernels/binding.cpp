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
    size_t words = (500 + 63) / 64;
    hdc_word_t x = ((hdc_word_t)rand() << words) | rand();
    hdc_word_t y = ((hdc_word_t)rand() << words) | rand();
    hdc_word_t z;
    size_t vl = get_rvv_vl;

    hdc_bind_scalar(&x,&y,&z,vl);

    printf("Result: %llu\n", z);

    return 0;
}