import numpy as np

import matplotlib.pyplot as plt

# Dane
blocks = ['4x1', '4x4', '8x1', '8x8', '16x1']

sse_means = [
    np.mean([1.360212, 1.309577]),
    np.mean([4.742248, 4.630840]),
    np.mean([2.232226, 2.230595]),
    np.mean([0.0, 0.0]),
    np.mean([3.185007, 3.639931])
]

avx_means = [
    np.mean([1.580112, 1.603852]),
    np.mean([5.143705, 5.214986]),
    np.mean([2.508850, 2.326790]),
    np.mean([8.259544, 7.236875]),
    np.mean([4.195901, 3.956599])
]

x = np.arange(len(blocks))
width = 0.35

fig, ax = plt.subplots()
rects1 = ax.bar(x - width/2, sse_means, width, label='SSE')
rects2 = ax.bar(x + width/2, avx_means, width, label='AVX')

ax.set_ylabel('GFLOPS')
ax.set_xlabel('Blok AxB')
ax.set_title('Porównanie wydajności SSE i AVX dla różnych bloków')
ax.set_xticks(x)
ax.set_xticklabels(blocks)
ax.legend()

plt.tight_layout()
plt.show()
plt.savefig("wykres_nowy.png")