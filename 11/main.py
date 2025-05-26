import numpy as np

import matplotlib.pyplot as plt

# Dane
n_b = [4, 8, 10, 16, 20, 25]

blocked_gflops = [
    np.mean([1.470513, 1.487689]),
    np.mean([2.548481, 2.272991]),
    np.mean([2.372252, 2.374663]),
    np.mean([2.095505, 2.088023]),
    np.mean([2.475584, 2.452556]),
    np.mean([2.164582, 2.176761])
]

naive_gflops = [
    np.mean([0.359413, 0.365086]),
    np.mean([0.364435, 0.364173]),
    np.mean([0.361920, 0.361935]),
    np.mean([0.365760, 0.362329]),
    np.mean([0.357743, 0.379534]),
    np.mean([0.358230, 0.354977]),
    
]

plt.figure(figsize=(8,5))
plt.plot(n_b, blocked_gflops, marker='o', label='Blocked')
plt.plot(n_b, naive_gflops, marker='s', label='Naive')
plt.xlabel('Liczba bloków $n_b$')
plt.ylabel('Średnia GFLOPS')
plt.title('Porównanie wydajności (GFLOPS) dla różnych liczby bloków')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
plt.savefig("wykres.png")