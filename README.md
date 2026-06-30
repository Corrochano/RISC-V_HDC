# RISC-V_HDC
This repository focus on optimize hyperdimensional computing (HDC) for RISC-V architectures workloads using the vectorials operations of the RVV 1.0 standar.

It's part of Álvaro Corrochano's PhD Thesis in Computer Science Engenieering at Complutense University of Madrid (UCM).

<div style="text-align: center;">
  <img src="https://www.ucm.es/data/cont/docs/3-2016-07-21-Marca%20UCM%20logo%20negro.png" alt="UCM Logo" width="250"/>
</div>

The repository structure divide into different folders in order to assure a good and legible workspace in the next folders: </br>
- BasicHDCKernels: This folder contains both, vectorial and scalar kernels of the operations as well as the definitions needed to work.
- BasicTests: This folder contain basic tests to check that the kernels work succesfully.
- DataGenerationScrips: This folder contains code to generate random hypervectors and a little test to check the succesful behaviour.
- Microbenchmarks: This folder contains benchmarks that measure different metrics for each kernel in both versions (vectorial and scalar).
- Performance Comparatives: This folder contains files to automatize and execute all the benchmarks with the parameters that we want and save the results into txt files.

On the sections below, there are explaine some of the most relevant information about the repository.

# Author
Álvaro Corrochano López

## Kernels

### Bind

### Hamming Distance

### Query

## Performance Comparatives Usage

## License

This project is licensed under the Apache License, Version 2.0 (the "License"). You may not use this file except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

### Copyright Notice

Copyright 2026 Álvaro Corrochano López
