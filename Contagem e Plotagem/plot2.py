import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('contagem.csv',
                 index_col= ['N'])

fig, ax = plt.subplots(2)

ax[0].plot(df.index, df['AVL'], color = 'blue')
ax[0].set_title("Custo da AVL")

ax[1].plot(df.index, df['AD'], color = 'red')
ax[1].set_title("Custo da AD")

plt.tight_layout()

plt.show()