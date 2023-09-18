import numpy as np
from scipy.stats import special_ortho_group as SO
from time import time

import pycuda.driver as drv
import pycuda.autoinit
from pycuda.compiler import SourceModule

import uproot
import utils
import glob
from tqdm import tqdm

num = 1000
n_alpha = 30
n_beta = 30
alpha_upper = 5.0
beta_upper = 10.0

alpha = np.linspace(0, alpha_upper, n_alpha + 1)[1:]
beta = np.linspace(0, beta_upper, n_beta + 1)[1:]

filenames = glob.glob('../cache/*.root')

gpu_kernels = open('./CudaKernels.cpp', 'r').read()
mod = SourceModule(gpu_kernels)
func = mod.get_function('calculate_recomb')

def decode_energy(fn):
    return utils.hex_to_float(fn.split('_')[-1].replace('.root', '').replace('o', '.'))

def make_gpu_args(X, Y, Ed, EventID, params):
    # in
    num_event = np.asarray(len(np.unique(EventID))).astype(np.int32)
    print("num_event \t= %i" % num_event[()])
    
    # in
    params = np.asarray(params).astype(np.float32)
    
    # in
    X = np.asarray(X).astype(np.float32)
    Y = np.asarray(Y).astype(np.float32)
    Ed = np.asarray(Ed).astype(np.float32)

    # in
    event_end = np.argwhere(np.diff(EventID, prepend=-1, append=num_event+1)!=0).ravel()
    event_end = np.asarray(event_end).astype(np.int32)
    
    # placeholders
    # in
    X_edge = np.zeros(2 * len(X)).astype(np.float32)
    Y_edge = np.zeros(2 * len(Y)).astype(np.float32)

    # in
    X_edge_ID = np.zeros(2 * len(X)).astype(np.int32)
    Y_edge_ID = np.zeros(2 * len(Y)).astype(np.int32)

    # in
    _, num_hits = np.unique(EventID, return_counts=True)
    weight = np.zeros(np.sum((2*num_hits-1)**2)).astype(np.float32)

    # in
    recomb = np.zeros(num_event).astype(np.float32)

    # in
    fcache = np.zeros(4 * len(X)).astype(np.float32)
    icache = np.zeros(4 * len(X)).astype(np.int32)
    
    mem_bytes = np.sum([arr.nbytes for arr in [num_event, params, X, Y, Ed, event_end, X_edge, Y_edge, X_edge_ID, Y_edge_ID, weight, recomb, fcache, icache]])
    print("Memory used \t= %i MB" % (mem_bytes/1e6))
    
    gpu_args = [
        drv.In(X),
        drv.In(Y),
        drv.In(Ed),
        drv.In(event_end),
        drv.In(num_event),
        drv.In(params),
        drv.In(X_edge),
        drv.In(Y_edge),
        drv.In(X_edge_ID),
        drv.In(Y_edge_ID),
        drv.In(weight),
        drv.In(fcache),
        drv.In(icache),
        drv.InOut(recomb)
    ]
    
    other_kwargs = {
        "num_event" : int(num_event[()]),
        "params" : params,
        "recomb_alloc" : gpu_args[-1],
        "auto_block" : (int(num_event[()]), 1, 1)
    }
    
    return gpu_args, other_kwargs

def free_gpu_args(args):
    if args == []:
        return
    for gpu_arr in args:
        alloc = gpu_arr.get_device_alloc()
        alloc.free()

for fn in filenames[:]:
    energy = decode_energy(fn)

    print()
    print()
    print("#############################################################")
    print("file = %s" % fn)
    print("energy = %f keV" % energy)
    print("alpha = np.linspace(0, %f, %i)" % (alpha_upper, n_alpha))
    print("beta = np.linspace(0, %f, %i)" % (beta_upper, n_beta))
    print("num_event = %i" % num)
    print("#############################################################")
    print()
    print()
    
    if energy < 13.5:
        continue
    if energy > 14.0:
        continue
    if glob.glob(fn.replace('.root', '.pkl')) != []:
        continue

    with uproot.open(fn) as file:
        rawEventID = file['LXe;1']['Event'].array(library='np')
        rawEd = -file['LXe;1']['Ed'].array(library='np')
        rawX = file['LXe;1']['X'].array(library='np')
        rawY = file['LXe;1']['Y'].array(library='np')
        rawZ = file['LXe;1']['Z'].array(library='np')

    mask = rawEd > 0
    mask &= (rawEventID >= 0) & (rawEventID < num)

    Ed = rawEd[mask]
    X = rawX[mask]
    Y = rawY[mask]
    Z = rawZ[mask]
    EventID = rawEventID[mask]
    
    SO3 = SO(3)
    tmp = np.stack([X, Y, Z]).T
    for _id in tqdm(range(num), desc='Rotating'):
        mask = EventID == _id
        rot = SO3.rvs()
        tmp[mask] = np.dot(tmp[mask], rot)
    X = tmp[:, 0]
    Y = tmp[:, 1]
    Z = tmp[:, 2]

    gpu_args, kwargs = make_gpu_args(X, Y, Ed, EventID, [0.0, 0.0])

    cache = np.zeros(n_alpha * n_beta, dtype=[
        ('alpha', float),
        ('beta', float),
        ('recomb', (float, (num,)))
    ])

    for i in tqdm(range(n_alpha), desc='energy=%f'%energy):
        for j in range(n_beta):
#             print(i, j, alpha[i], beta[j])
            gpu_args[5].get_device_alloc().free()
            gpu_args[5] = drv.In(
                np.asarray([alpha[i], beta[j]]).astype(np.float32)
            )
            func(*gpu_args, block=kwargs['auto_block'])

            cache[i * n_beta + j]['alpha'] = alpha[i]
            cache[i * n_beta + j]['beta'] = beta[j]
            cache[i * n_beta + j]['recomb'] = kwargs['recomb_alloc'].array

    free_gpu_args(gpu_args)
    np.save(open(fn.replace('.root', '.pkl'), 'bw'), cache, allow_pickle=True)
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
