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

python3 make_graph.py bind 8 --out images/bind_8.png --no-show --mode small-multiples
python3 make_graph.py bind 16 --out images/bind_16.png --no-show --mode small-multiples
python3 make_graph.py bind 32 --out images/bind_32.png --no-show --mode small-multiples
python3 make_graph.py bind 64 --out images/bind_64.png --no-show --mode small-multiples

python3 make_graph.py hamming 8 --out images/hamming_8.png --no-show --mode small-multiples
python3 make_graph.py hamming 16 --out images/hamming_16.png --no-show --mode small-multiples
python3 make_graph.py hamming 32 --out images/hamming_32.png --no-show --mode small-multiples
python3 make_graph.py hamming 64 --out images/hamming_64.png --no-show --mode small-multiples

python3 make_graph.py query 8 --out images/query_8.png --no-show --mode small-multiples
python3 make_graph.py query 16 --out images/query_16.png --no-show --mode small-multiples
python3 make_graph.py query 32 --out images/query_32.png --no-show --mode small-multiples
python3 make_graph.py query 64 --out images/query_64.png --no-show --mode small-multiples
