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
#include <chrono>
#include <vector>
#include <string>
#include <cstring>

#include "typeDefine.h"
#include "scalarKernels.h"
#include "randomUtils.h"

using namespace std;

const size_t ALIGNMENT = 64; 

int main(int argc, char* argv[]) {
    size_t nvec = std::stoull(argv[1]);
    size_t words = std::stoull(argv[2]);

    srand(time(NULL));

    size_t alloc_size = ((words * sizeof(hdc_word_t) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;

    size_t matrix_words = nvec * words;
    size_t matrix_alloc_size = ((matrix_words * sizeof(hdc_word_t) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;

    hdc_word_t* M = (hdc_word_t*)aligned_alloc(ALIGNMENT, matrix_alloc_size);
    hdc_word_t* q = (hdc_word_t*)aligned_alloc(ALIGNMENT, alloc_size);
    hdc_score_t* scores = (hdc_score_t*)aligned_alloc(ALIGNMENT, nvec * sizeof(hdc_score_t));

    if (!M || !q || !scores) {
        std::cerr << "Error: Fallo en la asignación de memoria alineada.\n";
        return 1;
    }

    randomize_hdc_vector(M, matrix_words);
    randomize_hdc_vector(q, words);

    double bytes_read = (double)(nvec * words * sizeof(hdc_word_t)) + (double)(nvec * words * sizeof(hdc_word_t));
    double bytes_written = (double)(nvec * sizeof(hdc_score_t));
    double total_gigabytes = (bytes_read + bytes_written) / (1024.0 * 1024.0 * 1024.0);

    // Warmup
    hdc_query(M, q, scores, nvec, words, ALIGNMENT, alloc_size);

    auto start = std::chrono::high_resolution_clock::now();
    
    hdc_query(M, q, scores, nvec, words, ALIGNMENT, alloc_size);
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    double seconds = duration.count();

    double gbs = (seconds > 0) ? (total_gigabytes / seconds) : 0.0;


    std::cout << "," << nvec << "," << words << "," << seconds << "," << gbs << "\n";

    // Liberar memoria
    free(M);
    free(q);
    free(scores);

    return 0;
}