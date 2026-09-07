# RISC-V_HDC

This repository focuses on optimizing hyperdimensional computing (HDC) workloads for RISC-V architectures using the vector operations of the RVV 1.0 standard.

It is part of Álvaro Corrochano's PhD thesis in Computer Science Engineering at the Complutense University of Madrid (UCM).

<div style="text-align: center;">
  <img src="https://www.ucm.es/data/cont/docs/3-2016-07-21-Marca%20UCM%20logo%20negro.png" alt="UCM Logo" width="250"/>
</div>

The repository is structured into the following directories to ensure a clean and readable workspace:

- **BasicHDCKernels**: This folder contains both vector and scalar kernels for the operations, as well as the definitions needed for them to work.
- **BasicTests**: This folder contains basic tests to verify that the kernels work successfully.
- **DataGenerationScripts**: This folder contains code to generate random hypervectors and a brief test to verify successful behavior.
- **Microbenchmarks**: This folder contains benchmarks that measure different metrics for each kernel in both versions (vector and scalar).
- **Performance Comparatives**: This folder contains files to automate the execution of all benchmarks with custom parameters and save the results into `.txt` files.

The sections below explain some of the most relevant information about the repository.

## Table of Contents
- [Author](#author)
- [Kernels](#kernels)
  - [Bind](#bind)
  - [Hamming Distance](#hamming-distance)
  - [Query](#query)
- [Performance Comparatives Usage](#performance-comparatives-usage)
- [License](#license)
  - [Copyright Notice](#copyright-notice)

# Author
Álvaro Corrochano López

## Kernels
This repository provides highly optimized implementations of core HDC operations tailored for RISC-V. Each kernel includes a standard **scalar version** (sequential C code) and a **vectorial version** written using RVV 1.0 assembly/intrinsics to leverage hardware parallelism.

### Bind
The Binding operation combines two hypervectors to create a new, orthogonal hypervector that preserves the properties of both inputs. 
* **Implementation:** For binary hypervectors, this is executed as an element-wise **XOR** operation. 
* **RVV 1.0 Optimization:** Uses vector load/store operations (`vle32.v`/`vse32.v`) and vector logical XOR (`vxor.vv`) to process multiple dimensions per clock cycle, scaling dynamically based on the hardware's Vector Length (`VLEN`).

### Hamming Distance
The Hamming Distance evaluates the dissimilarity between two hypervectors by counting the number of positions at which the corresponding bits are different.
* **Implementation:** It requires performing an element-wise XOR between two hypervectors followed by a population count (counting the number of set bits).
* **RVV 1.0 Optimization:** Leverages vector bitwise operations and efficient vector reduction instructions to accumulate bit counts in parallel across large hypervector dimensions (e.g., $D = 10,000$).

### Query
The Query (or Search) operation matches a target hypervector against a trained associative memory (codebook) to identify the closest class or item.
* **Implementation:** It iterates through the associative memory, computes the Hamming Distance between the query vector and every stored class vector, and extracts the minimum distance (or maximum similarity).
* **RVV 1.0 Optimization:** Unrolls and vectorizes the distance-checking loop, maximizing data reuse within the vector registers to eliminate pipeline stalls during large-scale lookups.

## Performance Comparatives Usage

## License

This project is licensed under the Apache License, Version 2.0 (the "License"). You may not use this file except in compliance with the License. You may obtain a copy of the License at:

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

### Copyright Notice

Copyright 2026 Álvaro Corrochano López
