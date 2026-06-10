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

import "bindBenchmark.cpp";
import "hammerBenchmark.cpp";
import "queryBenchmark.cpp";

using namespace std;

void bindComparation(){
    benchmarkResult bind_64 = bindingBenchmark_64(3, (char*[]){"bindBenchmark", "1000000", "128"});
    benchmarkResult bind_32 = bindingBenchmark_32(3, (char*[]){"bindBenchmark", "1000000", "128"});
    benchmarkResult bind_16 = bindingBenchmark_16(3, (char*[]){"bindBenchmark", "1000000", "128"});
    benchmarkResult bind_8 = bindingBenchmark_8(3, (char*[]){"bindBenchmark", "1000000", "128"});

    double speedup_32 = (bind_64.gbs > 0) ? (bind_32.gbs / bind_64.gbs) : 0.0;
    double speedup_16 = (bind_64.gbs > 0) ? (bind_16.gbs / bind_64.gbs) : 0.0;
    double speedup_8 = (bind_64.gbs > 0) ? (bind_8.gbs / bind_64.gbs) : 0.0;

    printf("Speedup of Bind 32-bit vs 64-bit: %f\n", speedup_32);
    printf("Speedup of Bind 16-bit vs 64-bit: %f\n", speedup_16);
    printf("Speedup of Bind 8-bit vs 64-bit: %f\n", speedup_8);
    
}

void hammerComparation(){
    benchmarkResult hammer_64 = hammerBenchmark_64(3, (char*[]){"hammerBenchmark", "1000000", "128"});
    benchmarkResult hammer_32 = hammerBenchmark_32(3, (char*[]){"hammerBenchmark", "1000000", "128"});
    benchmarkResult hammer_16 = hammerBenchmark_16(3, (char*[]){"hammerBenchmark", "1000000", "128"});
    benchmarkResult hammer_8 = hammerBenchmark_8(3, (char*[]){"hammerBenchmark", "1000000", "128"});

    double speedup_32 = (hammer_64.gbs > 0) ? (hammer_32.gbs / hammer_64.gbs) : 0.0;
    double speedup_16 = (hammer_64.gbs > 0) ? (hammer_16.gbs / hammer_64.gbs) : 0.0;
    double speedup_8 = (hammer_64.gbs > 0) ? (hammer_8.gbs / hammer_64.gbs) : 0.0;

    printf("Speedup of Hammer 32-bit vs 64-bit: %f\n", speedup_32);
    printf("Speedup of Hammer 16-bit vs 64-bit: %f\n", speedup_16);
    printf("Speedup of Hammer 8-bit vs 64-bit: %f\n", speedup_8);
}

void queryComparation(){
    benchmarkResult query_64 = queryBenchmark_64(3, (char*[]){"queryBenchmark", "1000000", "128"});
    benchmarkResult query_32 = queryBenchmark_32(3, (char*[]){"queryBenchmark", "1000000", "128"});
    benchmarkResult query_16 = queryBenchmark_16(3, (char*[]){"queryBenchmark", "1000000", "128"});
    benchmarkResult query_8 = queryBenchmark_8(3, (char*[]){"queryBenchmark", "1000000", "128"});

    double speedup_32 = (query_64.gbs > 0) ? (query_32.gbs / query_64.gbs) : 0.0;
    double speedup_16 = (query_64.gbs > 0) ? (query_16.gbs / query_64.gbs) : 0.0;
    double speedup_8 = (query_64.gbs > 0) ? (query_8.gbs / query_64.gbs) : 0.0;

    printf("Speedup of Query 32-bit vs 64-bit: %f\n", speedup_32);
    printf("Speedup of Query 16-bit vs 64-bit: %f\n", speedup_16);
    printf("Speedup of Query 8-bit vs 64-bit: %f\n", speedup_8);
}

void main() {
    printf("Comparing Bind benchmarks...\n");
    bindComparation();

    printf("\nComparing Hammer benchmarks...\n");
    hammerComparation();

    printf("\nComparing Query benchmarks...\n");
    queryComparation();
}
