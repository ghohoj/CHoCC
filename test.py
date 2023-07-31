import numpy as np
import matplotlib.pyplot as plt
fig, ax = plt.subplots(figsize=(4, 4))
cirs=[[1,2,3]]
def plot_cir(r,xc,yc):
    theta = np.linspace(0, 2 * np.pi, 200)
    x = r*np.cos(theta)+xc
    y = r*np.sin(theta)+yc
    return x,y
for cir in cirs:
    ax.plot(*plot_cir(*cir), color="darkred", linewidth=2)


ax.axis("equal")
plt.show()