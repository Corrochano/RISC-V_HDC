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

### Bind

### Hamming Distance

### Query

## Performance Comparatives Usage

## License

This project is licensed under the Apache License, Version 2.0 (the "License"). You may not use this file except in compliance with the License. You may obtain a copy of the License at:

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

### Copyright Notice

Copyright 2026 Álvaro Corrochano López
