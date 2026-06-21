import matplotlib.pyplot as plt
import numpy as np

labels = ['Webcam', 'Công khai 1', 'Công khai 2']
fps = [11.2, 10.8, 10.9]

fig, ax = plt.subplots(figsize=(6, 4))
ax.bar(labels, fps, color='#FF9800')
ax.set_ylabel('FPS')
ax.set_title('FPS trung bình trên các video')
ax.set_ylim(0, 15)

plt.savefig('Figure6.png', dpi=300, bbox_inches='tight')