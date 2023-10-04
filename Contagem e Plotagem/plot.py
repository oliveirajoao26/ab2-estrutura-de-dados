import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('contagem.csv',
                 index_col= ['N'])

fig = plt.figure()

ax = fig.add_subplot(111, label= "1")
ax2 = fig.add_subplot(111, label = "2", frame_on = False)

ax.scatter(df.index,df['AD'],color="red")

ax2.scatter(df.index, df['AVL'], color="blue")

ax2.set_xlabel("Números inseridos", color="black")
ax2.set_ylabel("Custo AVL", color="C0")
ax2.set_ylim(0, 25)


ax.yaxis.tick_right()
ax.set_ylabel('Custo AD', color="red")        
ax.yaxis.set_label_position('right') 
ax.set_ylim(0, 25)



plt.grid()

plt.show()