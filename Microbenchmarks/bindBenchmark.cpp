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
#include <chrono>
#include <fstream>
#include "randomUtils.h"
#include "scalarKernels.h"
#include "vectorialKernels.h"

using namespace std;

const size_t ALIGNMENT = 64; 

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <nvec> <words>\n", argv[0]);
        return 1;
    }

    size_t nvec = stoull(argv[1]);
    size_t words = stoull(argv[2]);

    size_t matrix_words = nvec * words;
    size_t matrix_alloc_size = ((matrix_words * sizeof(hdc_word_t) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;

    hdc_word_t* M = (hdc_word_t*)aligned_alloc(ALIGNMENT, matrix_alloc_size); 
    hdc_word_t* q = (hdc_word_t*)aligned_alloc(ALIGNMENT, ((words * sizeof(hdc_word_t) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT);
    hdc_word_t* Z = (hdc_word_t*)aligned_alloc(ALIGNMENT, matrix_alloc_size);

    printf("Create vectors\n");

    randomize_hdc_vector(M, matrix_words);
    randomize_hdc_vector(q, words);

    double bytes_read = (double)(matrix_words * sizeof(hdc_word_t)) + (double)(nvec * words * sizeof(hdc_word_t));
    double bytes_written = (double)(matrix_words * sizeof(hdc_word_t));
    double total_gigabytes = (bytes_read + bytes_written) / (1024.0 * 1024.0 * 1024.0);

    // Warmup
    printf("Warming up...\n");
    for (size_t i = 0; i < nvec; ++i) {
        hdc_bind(&M[i * words], q, &Z[i * words], words);
    }

    printf("Start vectorized\n");
    auto start = chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < nvec; ++i) {
        hdc_bind(&M[i * words], q, &Z[i * words], words);
    }
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    double seconds = duration.count();

    double gbs = (seconds > 0) ? (total_gigabytes / seconds) : 0.0;

    printf("Start scalar\n");
    auto sstart = chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < nvec; ++i) {
        scalar_bind(&M[i * words], q, &Z[i * words], words);
    }
    
    auto send = chrono::high_resolution_clock::now();
    chrono::duration<double> sduration = send - sstart;
    double sseconds = sduration.count();

    double sgbs = (sseconds > 0) ? (total_gigabytes / sseconds) : 0.0;

    double speedup = (gbs > 0) ? (sgbs / gbs) : 0.0;

    printf("----------------------------------------------------------------------------------------\n");
    printf("Vectorized results (Bind):\n");
    printf("%lu vects, %lu words, %f s, %f gb/s\n", nvec, words, seconds, gbs);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Scalar results (Bind):\n");
    printf("%lu vects, %lu words, %f s, %f gb/s\n", nvec, words, sseconds, sgbs);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Speedup: %f\n", speedup);
    printf("----------------------------------------------------------------------------------------\n");

    ofstream output_file("bind_benchmark_64.txt");

    if (output_file.is_open()) {
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Vectorized results (Bind):\n";
        output_file << nvec << " vects, " << words << " words, " << seconds << " s, " << gbs << " gb/s\n";
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Scalar results (Bind):\n";
        output_file << nvec << " vects, " << words << " words, " << sseconds << " s, " << sgbs << " gb/s\n";
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Speedup: " << speedup << "\n";
        output_file << "----------------------------------------------------------------------------------------\n";
        
        output_file.close();
    }

    free(M);
    free(q);
    free(Z);

    return 0;
}