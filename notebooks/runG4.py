import os
import numpy as np
import utils
from tqdm import tqdm

# float_to_hex = lambda f: float.hex(float(f)).upper()
# hex_to_float = lambda h: float.fromhex(h)

path = '/home/zihaoxu/'
os.system('cd {path}/Geant4/geant4.10.07.p02-install/bin/ && sh geant4.sh'.format(path=path))

n_event = 1000

for e_keV in tqdm(np.linspace(0, 20, 21)[1:]):

    file_name = '{path}/ERRecombStudy/cache/BetaInLXe_E_{energy}'.format(
        path=path, 
        energy=float_to_hex(e_keV).replace('.', 'o')
    )

    a = os.system(
        '{path}/ERRecombStudy/build/run {args}'.format(
            path=path,
            args=' '.join(
                ['--n_event', '%d'%n_event,
                 '--e_keV', '%.5f'%e_keV,
                 '--file_name', file_name]
            )
        )
    )
