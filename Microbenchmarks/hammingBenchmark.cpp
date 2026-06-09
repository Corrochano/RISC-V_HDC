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

int hammingBenchmark_64(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <nvec> <words>\n", argv[0]);
        return 1;
    }

    size_t nvec = stoull(argv[1]);
    size_t words = stoull(argv[2]);

    size_t alloc_size = ((words * sizeof(hdc_word_t) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;
    size_t matrix_words = nvec * words;
    size_t matrix_alloc_size = ((matrix_words * sizeof(hdc_word_t) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;

    hdc_word_t* M = (hdc_word_t*)aligned_alloc(ALIGNMENT, matrix_alloc_size); 
    hdc_word_t* q = (hdc_word_t*)aligned_alloc(ALIGNMENT, alloc_size);
    hdc_score_t* scores = (hdc_score_t*)aligned_alloc(ALIGNMENT, nvec * sizeof(hdc_score_t));

    printf("Create vectors\n");

    randomize_hdc_vector(M, matrix_words);
    randomize_hdc_vector(q, words);

    double bytes_read = (double)(nvec * words * sizeof(hdc_word_t)) + (double)(nvec * words * sizeof(hdc_word_t));
    double bytes_written = (double)(nvec * sizeof(hdc_score_t));
    double total_gigabytes = (bytes_read + bytes_written) / (1024.0 * 1024.0 * 1024.0);

    printf("Warming up...\n");
    for (size_t i = 0; i < nvec; ++i) {
        scores[i] = 0; // Explicitly reset accumulator to prevent garbage values
        hdc_hamming(&M[i * words], q, &scores[i], words, ALIGNMENT, alloc_size);
    }

    printf("Start vectorized\n");
    auto start = chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < nvec; ++i) {
        scores[i] = 0;
        hdc_hamming(&M[i * words], q, &scores[i], words, ALIGNMENT, alloc_size);
    }
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    double seconds = duration.count();

    double gbs = (seconds > 0) ? (total_gigabytes / seconds) : 0.0;

    printf("Start scalar\n");
    auto sstart = chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < nvec; ++i) {
        scores[i] = 0;
        scalar_hamming(&M[i * words], q, &scores[i], words);
    }
    
    auto send = chrono::high_resolution_clock::now();
    chrono::duration<double> sduration = send - sstart;
    double sseconds = sduration.count();

    double sgbs = (sseconds > 0) ? (total_gigabytes / sseconds) : 0.0;

    double speedup = (gbs > 0) ? (sgbs / gbs) : 0.0;

    printf("----------------------------------------------------------------------------------------\n");
    printf("Vectorized results (Hamming):\n");
    printf("%lu vects, %lu words, %f s, %f gb/s\n", nvec, words, seconds, gbs);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Scalar results (Hamming):\n");
    printf("%lu vects, %lu words, %f s, %f gb/s\n", nvec, words, sseconds, sgbs);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Speedup: %f\n", speedup);
    printf("----------------------------------------------------------------------------------------\n");

    ofstream output_file("hamming_benchmark_64.txt");

    if (output_file.is_open()) {
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Vectorized results (Hamming):\n";
        output_file << nvec << " vects, " << words << " words, " << seconds << " s, " << gbs << " gb/s\n";
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Scalar results (Hamming):\n";
        output_file << nvec << " vects, " << words << " words, " << sseconds << " s, " << sgbs << " gb/s\n";
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Speedup: " << speedup << "\n";
        output_file << "----------------------------------------------------------------------------------------\n";
        
        output_file.close();
    }

    free(M);
    free(q);
    free(scores);

    return 0;
}

int hammingBenchmark_32(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <nvec> <words>\n", argv[0]);
        return 1;
    }

    size_t nvec = stoull(argv[1]);
    size_t words = stoull(argv[2]);

    size_t alloc_size = ((words * sizeof(hdc_word_t_32) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;
    size_t matrix_words = nvec * words;
    size_t matrix_alloc_size = ((matrix_words * sizeof(hdc_word_t_32) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;

    hdc_word_t_32* M = (hdc_word_t_32*)aligned_alloc(ALIGNMENT, matrix_alloc_size); 
    hdc_word_t_32* q = (hdc_word_t_32*)aligned_alloc(ALIGNMENT, alloc_size);
    hdc_score_t_32* scores = (hdc_score_t_32*)aligned_alloc(ALIGNMENT, nvec * sizeof(hdc_score_t_32));

    printf("Create vectors\n");

    randomize_hdc_vector(M, matrix_words);
    randomize_hdc_vector(q, words);

    double bytes_read = (double)(nvec * words * sizeof(hdc_word_t_32)) + (double)(nvec * words * sizeof(hdc_word_t_32));
    double bytes_written = (double)(nvec * sizeof(hdc_score_t_32));
    double total_gigabytes = (bytes_read + bytes_written) / (1024.0 * 1024.0 * 1024.0);

    printf("Warming up...\n");
    for (size_t i = 0; i < nvec; ++i) {
        scores[i] = 0; // Explicitly reset accumulator to prevent garbage values
        hdc_hamming(&M[i * words], q, &scores[i], words, ALIGNMENT, alloc_size);
    }

    printf("Start vectorized\n");
    auto start = chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < nvec; ++i) {
        scores[i] = 0;
        hdc_hamming(&M[i * words], q, &scores[i], words, ALIGNMENT, alloc_size);
    }
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    double seconds = duration.count();

    double gbs = (seconds > 0) ? (total_gigabytes / seconds) : 0.0;

    printf("Start scalar\n");
    auto sstart = chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < nvec; ++i) {
        scores[i] = 0;
        scalar_hamming(&M[i * words], q, &scores[i], words);
    }
    
    auto send = chrono::high_resolution_clock::now();
    chrono::duration<double> sduration = send - sstart;
    double sseconds = sduration.count();

    double sgbs = (sseconds > 0) ? (total_gigabytes / sseconds) : 0.0;

    double speedup = (gbs > 0) ? (sgbs / gbs) : 0.0;

    printf("----------------------------------------------------------------------------------------\n");
    printf("Vectorized results (Hamming):\n");
    printf("%lu vects, %lu words, %f s, %f gb/s\n", nvec, words, seconds, gbs);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Scalar results (Hamming):\n");
    printf("%lu vects, %lu words, %f s, %f gb/s\n", nvec, words, sseconds, sgbs);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Speedup: %f\n", speedup);
    printf("----------------------------------------------------------------------------------------\n");

    ofstream output_file("hamming_benchmark_64.txt");

    if (output_file.is_open()) {
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Vectorized results (Hamming):\n";
        output_file << nvec << " vects, " << words << " words, " << seconds << " s, " << gbs << " gb/s\n";
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Scalar results (Hamming):\n";
        output_file << nvec << " vects, " << words << " words, " << sseconds << " s, " << sgbs << " gb/s\n";
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Speedup: " << speedup << "\n";
        output_file << "----------------------------------------------------------------------------------------\n";
        
        output_file.close();
    }

    free(M);
    free(q);
    free(scores);

    return 0;
}

int hammingBenchmark_16(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <nvec> <words>\n", argv[0]);
        return 1;
    }

    size_t nvec = stoull(argv[1]);
    size_t words = stoull(argv[2]);

    size_t alloc_size = ((words * sizeof(hdc_word_t_16) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;
    size_t matrix_words = nvec * words;
    size_t matrix_alloc_size = ((matrix_words * sizeof(hdc_word_t_16) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;

    hdc_word_t_16* M = (hdc_word_t_16*)aligned_alloc(ALIGNMENT, matrix_alloc_size); 
    hdc_word_t_16* q = (hdc_word_t_16*)aligned_alloc(ALIGNMENT, alloc_size);
    hdc_score_t_16* scores = (hdc_score_t_16*)aligned_alloc(ALIGNMENT, nvec * sizeof(hdc_score_t_16));

    printf("Create vectors\n");

    randomize_hdc_vector(M, matrix_words);
    randomize_hdc_vector(q, words);

    double bytes_read = (double)(nvec * words * sizeof(hdc_word_t_16)) + (double)(nvec * words * sizeof(hdc_word_t_16));
    double bytes_written = (double)(nvec * sizeof(hdc_score_t_16));
    double total_gigabytes = (bytes_read + bytes_written) / (1024.0 * 1024.0 * 1024.0);

    printf("Warming up...\n");
    for (size_t i = 0; i < nvec; ++i) {
        scores[i] = 0; // Explicitly reset accumulator to prevent garbage values
        hdc_hamming(&M[i * words], q, &scores[i], words, ALIGNMENT, alloc_size);
    }

    printf("Start vectorized\n");
    auto start = chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < nvec; ++i) {
        scores[i] = 0;
        hdc_hamming(&M[i * words], q, &scores[i], words, ALIGNMENT, alloc_size);
    }
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    double seconds = duration.count();

    double gbs = (seconds > 0) ? (total_gigabytes / seconds) : 0.0;

    printf("Start scalar\n");
    auto sstart = chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < nvec; ++i) {
        scores[i] = 0;
        scalar_hamming(&M[i * words], q, &scores[i], words);
    }
    
    auto send = chrono::high_resolution_clock::now();
    chrono::duration<double> sduration = send - sstart;
    double sseconds = sduration.count();

    double sgbs = (sseconds > 0) ? (total_gigabytes / sseconds) : 0.0;

    double speedup = (gbs > 0) ? (sgbs / gbs) : 0.0;

    printf("----------------------------------------------------------------------------------------\n");
    printf("Vectorized results (Hamming):\n");
    printf("%lu vects, %lu words, %f s, %f gb/s\n", nvec, words, seconds, gbs);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Scalar results (Hamming):\n");
    printf("%lu vects, %lu words, %f s, %f gb/s\n", nvec, words, sseconds, sgbs);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Speedup: %f\n", speedup);
    printf("----------------------------------------------------------------------------------------\n");

    ofstream output_file("hamming_benchmark_64.txt");

    if (output_file.is_open()) {
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Vectorized results (Hamming):\n";
        output_file << nvec << " vects, " << words << " words, " << seconds << " s, " << gbs << " gb/s\n";
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Scalar results (Hamming):\n";
        output_file << nvec << " vects, " << words << " words, " << sseconds << " s, " << sgbs << " gb/s\n";
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Speedup: " << speedup << "\n";
        output_file << "----------------------------------------------------------------------------------------\n";
        
        output_file.close();
    }

    free(M);
    free(q);
    free(scores);

    return 0;
}

int hammingBenchmark_8(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <nvec> <words>\n", argv[0]);
        return 1;
    }

    size_t nvec = stoull(argv[1]);
    size_t words = stoull(argv[2]);

    size_t alloc_size = ((words * sizeof(hdc_word_t_8) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;
    size_t matrix_words = nvec * words;
    size_t matrix_alloc_size = ((matrix_words * sizeof(hdc_word_t_8) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;

    hdc_word_t_8* M = (hdc_word_t_8*)aligned_alloc(ALIGNMENT, matrix_alloc_size); 
    hdc_word_t_8* q = (hdc_word_t_8*)aligned_alloc(ALIGNMENT, alloc_size);
    hdc_score_t_8* scores = (hdc_score_t_8*)aligned_alloc(ALIGNMENT, nvec * sizeof(hdc_score_t_8));

    printf("Create vectors\n");

    randomize_hdc_vector(M, matrix_words);
    randomize_hdc_vector(q, words);

    double bytes_read = (double)(nvec * words * sizeof(hdc_word_t_8)) + (double)(nvec * words * sizeof(hdc_word_t_8));
    double bytes_written = (double)(nvec * sizeof(hdc_score_t_8));
    double total_gigabytes = (bytes_read + bytes_written) / (1024.0 * 1024.0 * 1024.0);

    printf("Warming up...\n");
    for (size_t i = 0; i < nvec; ++i) {
        scores[i] = 0; // Explicitly reset accumulator to prevent garbage values
        hdc_hamming(&M[i * words], q, &scores[i], words, ALIGNMENT, alloc_size);
    }

    printf("Start vectorized\n");
    auto start = chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < nvec; ++i) {
        scores[i] = 0;
        hdc_hamming(&M[i * words], q, &scores[i], words, ALIGNMENT, alloc_size);
    }
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    double seconds = duration.count();

    double gbs = (seconds > 0) ? (total_gigabytes / seconds) : 0.0;

    printf("Start scalar\n");
    auto sstart = chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < nvec; ++i) {
        scores[i] = 0;
        scalar_hamming(&M[i * words], q, &scores[i], words);
    }
    
    auto send = chrono::high_resolution_clock::now();
    chrono::duration<double> sduration = send - sstart;
    double sseconds = sduration.count();

    double sgbs = (sseconds > 0) ? (total_gigabytes / sseconds) : 0.0;

    double speedup = (gbs > 0) ? (sgbs / gbs) : 0.0;

    printf("----------------------------------------------------------------------------------------\n");
    printf("Vectorized results (Hamming):\n");
    printf("%lu vects, %lu words, %f s, %f gb/s\n", nvec, words, seconds, gbs);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Scalar results (Hamming):\n");
    printf("%lu vects, %lu words, %f s, %f gb/s\n", nvec, words, sseconds, sgbs);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Speedup: %f\n", speedup);
    printf("----------------------------------------------------------------------------------------\n");

    ofstream output_file("hamming_benchmark_64.txt");

    if (output_file.is_open()) {
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Vectorized results (Hamming):\n";
        output_file << nvec << " vects, " << words << " words, " << seconds << " s, " << gbs << " gb/s\n";
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Scalar results (Hamming):\n";
        output_file << nvec << " vects, " << words << " words, " << sseconds << " s, " << sgbs << " gb/s\n";
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Speedup: " << speedup << "\n";
        output_file << "----------------------------------------------------------------------------------------\n";
        
        output_file.close();
    }

    free(M);
    free(q);
    free(scores);

    return 0;
}
