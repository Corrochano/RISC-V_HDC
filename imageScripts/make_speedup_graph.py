'''
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
'''

import argparse
from math import ceil
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

from make_graph import parse_benchmark


BASE_DIR = Path(__file__).resolve().parent


def collect_speedups(name, bits):
    """Return speedups against noOpt scalar, grouped by vector count."""
    sources = {
        'noOpt': BASE_DIR / 'noOpt' / f'{name}_benchmark_{bits}.txt',
        'o2Execution': BASE_DIR / 'o2Execution' / f'{name}_benchmark_{bits}.txt',
        'o3Execution': BASE_DIR / 'o3Execution' / f'{name}_benchmark_{bits}.txt',
    }
    parsed = {case: parse_benchmark(path) for case, path in sources.items()}
    baseline = parsed['noOpt']
    if not baseline:
        raise SystemExit('No noOpt benchmark data found for the requested files')

    grouped = {}
    for (vects, words), baseline_times in baseline.items():
        baseline_time = baseline_times.get('scalar')
        if baseline_time is None or baseline_time <= 0:
            continue

        speedups = {'noOpt scalar': 1.0}
        comparisons = [
            ('noOpt vectorial', 'noOpt', 'vectorized'),
            ('O2 scalar', 'o2Execution', 'scalar'),
            ('O2 vectorial', 'o2Execution', 'vectorized'),
            ('O3 scalar', 'o3Execution', 'scalar'),
            ('O3 vectorial', 'o3Execution', 'vectorized'),
        ]
        for label, case, result_kind in comparisons:
            execution_time = parsed[case].get((vects, words), {}).get(result_kind)
            speedups[label] = (
                baseline_time / execution_time
                if execution_time is not None and execution_time > 0
                else None
            )
        grouped.setdefault(vects, []).append((words, speedups))

    return grouped


def plot(name, bits, out=None, show=True):
    grouped = collect_speedups(name, bits)
    vects_sorted = sorted(grouped)
    if not vects_sorted:
        raise SystemExit('No valid benchmark data to plot')

    cols = min(4, len(vects_sorted))
    rows = ceil(len(vects_sorted) / cols)
    fig, axes = plt.subplots(rows, cols, figsize=(4 * cols, 3 * rows), sharey=False)
    axes_list = np.atleast_1d(axes).ravel()
    cases = [
        ('noOpt scalar', 'noOpt scalar (baseline)', 'black'),
        ('noOpt vectorial', 'noOpt vectorial', 'forestgreen'),
        ('O2 scalar', 'O2 scalar', 'royalblue'),
        ('O2 vectorial', 'O2 vectorial', 'deepskyblue'),
        ('O3 scalar', 'O3 scalar', 'purple'),
        ('O3 vectorial', 'O3 vectorial', 'darkorange'),
    ]

    for ax, vects in zip(axes_list, vects_sorted):
        entries = sorted(grouped[vects], key=lambda entry: entry[0])
        words = np.array([entry[0] for entry in entries], dtype=float)
        for case, label, color in cases:
            values = np.array([
                entry[1][case] if entry[1][case] is not None else np.nan
                for entry in entries
            ], dtype=float)
            valid = np.isfinite(values)
            if valid.any():
                ax.plot(words[valid], values[valid], marker='o', label=label, color=color)

        ax.set_xscale('log')
        ax.set_title(f'{vects} vects')
        ax.set_xlabel('Number of words')
        ax.set_ylabel('Speedup') # (noOpt scalar time / execution time)
        ax.axhline(1.0, color='gray', linestyle='--', linewidth=0.8, alpha=0.7)
        ax.grid(axis='y', alpha=0.2)

    for ax in axes_list[len(vects_sorted):]:
        ax.axis('off')

    handles, labels = axes_list[0].get_legend_handles_labels()
    if handles:
        fig.legend(handles, labels, loc='upper right')

    fig.suptitle(f'{name.capitalize()} speedup ({bits}-bit)')
    plt.tight_layout()
    if out:
        plt.savefig(out, dpi=200, bbox_inches='tight')
    if show:
        plt.show()
    else:
        plt.close(fig)


def main():
    parser = argparse.ArgumentParser(description='Plot speedups against noOpt scalar')
    parser.add_argument('name', choices=['bind', 'hamming', 'query'], help='benchmark name')
    parser.add_argument('bits', choices=['8', '16', '32', '64'], help='data width')
    parser.add_argument('--out', '-o', help='output image file')
    parser.add_argument('--no-show', action='store_true', help="don't open an interactive window")
    args = parser.parse_args()
    plot(args.name, args.bits, out=args.out, show=not args.no_show)


if __name__ == '__main__':
    main()