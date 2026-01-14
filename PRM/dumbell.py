import matplotlib.pyplot as plt
import numpy as np


def plot_dumbbell_grid(panels, model_groups, save_path=None):
    """
    panels: list of dicts, each with keys:
        title: str
        series: list of dicts, each with keys:
            label: str
            values: list of (low, high) for each model in model_groups
            marker_low: str (matplotlib marker)
            marker_high: str (matplotlib marker)
            color: str
            logy: bool (optional)
    model_groups: list of lists of model names; each group is one subplot row group
    """
    rows = len(model_groups)
    cols = len(panels)
    fig, axes = plt.subplots(rows, cols, figsize=(4.2 * cols, 2.4 * rows), sharey=False)
    if rows == 1:
        axes = np.array([axes])

    for r, models in enumerate(model_groups):
        x = np.arange(len(models))
        for c, panel in enumerate(panels):
            ax = axes[r, c]
            for s in panel["series"]:
                lows = [v[0] for v in s["values"][r]]
                highs = [v[1] for v in s["values"][r]]
                for i, (lo, hi) in enumerate(zip(lows, highs)):
                    ax.vlines(x[i], lo, hi, color=s["color"], alpha=0.5, linewidth=1.5)
                    ax.plot(
                        x[i],
                        lo,
                        marker=s["marker_low"],
                        markersize=7,
                        markerfacecolor="white",
                        markeredgecolor=s["color"],
                        markeredgewidth=1.5,
                    )
                    ax.plot(
                        x[i],
                        hi,
                        marker=s["marker_high"],
                        markersize=7,
                        markerfacecolor=s["color"],
                        markeredgecolor="black",
                        markeredgewidth=0.5,
                    )

            ax.set_title(panel["title"], fontsize=12)
            ax.set_xticks(x)
            ax.set_xticklabels(models, fontsize=10)
            ax.grid(True, axis="y", linestyle="-", alpha=0.25)

            if panel.get("logy", False):
                ax.set_yscale("log")
                ax.set_ylim(1e-4, 1.0)
            else:
                ax.set_ylim(0.0, 1.0)

            if c == 0:
                ax.set_ylabel("SER", fontsize=11)

    fig.tight_layout()
    if save_path:
        fig.savefig(save_path, bbox_inches="tight", dpi=200)
    else:
        plt.show()


def demo():
    panels = [
        {"title": "Qwen2.5-Coder-7B-Instruct", "logy": False, "series": []},
        {"title": "Qwen2.5-Coder-32B-Instruct", "logy": False, "series": []},
        {"title": "Qwen3-Coder-30B-Instruct", "logy": False, "series": []},
        {"title": "SER max (log)", "logy": True, "series": []},
    ]
    model_groups = [
        ["pass@1", "pass@3", "pass@5", "pass@10"],

    ]

    colors = ["#1f77b4", "#ff7f0e", "#2ca02c", "#d62728", "#9467bd"]
    markers = [("o", "o"), ("s", "s"), ("^", "^"), ("D", "D"), ("P", "P")]
    labels = ["A", "B", "C", "D", "E"]

    rng = np.random.default_rng(3)
    for p in panels:
        for i, (mlo, mhi) in enumerate(markers):
            values = []
            for models in model_groups:
                group_vals = []
                for _ in models:
                    lo = float(rng.uniform(0.0, 0.2))
                    hi = float(rng.uniform(0.5, 1.0))
                    if p["logy"]:
                        lo = float(10 ** rng.uniform(-4, -1))
                        hi = float(10 ** rng.uniform(-1, 0))
                    group_vals.append((lo, hi))
                values.append(group_vals)
            p["series"].append(
                {
                    "label": labels[i],
                    "values": values,
                    "marker_low": mlo,
                    "marker_high": mhi,
                    "color": colors[i],
                }
            )

    plot_dumbbell_grid(panels, model_groups, save_path="dumbbell.png")


if __name__ == "__main__":
    demo()
