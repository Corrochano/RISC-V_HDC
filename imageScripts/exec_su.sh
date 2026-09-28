#!/bin/bash

# Copyright 2026 Álvaro Corrochano López
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# Launch the compiled `comparation` program across multiple combinations
# of `nvec` and `words` (width) parameters.

python3 make_speedup_graph.py bind 8 --out images/speedup/bind_8_speedup.png --no-show
python3 make_speedup_graph.py bind 16 --out images/speedup/bind_16_speedup.png --no-show
python3 make_speedup_graph.py bind 32 --out images/speedup/bind_32_speedup.png --no-show
python3 make_speedup_graph.py bind 64 --out images/speedup/bind_64_speedup.png --no-show

python3 make_speedup_graph.py hamming 8 --out images/speedup/hamming_8_speedup.png --no-show
python3 make_speedup_graph.py hamming 16 --out images/speedup/hamming_16_speedup.png --no-show
python3 make_speedup_graph.py hamming 32 --out images/speedup/hamming_32_speedup.png --no-show
python3 make_speedup_graph.py hamming 64 --out images/speedup/hamming_64_speedup.png --no-show

python3 make_speedup_graph.py query 8 --out images/speedup/query_8_speedup.png --no-show
python3 make_speedup_graph.py query 16 --out images/speedup/query_16_speedup.png --no-show
python3 make_speedup_graph.py query 32 --out images/speedup/query_32_speedup.png --no-show
python3 make_speedup_graph.py query 64 --out images/speedup/query_64_speedup.png --no-show