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

#include "../Microbenchmarks/bindBenchmark.h"
#include "../Microbenchmarks/hammingBenchmark.h"
#include "../Microbenchmarks/queryBenchmark.h"

using namespace std;

void bindComparation(size_t nvec, size_t words){
    benchmarkResult bind_64 = bindingBenchmark_64(nvec, words);
    benchmarkResult bind_32 = bindingBenchmark_32(nvec, words);
    benchmarkResult bind_16 = bindingBenchmark_16(nvec, words);
    benchmarkResult bind_8 = bindingBenchmark_8(nvec, words);

    double speedup_32 = (bind_64.gbs > 0) ? (bind_32.gbs / bind_64.gbs) : 0.0;
    double speedup_16 = (bind_64.gbs > 0) ? (bind_16.gbs / bind_64.gbs) : 0.0;
    double speedup_8 = (bind_64.gbs > 0) ? (bind_8.gbs / bind_64.gbs) : 0.0;

    printf("Speedup of Bind 32-bit vs 64-bit: %f\n", speedup_32);
    printf("Speedup of Bind 16-bit vs 64-bit: %f\n", speedup_16);
    printf("Speedup of Bind 8-bit vs 64-bit: %f\n", speedup_8);

    ofstream output_file("bind_speedups.txt");

    if (output_file.is_open()) {
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Speedup of Bind 32-bit vs 64-bit: %f\n", speedup_32;
        output_file << "Speedup of Bind 16-bit vs 64-bit: %f\n", speedup_16;
        output_file << "Speedup of Bind 8-bit vs 64-bit: %f\n", speedup_8;
        output_file << "----------------------------------------------------------------------------------------\n";
        
        output_file.close();
    }
    
}

void hammerComparation(size_t nvec, size_t words){
    benchmarkResult hammer_64 = hammingBenchmark_64(nvec, words);
    benchmarkResult hammer_32 = hammingBenchmark_32(nvec, words);
    benchmarkResult hammer_16 = hammingBenchmark_16(nvec, words);
    benchmarkResult hammer_8 = hammingBenchmark_8(nvec, words);

    double speedup_32 = (hammer_64.gbs > 0) ? (hammer_32.gbs / hammer_64.gbs) : 0.0;
    double speedup_16 = (hammer_64.gbs > 0) ? (hammer_16.gbs / hammer_64.gbs) : 0.0;
    double speedup_8 = (hammer_64.gbs > 0) ? (hammer_8.gbs / hammer_64.gbs) : 0.0;

    printf("Speedup of Hammer 32-bit vs 64-bit: %f\n", speedup_32);
    printf("Speedup of Hammer 16-bit vs 64-bit: %f\n", speedup_16);
    printf("Speedup of Hammer 8-bit vs 64-bit: %f\n", speedup_8);

    ofstream output_file("hammer_speedups.txt");

    if (output_file.is_open()) {
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Speedup of Hammer 32-bit vs 64-bit: %f\n", speedup_32;
        output_file << "Speedup of Hammer 16-bit vs 64-bit: %f\n", speedup_16;
        output_file << "Speedup of Hammer 8-bit vs 64-bit: %f\n", speedup_8;
        output_file << "----------------------------------------------------------------------------------------\n";
        
        output_file.close();
    }    
}

void queryComparation(size_t nvec, size_t words){
    benchmarkResult query_64 = queryBenchmark_64(nvec, words);
    benchmarkResult query_32 = queryBenchmark_32(nvec, words);
    benchmarkResult query_16 = queryBenchmark_16(nvec, words);
    benchmarkResult query_8 = queryBenchmark_8(nvec, words);

    double speedup_32 = (query_64.gbs > 0) ? (query_32.gbs / query_64.gbs) : 0.0;
    double speedup_16 = (query_64.gbs > 0) ? (query_16.gbs / query_64.gbs) : 0.0;
    double speedup_8 = (query_64.gbs > 0) ? (query_8.gbs / query_64.gbs) : 0.0;

    printf("Speedup of Query 32-bit vs 64-bit: %f\n", speedup_32);
    printf("Speedup of Query 16-bit vs 64-bit: %f\n", speedup_16);
    printf("Speedup of Query 8-bit vs 64-bit: %f\n", speedup_8);

    ofstream output_file("query_speedups.txt");

    if (output_file.is_open()) {
        output_file << "----------------------------------------------------------------------------------------\n";
        output_file << "Speedup of Query 32-bit vs 64-bit: %f\n", speedup_32;
        output_file << "Speedup of Query 16-bit vs 64-bit: %f\n", speedup_16;
        output_file << "Speedup of Query 8-bit vs 64-bit: %f\n", speedup_8;
        output_file << "----------------------------------------------------------------------------------------\n";
        
        output_file.close();
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <nvec> <words>\n", argv[0]);
        return 1;
    }

    size_t nvec = stoull(argv[1]);
    size_t words = stoull(argv[2]);   

    printf("Comparing Bind benchmarks...\n");
    bindComparation(nvec, words);

    printf("\nComparing Hammer benchmarks...\n");
    hammerComparation(nvec, words);

    printf("\nComparing Query benchmarks...\n");
    queryComparation(nvec, words);

    return 0;
}
