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

void scalar_bind(
    const hdc_word_t *x,
    const hdc_word_t *y,
    hdc_word_t *z,
    size_t words) 
{    
        for (size_t i = 0; i < words; ++i) {
        z[i] = x[i] ^ y[i];
    }
}

void scalar_hamming(
    const hdc_word_t *x,
    const hdc_word_t *y,
    hdc_score_t *acc,
    size_t words)
{

    for (size_t i = 0; i < words; ++i) {
        if ((x[i] ^ y[i]) != 0) {
            *acc++;
        }
    }
}

void scalar_query( 
    const hdc_word_t *M,
    const hdc_word_t *q,
    hdc_score_t *scores,
    size_t nvec,
    size_t words)
{
for (size_t i = 0; i < nvec; ++i) {
        scores[i] = 0; 
        scalar_hamming(&M[i * words], q, &scores[i], words);
    }
}
