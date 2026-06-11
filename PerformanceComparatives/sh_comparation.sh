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

EXE="./comparation"

if [[ ! -x "$EXE" ]]; then
  echo "Error: executable $EXE not found or not executable." >&2
  exit 2
fi

# Default parameter lists (edit as needed)
NVEC_LIST=(1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536)
WORDS_LIST=(8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536)

if [[ -n "${NVEC_LIST_ENV:-}" ]]; then
  IFS=',' read -r -a NVEC_LIST <<< "$NVEC_LIST_ENV"
fi
if [[ -n "${WORDS_LIST_ENV:-}" ]]; then
  IFS=',' read -r -a WORDS_LIST <<< "$WORDS_LIST_ENV"
fi

echo "Running comparation with combinations (nvec x words) ..."

for nvec in "${NVEC_LIST[@]}"; do
  for words in "${WORDS_LIST[@]}"; do
    "$EXE" "$nvec" "$words" || {
      echo "Command failed for nvec=$nvec words=$words" >&2
    }
  done
done

echo "Finished.""
