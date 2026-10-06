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
#include "bindBenchmark.h"

using namespace std;

template <typename WordT>
benchmarkResult run_binding_benchmark(
    size_t nvec,
    size_t words,
    const char *output_path,
    void (*vector_bind)(const WordT *, const WordT *, WordT *, size_t),
    void (*scalar_bind)(const WordT *, const WordT *, WordT *, size_t))
{
    benchmarkResult results;
    const size_t matrix_words = nvec * words;
    const size_t matrix_bytes = matrix_words * sizeof(WordT);
    const size_t matrix_alloc_size = ((matrix_bytes + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;
    const size_t query_alloc_size = ((words * sizeof(WordT) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;

    WordT *M = static_cast<WordT *>(aligned_alloc(ALIGNMENT, matrix_alloc_size));
    WordT *q = static_cast<WordT *>(aligned_alloc(ALIGNMENT, query_alloc_size));
    WordT *Z = static_cast<WordT *>(aligned_alloc(ALIGNMENT, matrix_alloc_size));

    printf("Create vectors\n");
    randomize_hdc_vector(M, matrix_words);
    randomize_hdc_vector(q, words);

    const double bytes_read = 2.0 * matrix_words * sizeof(WordT);
    const double bytes_written = static_cast<double>(matrix_words * sizeof(WordT));
    const double total_gigabytes = (bytes_read + bytes_written) / (1024.0 * 1024.0 * 1024.0);

    printf("Warming up...\n");
    for (size_t i = 0; i < nvec; ++i) {
        vector_bind(&M[i * words], q, &Z[i * words], words);
    }

    printf("Start vectorized\n");
    auto start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < nvec; ++i) {
        vector_bind(&M[i * words], q, &Z[i * words], words);
    }
    auto end = chrono::high_resolution_clock::now();
    const double seconds = chrono::duration<double>(end - start).count();
    const double gbs = (seconds > 0) ? (total_gigabytes / seconds) : 0.0;

    printf("Start scalar\n");
    auto sstart = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < nvec; ++i) {
        scalar_bind(&M[i * words], q, &Z[i * words], words);
    }
    auto send = chrono::high_resolution_clock::now();
    const double sseconds = chrono::duration<double>(send - sstart).count();
    const double sgbs = (sseconds > 0) ? (total_gigabytes / sseconds) : 0.0;
    const double speedup = (seconds > 0) ? (sseconds / seconds) : 0.0;

    printf("----------------------------------------------------------------------------------------\n");
    printf("Vectorized results (Bind):\n");
    printf("%zu vects, %zu words, %f s, %f gb/s\n", nvec, words, seconds, gbs);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Scalar results (Bind):\n");
    printf("%zu vects, %zu words, %f s, %f gb/s\n", nvec, words, sseconds, sgbs);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Speedup: %f\n", speedup);
    printf("----------------------------------------------------------------------------------------\n");

    ofstream output_file(output_path, ios::app);
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
    }

    free(M);
    free(q);
    free(Z);

    results.seconds = seconds;
    results.gbs = gbs;
    results.sseconds = sseconds;
    results.sgbs = sgbs;
    return results;
}

#define DEFINE_BINDING_BENCHMARK(BITS, LMUL, WORD_TYPE) \
benchmarkResult bindingBenchmark_##BITS##_m##LMUL(size_t nvec, size_t words) { \
    return run_binding_benchmark<WORD_TYPE>( \
        nvec, words, "bind_benchmark_" #BITS "_m" #LMUL ".txt", \
        hdc_bind_m##LMUL, scalar_bind); \
}

////////////////////////////////////////////////////////////////////////////////////////
//                                      LMUL = 1                                      //
////////////////////////////////////////////////////////////////////////////////////////

DEFINE_BINDING_BENCHMARK(64, 1, hdc_word_t)

DEFINE_BINDING_BENCHMARK(32, 1, hdc_word_t_32)

DEFINE_BINDING_BENCHMARK(16, 1, hdc_word_t_16)

DEFINE_BINDING_BENCHMARK(8, 1, hdc_word_t_8)

////////////////////////////////////////////////////////////////////////////////////////
//                                      LMUL = 2                                      //
////////////////////////////////////////////////////////////////////////////////////////

DEFINE_BINDING_BENCHMARK(64, 2, hdc_word_t)

DEFINE_BINDING_BENCHMARK(32, 2, hdc_word_t_32)

DEFINE_BINDING_BENCHMARK(16, 2, hdc_word_t_16)

DEFINE_BINDING_BENCHMARK(8, 2, hdc_word_t_8)

////////////////////////////////////////////////////////////////////////////////////////
//                                      LMUL = 4                                      //
////////////////////////////////////////////////////////////////////////////////////////

DEFINE_BINDING_BENCHMARK(64, 4, hdc_word_t)

DEFINE_BINDING_BENCHMARK(32, 4, hdc_word_t_32)

DEFINE_BINDING_BENCHMARK(16, 4, hdc_word_t_16)

DEFINE_BINDING_BENCHMARK(8, 4, hdc_word_t_8)

////////////////////////////////////////////////////////////////////////////////////////
//                                      LMUL = 8                                      //
////////////////////////////////////////////////////////////////////////////////////////

DEFINE_BINDING_BENCHMARK(64, 8, hdc_word_t)

DEFINE_BINDING_BENCHMARK(32, 8, hdc_word_t_32)

DEFINE_BINDING_BENCHMARK(16, 8, hdc_word_t_16)

DEFINE_BINDING_BENCHMARK(8, 8, hdc_word_t_8)
