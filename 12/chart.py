import matplotlib.pyplot as plt

# Dane
data = {
    "2x1": [0.818248, 0.792001],
    "2x2": [1.413147, 1.318282],
    "4x1": [1.174033, 1.300282],
    "4x4": [1.693266, 1.847599],
    "8x1": [2.244353, 2.415496],
    "16x1": [3.570034, 3.149775],
}

labels = list(data.keys())
means = [sum(vals)/len(vals) for vals in data.values()]
errors = [max(abs(val - mean) for val in vals) for vals, mean in zip(data.values(), means)]

plt.figure(figsize=(8, 5))
plt.bar(labels, means, yerr=errors, capsize=5, color='skyblue')
plt.ylabel('GFLOPS (średnia)')
plt.xlabel('Algorytm (AxB)')
plt.title('Wydajność GFLOPS dla różnych algorytmów')
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.tight_layout()
plt.show()
plt.savefig("wykres.png")