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
#pragma once

#include <chrono>
#include <fstream>
#include "../DataGenerationScripts/randomUtils.h"
#include "../BasicHDCKernels/scalarKernels.h"
#include "../BasicHDCKernels/vectorialKernels.h"

using namespace std;

benchmarkResult hammingBenchmark_64(size_t nvec, size_t words);

benchmarkResult hammingBenchmark_32(size_t nvec, size_t words);

benchmarkResult hammingBenchmark_16(size_t nvec, size_t words);

benchmarkResult hammingBenchmark_8(size_t nvec, size_t words);
