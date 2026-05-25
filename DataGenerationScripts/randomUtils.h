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
#include <vector>
#include <ctime>

#include "typeDefine.h"

void randomize_hdc_vector(hdc_word_t* vec, size_t words) {
    for (size_t i = 0; i < words; i++) {
        vec[i] = ((hdc_word_t)rand() << 32) | rand();
    }
}