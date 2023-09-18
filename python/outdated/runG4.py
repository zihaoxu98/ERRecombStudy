import os
import numpy as np
from utils import *
from tqdm import tqdm

HOME = os.environ['HOME']
n_event = 1000
os.makedirs(f'{HOME}/ERRecombStudy/cache', exist_ok=True)

for e_keV in tqdm(np.linspace(0, 100, 101)[1:]):
    file_name = f'{HOME}/ERRecombStudy/cache/BetaInLXe_E_{e_keV:.1f}.root'
    args = ' '.join(['--n_event', '%d'%n_event,
                     '--e_keV', '%.5f'%e_keV,
                     '--file_name', file_name])
    os.system(f'{HOME}/ERRecombStudy/build/run {args}')
