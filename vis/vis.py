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

def main():
    data = array.array('d')
    with open('../calc', 'rb') as f:
        n = os.fstat(f.fileno()).st_size // 8
        data.fromfile(f, n)
    f.close()

    parameter_len = int(data[0] // 8)
    parameters = Parameters(data[1:parameter_len])
    print_class(parameters)

    amount_of_steps = calc_amount_of_steps(len(data)-PARAMETERS_SIZE)
    steps = []

    start = int(PARAMETERS_SIZE)
    particle_indx = 1
    time = -parameters.t_step
    for i in range(0, amount_of_steps, STEP_SIZE):
        if data[start] == 1:
            particle_indx = 1
            time = time + parameters.t_step

        steps.append(ParticleState(particle_indx, time, data[start+1:start+STEP_SIZE]))

        start = start + STEP_SIZE
        particle_indx = particle_indx + 1


    for j in range(1*int(parameters.amount_of_particle)):
        print_class(steps[j])

        # !TODO draw!

if __name__ == "__main__":
    main()
#           5
# | | | > | | | > | | |