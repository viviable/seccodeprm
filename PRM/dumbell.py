import csv
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
    axes = np.atleast_2d(axes)

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
                ax.set_ylabel("Score", fontsize=11)

    fig.tight_layout()
    if save_path:
        fig.savefig(save_path, bbox_inches="tight", dpi=200)
    else:
        plt.show()


def demo():
    panels, model_groups = build_panels_from_cweval(
        "/home/wyu3/workspace/PURE/PRM/eval/cweval_exp.csv"
    )
    plot_dumbbell_grid(panels, model_groups, save_path="dumbbell.png")


def load_cweval_csv(path):
    rows = []
    with open(path, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            if not row.get("name") or row["name"] == "name":
                continue
            rows.append(row)
    if not rows:
        raise ValueError(f"No usable rows found in {path}")
    for row in rows:
        for key, val in row.items():
            if key == "name":
                continue
            row[key] = float(val)
    return rows


def shorten_name(name):
    base = name.split("/")[-1]
    base = base.replace("_cweval", "")
    base = base.replace("prime_s2_ckp500_from_s0_all_", "")
    base = base.replace("prime_s2_ckp500_from_s0_", "")
    return base


def build_panels_from_cweval(csv_path):
    rows = load_cweval_csv(csv_path)
    ks = sorted(
        {
            int(col.split("@")[1])
            for col in rows[0].keys()
            if col.startswith("functional@")
        }
    )
    model_groups = [[shorten_name(r["name"]) for r in rows]]
    panels = []
    for k in ks:
        func_col = f"functional@{k}"
        sec_col = f"secure@{k}"
        group_vals = [(r[sec_col], r[func_col]) for r in rows]
        panels.append(
            {
                "title": f"@{k}",
                "logy": False,
                "series": [
                    {
                        "label": "secure->functional",
                        "values": [group_vals],
                        "marker_low": "o",
                        "marker_high": "o",
                        "color": "#1f77b4",
                    }
                ],
            }
        )
    return panels, model_groups


if __name__ == "__main__":
    demo()
