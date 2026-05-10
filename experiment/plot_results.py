import re
import numpy as np
import matplotlib.pyplot as plt

# ============================================================
# ЗАГРУЗКА ДАННЫХ ИЗ res.txt
# ============================================================

with open("res.txt") as f:
    content = f.read()

# строки данных: | # | N | val1 | val2 |
row = r'\|\s*\d+\s*\|\s*(\d+)\s*\|\s*([\d.]+)\s*\|\s*([\d.]+)\s*\|'

parts = content.split("Таблица")

t1 = re.findall(row, parts[1])  # Таблица 1
t2 = re.findall(row, parts[2])  # Таблица 2

n1       = np.array([int(r[0])   for r in t1])
t_insert = np.array([float(r[1]) for r in t1])
t_build  = np.array([float(r[2]) for r in t1])

n2          = np.array([int(r[0])   for r in t2])
t_extractmin = np.array([float(r[1]) for r in t2])
t_decrease   = np.array([float(r[2]) for r in t2])

# ============================================================
# ПРЕОБРАЗОВАНИЕ
# ============================================================

x1 = n1 / 1000  # тысячи элементов
x2 = n2 / 1000

t_insert     *= 1e6  # секунды -> микросекунды
t_build      *= 1e6
t_extractmin *= 1e6
t_decrease   *= 1e6

xf1 = np.linspace(x1.min(), x1.max(), 400)
xf2 = np.linspace(x2.min(), x2.max(), 400)

# ============================================================
# АППРОКСИМАЦИЯ (ПОЛИНОМ 3-Й СТЕПЕНИ)
# ============================================================

def approx(x, y, xf, deg=3):
    p = np.poly1d(np.polyfit(x, y, deg))
    return p(xf)

# ============================================================
# НАСТРОЙКИ
# ============================================================

plt.rcParams.update({
    "figure.figsize": (14, 5),
    "axes.grid": True,
    "grid.linestyle": "--",
    "grid.alpha": 0.3,
    "lines.linewidth": 2,
    "font.size": 11
})

fig, ax = plt.subplots(1, 2)

# ============================================================
# ГРАФИК 1 — Таблица 1: insert vs build
# ============================================================

ax[0].scatter(x1, t_insert, label="minheap_insert")
ax[0].scatter(x1, t_build,  label="build_minheap")
ax[0].plot(xf1, approx(x1, t_insert, xf1), "--")
ax[0].plot(xf1, approx(x1, t_build,  xf1), "--")
ax[0].set_title("Добавление элементов")
ax[0].set_xlabel("Количество элементов (тыс.)")
ax[0].set_ylabel("Время (мкс)")
ax[0].set_xlim(x1.min(), x1.max())
ax[0].legend()

# ============================================================
# ГРАФИК 2 — Таблица 2: extractmin vs decrease_key
# ============================================================

ax[1].scatter(x2, t_extractmin, label="minheap_extractmin")
ax[1].scatter(x2, t_decrease,   label="minheap_decrease_key")
ax[1].plot(xf2, approx(x2, t_extractmin, xf2), "--")
ax[1].plot(xf2, approx(x2, t_decrease,   xf2), "--")
ax[1].set_title("Извлечение и уменьшение приоритета")
ax[1].set_xlabel("Количество элементов (тыс.)")
ax[1].set_ylabel("Время (мкс)")
ax[1].set_xlim(x2.min(), x2.max())
ax[1].legend()

plt.tight_layout()
plt.savefig("results.png", dpi=150)
plt.show()
