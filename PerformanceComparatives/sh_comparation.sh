#!/usr/bin/env bash
# Launch the compiled `comparation` program across multiple combinations
# of `nvec` and `words` (width) parameters.

set -euo pipefail

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
    "$EXE" --nvec "$nvec" --words "$words" > 2>&1 || {
      echo "Command failed for nvec=$nvec words=$words" >&2
    }
  done
done

echo "Finished.""
