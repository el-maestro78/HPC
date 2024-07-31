import math
import glob
import re
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sn
import os

def make_heatmap(filename):

    try:
        with open(filename, 'r', encoding='utf-8') as file:
            data = file.read().strip()
    except UnicodeDecodeError:
        with open(filename, 'rb') as file:
            data = file.read().decode('utf-8', errors='ignore').strip()

    data = re.findall(r'(\d+\.?\d+)', data)
    data = list(map(float, data))
    N = int(math.sqrt(len(data)))
    data = np.resize(data, (N, N))

    plt.clf()
    plt.axis('off')
    sn.heatmap(data=data, cmap="coolwarm")

    filename = filename.split('.')[1].replace('\\', '')
    try:
        plt.savefig(f"./plots/{filename}.png")
    except:
        os.makedirs('./plots', exist_ok=True)
        plt.savefig(f"/plots/{filename}.png")
    print(f"Created plot for {filename}.")

for file in glob.glob("./*.dat"):
    make_heatmap(file)

