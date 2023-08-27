#!/usr/bin/env python3
import array
import os
import timeit
import json
import random
import numpy as np
from ctypes import *
import awkward as ak
#from struct import *
import binascii
import struct
import matplotlib.pyplot as plt
from matplotlib import animation

PARAMETERS_SIZE = 10
STEP_SIZE = 4 + 3 * 3

class Parameters(Structure):
    def __init__(self, params):
        self.amount_of_particle = params[0]
        self.t_full = params[1]
        self.t_step = params[2]
        self.a = params[3]
        self.ra = params[4]
        self.Uac = params[5]
        self.Udc = params[6]
        self.freq = params[7]
        self.nu = params[8]

class ParticleState(Structure):
    def __init__(self, indx, time, state):
        self.indx = indx
        self.time = time

        self.rho = state[0]
        self.d = state[1]
        self.q = state[2]

        self.rx = state[3]
        self.ry = state[4]
        self.rz = state[5]

        self.vx = state[6]
        self.vy = state[7]
        self.vz = state[8]

        self.ax = state[9]
        self.ay = state[10]
        self.az = state[11]


def print_class(class_to_print):
    attrs = vars(class_to_print)
    print("{ ", ', '.join("%s: %s" % item for item in attrs.items()), "}")

def calc_amount_of_steps(data_len):
    print("amount_of_steps: ",data_len / STEP_SIZE)

    return data_len // STEP_SIZE

def load_data(data, parameters):
    amount_of_steps = calc_amount_of_steps(len(data) - PARAMETERS_SIZE)

    steps = []

    start = int(PARAMETERS_SIZE)
    particle_indx = 1
    time = -parameters.t_step
    for i in range(0, amount_of_steps, STEP_SIZE):
        if data[start] == 1:
            particle_indx = 1
            time = time + parameters.t_step

        steps.append(ParticleState(particle_indx, time, data[start + 1:start + STEP_SIZE]))

        start = start + STEP_SIZE
        particle_indx = particle_indx + 1
    return steps


def main():
    data = array.array('d')
    with open('../calc', 'rb') as f:
        n = os.fstat(f.fileno()).st_size // 8
        data.fromfile(f, n)
    f.close()

    parameter_len = int(data[0] // 8)
    parameters = Parameters(data[1:parameter_len])
    steps = load_data(data, parameters)

    # for j in range(2*int(parameters.amount_of_particle)):
    #     print_class(steps[j])

    # !TODO draw!

    N = int(parameters.t_full / parameters.t_step)

    # Setup the figure and axes...
    fig, ax = plt.subplots(figsize=(6, 6))
    trap_size = parameters.ra
    ax.set(xlim=(-trap_size/2, trap_size/2), ylim=(-trap_size/2, trap_size/2), ylabel='meters', xlabel='meters', title='2D Particle dynamics')

    r = np.zeros((N + 1, int(parameters.amount_of_particle), 3))

    for k in range(N*int(parameters.amount_of_particle)):
        dt = int(k//int(parameters.amount_of_particle))
        #print("t = ", dt, "indx = ", steps[k].indx)
        r[dt, steps[k].indx-1, 0] = steps[k].rx
        r[dt, steps[k].indx-1, 1] = steps[k].ry
        r[dt, steps[k].indx-1, 2] = steps[k].rz


    # for n in range(2*int(parameters.amount_of_particle)):
    #     dt = int(n // int(parameters.amount_of_particle))
    #     print("t = ",dt, "indx = ",steps[n].indx)
    #     print("rx=",r[dt, steps[n].indx-1, 0],
    #           " ry=",r[dt, steps[n].indx-1, 1],
    #            " rz=",r[dt, steps[n].indx-1, 2])



    ## drawing and animating
    scat = ax.scatter(r[0, :, 0], r[0, :, 1], marker='o', c=['b'], s=100)

    def animate(i):
        scat.set_offsets(r[i])

    ani = animation.FuncAnimation(fig, animate, frames=N)
    plt.close()
    ## this function will create a lot of *.png files in a folder '3Body_frames'
    ani.save('particle_dynamics.html', writer=animation.HTMLWriter(fps=(1//parameters.t_step)))

    from IPython.display import HTML
    HTML('CNtower.html')


if __name__ == "__main__":
    main()
#   0 1 2   3 4 5
# # | | | > | | | > | | |
#целая часть от деления на 3 - номер интервала