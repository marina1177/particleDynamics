import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation


# Setup the figure and axes...
fig, ax = plt.subplots(figsize=(6,6))
ax.set(xlim=(-3.5, 3.5), ylim=(-3.5, 3.5), ylabel='meters', xlabel='meters', title='3-Body problem')

# parameters of the problem
T = 10. #s
m = 1.0 #kg
ks = 5 #N/m
r0 = 1. #m

# setting a timestep to be 50 ms
dt = 0.05 #s
N = int(T / dt)

# Allocating arrays for 2D problem: first axis - time. second axis - particle's number. third - coordinate
v = np.zeros((N+1, 3, 2))
r = np.zeros((N+1, 3, 2))
f = np.zeros((N+1, 3, 2))

# initial conditions for 3 particles:
r[0,0] = np.array([0., 2.])
r[0,1] = np.array([2., 0.])
r[0,2] = np.array([-1., 0.])

def compute_forces(n):
    '''The function computes forces on each particle at time step n'''
    for i in range(3):
        for j in range(3):
            if i != j:
                rij = r[n,i] - r[n,j]
                rij_abs = np.linalg.norm(rij)
                f[n, i] -= ks * (rij_abs - r0) * rij / rij_abs 
## Run dynamics:
for n in range(N):
    compute_forces(n)
    v[n+1] = v[n] + f[n]/m * dt
    r[n+1] = r[n] + v[n+1] * dt

## drawing and animating 
scat = ax.scatter(r[0,:,0], r[0,:,1], marker='o', c=['b', 'k', 'r'], s=1000)

def animate(i):
    scat.set_offsets(r[i])

ani = animation.FuncAnimation(fig, animate, frames=N)
plt.close()
## this function will create a lot of *.png files in a folder '3Body_frames'
ani.save('3body.html', writer=animation.HTMLWriter(fps= 1//dt))


from IPython.display import HTML
HTML('CNtower.html')

# def main():
# 	# with open('vis.json') as f:
# 	# 	data = json.load(f)

# 		# Fixing random state for reproducibility
#     np.random.seed(19680801)


#     N = 50
#     x = np.random.rand(N)
#     y = np.random.rand(N)
#     colors = np.random.rand(N)
#     area = (30 * np.random.rand(N))**2  # 0 to 15 point radii

#     plt.scatter(x, y, s=area, c=colors, alpha=0.5)
#     plt.show()


# if __name__ == "__main__":
# 	main()