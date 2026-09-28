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

import re
import argparse
import matplotlib.pyplot as plt
from collections import defaultdict
import numpy as np

LINE_RE = re.compile(r"(?P<vects>\d+) vects, (?P<words>\d+) words, (?P<time>[0-9.eE+-]+) s, (?P<gb>[0-9.eE+-]+) gb/s")

def parse_benchmark(path):
    """Parse a benchmark file and keep the fastest time for each result kind."""
    data = defaultdict(lambda: {'scalar': None, 'vectorized': None})
    kind = None
    try:
        with open(path, 'r') as f:
            #print("---------------------------------------------------------------------------------------------------")
            #print(path)
            for line in f:
                line = line.strip()
                if line.startswith('Vectorized results'):
                    kind = 'vectorized'
                    continue
                if line.startswith('Scalar results'):
                    kind = 'scalar'
                    continue
                m = LINE_RE.search(line)
                if not m or kind is None:
                    continue
                vects = int(m.group('vects'))
                words = int(m.group('words'))
                elapsed = float(m.group('time'))
                #print("words", words, "\nvects", vects, "\nelapsed", elapsed)
                prev = data[(vects, words)].get(kind)
                if prev is None or elapsed < prev:
                    data[(vects, words)][kind] = elapsed
    except FileNotFoundError:
        # missing file -> return empty mapping
        return {}
    return dict(data)


def collect_series(name, bits):
    """Return points containing words, vector size, and four execution times."""
    sources = {
        'scalar': ('noOpt', 'scalar'),
        'vectorial': ('o3Execution', 'vectorized'),
        'o2': ('o2Execution', 'scalar'),
        'o3': ('o3Execution', 'scalar'),
    }
    parsed = {
        case: parse_benchmark(f"{directory}/{name}_benchmark_{bits}.txt")
        for case, (directory, _) in sources.items()
    }
    keys = sorted(set().union(*(values.keys() for values in parsed.values())))
    points = []
    for (vects, words) in keys:
        times = {
            case: values.get((vects, words), {}).get(result_kind)
            for case, values in parsed.items()
            for result_kind in [sources[case][1]]
        }
        points.append((words, vects, times))
    return points


def plot(name, bits, out=None, show=True, mode='3d'):
    points = collect_series(name, bits)
    if not points:
        raise SystemExit('No data found for the requested files')

    words = np.array([p[0] for p in points], dtype=float)
    vects = np.array([p[1] for p in points], dtype=float)
    cases = [
        ('scalar', 'Scalar (noOpt)', 'forestgreen'),
        ('vectorial', 'Vectorial (O3)', 'red'),
        ('o2', 'Scalar (O2)', 'royalblue'),
        ('o3', 'Scalar (O3)', 'purple'),
    ]
    if mode == '3d':
        fig, axes = plt.subplots(2, 2, figsize=(15, 11), subplot_kw={'projection': '3d'})
        for ax, (case, title, color) in zip(axes.flat, cases):
            # times in input files are seconds; convert to milliseconds for plotting
            times = np.array([
                (point[2][case] * 1000) if point[2][case] is not None else np.nan
                for point in points
            ])
            valid = np.isfinite(times)
            # X: words, Y: execution time, Z: vector size.
            if np.any(valid):
                ax.set_xlim(words[valid].min(), words[valid].max())
                ax.set_ylim(times[valid].min(), times[valid].max())
                ax.set_zlim(vects[valid].min(), vects[valid].max())

            # Draw a line (and markers) for each distinct vector size to improve depth perception
            unique_vects = np.unique(vects[valid]) if np.any(valid) else np.array([])
            for vz in unique_vects:
                mask = (vects == vz) & valid
                if np.count_nonzero(mask) == 0:
                    continue
                # sort by words for a clean line
                idx = np.argsort(words[mask])
                x = words[mask][idx]
                y = times[mask][idx]
                z = vects[mask][idx]
                ax.plot(x, y, z, color=color, linewidth=1.2, alpha=0.9)
                ax.scatter(x, y, z, color=color, s=36, alpha=0.9, edgecolor='black', linewidth=0.3)

            ax.set_xscale('log')
            ax.set_xlabel('Number of words')
            ax.set_ylabel('Execution time (ms)')
            ax.set_zlabel('Vector size')
            ax.set_title(title)
            ax.set_box_aspect((1.5, 1.0, 1.2))
            ax.view_init(elev=24, azim=-55)

        fig.suptitle(f"{name.capitalize()} benchmark ({bits}-bit)", fontsize=16)
        plt.tight_layout()
        if out:
            plt.savefig(out, dpi=200, bbox_inches='tight')
        if show:
            plt.show()
    elif mode == 'small-multiples':
        # Group points by vector size
        from math import ceil
        grouped = {}
        for w, v, times in points:
            grouped.setdefault(v, []).append((w, times))

        vects_sorted = sorted(grouped.keys())
        if not vects_sorted:
            raise SystemExit('No data to plot')

        n = len(vects_sorted)
        cols = min(4, n)
        rows = ceil(n / cols)
        # don't share y-axis so each subplot can have its own max
        fig, axes = plt.subplots(rows, cols, figsize=(4 * cols, 3 * rows), sharey=False)
        axes_list = axes.flat if n > 1 else [axes]

        # don't impose global y-limits; we'll set each subplot individually

        for ax, v in zip(axes_list, vects_sorted):
            entries = sorted(grouped[v], key=lambda x: x[0])
            words_arr = np.array([w for w, _ in entries], dtype=float)
            # collect all finite y-values across cases for this subplot
            yvals_collect = []
            for case, title, color in cases:
                # convert seconds -> milliseconds for plotting
                times_arr = np.array([ (t.get(case, np.nan) * 1000) if t.get(case) is not None else np.nan for _, t in entries], dtype=float)
                finite = np.isfinite(times_arr)
                if finite.any():
                    ax.plot(words_arr[finite], times_arr[finite], marker='o', label=title, color=color)
                    yvals_collect.append(times_arr[finite])
            ax.set_xscale('log')
            ax.set_title(f'{v} vects')
            ax.set_xlabel('Number of words')
            # set y-limits specific to this subplot using the collected values
            if yvals_collect:
                yconcat = np.concatenate(yvals_collect)
                if yconcat.size:
                    yfinite = yconcat[np.isfinite(yconcat)]
                    if yfinite.size:
                        pad = 0.05 * (yfinite.max() - yfinite.min()) if yfinite.max() != yfinite.min() else (0.1 * yfinite.max() if yfinite.max() != 0 else 0.1)
                        ax.set_ylim(max(0, yfinite.min() - pad), yfinite.max() + pad)
        # hide unused subplots
        for i in range(n, rows * cols):
            try:
                axes.flat[i].axis('off')
            except Exception:
                pass

        # no global y-limits applied

        # legend on top
        handles, labels = axes_list[0].get_legend_handles_labels()
        if handles:
            fig.legend(handles, labels, loc='upper right')

        fig.suptitle(f"{name.capitalize()} benchmark ({bits}-bit)", fontsize=16)
        plt.tight_layout()
        if out:
            plt.savefig(out, dpi=200, bbox_inches='tight')
        if show:
            plt.show()
def main():
    p = argparse.ArgumentParser(description='Plot 3D benchmark comparisons')
    p.add_argument('name', choices=['bind','hamming','query'], help='benchmark name')
    p.add_argument('bits', choices=['8','16','32','64'], help='data width')
    p.add_argument('--out', '-o', help='output file (png)')
    p.add_argument('--no-show', action='store_true', help="don't open interactive window")
    p.add_argument('--mode', choices=['3d', 'small-multiples'], default='3d', help='plot mode')
    args = p.parse_args()
    plot(args.name, args.bits, out=args.out, show=not args.no_show, mode=args.mode)


if __name__ == '__main__':
    main()
