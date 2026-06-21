import matplotlib.pyplot as plt
import numpy as np

labels = ['Webcam', 'Công khai']
accuracy = [0.66, 0.65]
f1_score = [0.80, 0.84]

x = np.arange(len(labels))
width = 0.35

fig, ax = plt.subplots(figsize=(6, 4))
ax.bar(x - width/2, accuracy, width, label='Accuracy', color='#4CAF50')
ax.bar(x + width/2, f1_score, width, label='F1-score', color='#2196F3')
ax.set_ylabel('Giá trị')
ax.set_title('So sánh Accuracy và F1-score')
ax.set_xticks(x)
ax.set_xticklabels(labels)
ax.legend()

plt.savefig('Figure5.png', dpi=300, bbox_inches='tight')