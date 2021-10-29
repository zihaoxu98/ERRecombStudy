import numpy as np
import matplotlib.pyplot as plt
from scipy.special import erf

float_to_hex = lambda f: float.hex(float(f)).upper()
hex_to_float = lambda h: float.fromhex(h)

def decode_energy(fn):
    return hex_to_float(fn.split('_')[-1].replace('.root', '').replace('.pkl', '').replace('o', '.'))

def nest_er_recomb(Energy, Field):
    ATOM_NUM = 54
    MolarMass = 131.293
    density = 2.862
    Wq_eV = 20.7 - 1.01e-23 * (density / MolarMass) * 6.0221409e+23 * ATOM_NUM
    Nq=0.
    Qy=0.
    Ly=0.
    Ne=0.
    Nph=0.
    Lindhard=0.
    NexONi=0.
    Nq = Energy / Wq_eV * 1e3;
    
    QyLvllowE = 1e3 / Wq_eV + 6.5 * (1. - 1. / (1. + np.power(Field / 47.408, 1.9851)))
    HiFieldQy = 1. + 0.4607 / np.power(1. + np.power(Field / 621.74, -2.2717), 53.502);
    QyLvlmedE = 32.988 - 32.988 / (1. + np.power(Field / (0.026715 * np.exp(density / 0.33926)), 0.6705))
    QyLvlmedE *= HiFieldQy
    DokeBirks = 1652.264 + (1.415935e10 - 1652.264) / (1. + np.power(Field / 0.02673144, 1.564691));
    QyLvlhighE = 28.0;

    Qy = QyLvlmedE + (QyLvllowE - QyLvlmedE) / np.power(1. + 1.304 * np.power(Energy, 2.1393), 0.35535) + QyLvlhighE / (1. + DokeBirks * np.power(Energy, -2.0));
    Ly = Nq / Energy - Qy;
    Ne = Qy * Energy;
    Nph = Ly * Energy;
    
    Lindhard = 1.0;
    alpha = 0.067366 + density * 0.039693;
    NexONi = alpha * erf(0.05 * Energy);

    FreeParam = [1.,1.,0.1,0.5,0.19,2.25];
    elecFrac = Ne / Nq;
    if (elecFrac > 1.0):
        elecFrac = 1.;
    if (elecFrac < 0.0):
        elecFrac = 0.;
    highE = False;
    if (NexONi < 0): 
        NexONi = 0
        highE = true

    alf = 1. / (1. + NexONi);
    recombProb = 1. - (NexONi + 1.) * elecFrac;

    if (recombProb < 0):
        NexONi = 1. / elecFrac - 1.

    # float Fano; 
    # int Nq_actual, Ni_actual, Nex_actual;

    Fano = 0.12707 - 0.029623 * density - 0.0057042 * np.power(density, 2.) + 0.0015957 * np.power(density, 3.) + 0.0015 * np.sqrt(Nq) * np.power(Field, 0.5);
    Nq_actual = int(np.random.normal() * np.sqrt(Fano * Nq) + Nq);
    if (Nq_actual < 0):
        Nq_actual = 0
    Ni_actual = np.random.binomial(Nq_actual, alf)
    Nex_actual = Nq_actual - Ni_actual;
    if (Ni_actual < 0):
        Ni_actual = 0
        Nex_actual = Nq_actual
    if (Nex_actual < 0):
        Nex_actual = 0
        Ni_actual = Nq_actual

    if (Nex_actual <= 0 and highE):
        recombProb = Nph / float(Ni_actual);
    if (recombProb < 0.):
        recombProb = 0.;
    if (recombProb > 1.):
        recombProb = 1.;

    if (Nq_actual > 0):

        omega = 0;
        ampl = 0.14+(0.043-0.14)/(1.+np.power(Field/1210.,1.25));
        if (ampl < 0):
            ampl = 0;
        wide = 0.205
        cntr = 0.5
        skew=-0.2;
        mode = cntr + np.sqrt(2./3.1415926)*skew*wide/np.sqrt(1.+skew*skew);
        norm = 1./(np.exp(-0.5*np.power(mode-cntr,2.)/(wide*wide))*(1.+erf(skew*(mode-cntr)/(wide*np.sqrt(2.)))));
        omega = norm*ampl*np.exp(-0.5*np.power(elecFrac-cntr,2.)/(wide*wide))*(1.+erf(skew*(elecFrac-cntr)/(wide*np.sqrt(2.))));
        if (omega < 0):
            omega = 0
            
    variance = recombProb * (1. - recombProb) * Ni_actual + omega * omega * Ni_actual * Ni_actual;
    if Ne + Nph > 1e4 or Field > 4e3 or Field < 50.:
        skewness = 0.
    else:
        alpha0 = 1.39
        cc0 = 4.0
        cc1 = 22.1
        E0 = 7.7
        E1 = 54.
        E2 = 26.7
        E3 = 6.4
        F0 = 225.
        F1 = 71.;
        
        skewness = 1. / (1. + np.exp((Energy - E2) / E3)) * (alpha0 + cc0 * np.exp(-1. * Field / F0) * (1. - np.exp(-1. * Energy / E0))) + 1. / (1. + np.exp(-1. * (Energy - E2) / E3)) * cc1 * np.exp(-1. * Energy / E1) * np.exp(-1. * np.sqrt(Field) / np.sqrt(F1));
        
        
        widthCorrection = np.sqrt( 1. - (2./3.1415926) * skewness*skewness/(1. + skewness*skewness));
        muCorrection = (np.sqrt(variance)/widthCorrection)*(skewness/np.sqrt(1.+skewness*skewness))*np.sqrt(2./np.pi);
    
#     if (skewness == 0)
#         Ne_actual = int(curand_normal(s) * sqrt(variance) + (1.-recombProb)*Ni_actual);
#     else
#         Ne_actual = int(gpu_skew_gaussian(s, (1.-recombProb)*Ni_actual-muCorrection, sqrt(variance)/widthCorrection, skewness));
    if skewness == 0:
        recomb_fluc = omega
    else:
        recomb_fluc = omega / widthCorrection

    return recombProb, recomb_fluc

def xenon1t_er_recomb(Energy, Field):
    xe1t_params = {"er_fraction_ambe_sr1": 0.030295843987699224, "rf0_sr1": 0.03358301091245768, "ac_rate_rn220_sr0": 1.6170966323266516, "g1": 0.142273708702041, "zeta": 0.044974109637703236, "ac_rate_ambe_sr1": 1.6748087942520922, "py2": -0.24646905912359227, "nr_rate_ambe_sr1": 2025.9599659367652, "eta": 3.35428057146546, "kappa": 0.13776090218160925, "s_s1_acc": 0.07731780866868798, "p_dpe": 0.2191448166462383, "er_fraction_ambe_sr0": 0.2454063712570706, "s_rec_eff": -0.31816407029454036, "rf1_sr0": 1.8180247075970317, "nr_rate_ambe_sr0": 1992.04939329848, "s_s2_acc": -0.1228122910975008, "ac_rate_ambe_sr0": 1.6927220473962246, "py3": 1.0950774408106905, "ac_rate_ng_sr1": 8.416229702014286, "s_scut_acc_on_mscatter": 0.5221952478657972, "py0": 0.12479984184924389, "beta": 269.4569455797065, "delta": 0.06108723720617416, "g2": 11.388194791209763, "py1": 30.325423964060334, "W": 0.013809387994615736, "gamma": 0.014140864759394875, "s_rec_s2bias_mean": 0.49198507921078005, "s_rec_bias_mean": 0.5948841302444277, "rf0_sr0": 0.040581595787840694, "nr_rate_ng_sr1": 2051.596216517608, "er_fraction_ng_sr1": 0.011860170398825927, "py4": 0.48166709274590885, "s_pos_rec": 0.5686231763318798, "elifetime_uncertainty_sr1": 0.005973513511586094, "er_rate_rn220_sr1": 7514.547481886541, "py_th": 4.017265580395882, "s_rec_bias_res": 0.31763472112328395, "lambda": 1.1444457673408979, "s_rec_s2bias_res": 0.5272888301015638, "Nex/Ni": 0.1455388304025605, "er_rate_rn220_sr0": 993.7741378351051, "alpha": 1.283267781071301}
    
    W = xe1t_params['W']
    ExIonRatio = xe1t_params['Nex/Ni']
    rec_frac_pars = [xe1t_params['py0'], xe1t_params['py1'], xe1t_params['py2'], xe1t_params['py3'], xe1t_params['py4']]
    rec_fluc_pars = [xe1t_params['rf0_sr0'], xe1t_params['rf1_sr0']]

    MeanQuantaNum = Energy / W
    ExcimerNum = MeanQuantaNum * (ExIonRatio/(1.0 + ExIonRatio))
    IonNum = MeanQuantaNum - ExcimerNum
    HeatQuenching = 1.
    TI = rec_frac_pars[0]*np.exp(-Energy/rec_frac_pars[1])*np.power(Field, rec_frac_pars[2])
    RecombFrac = 1.0 - np.log(1.0 + (TI/4.0)*IonNum)/((TI/4.0)*IonNum)
    FD_fraction = 1. / (1. + np.exp( -(Energy-rec_frac_pars[3])/rec_frac_pars[4] ) )
    RecombFrac *= FD_fraction

    DeltaR = rec_fluc_pars[0] * (1. - np.exp(-Energy/rec_fluc_pars[1]) )
    
    return RecombFrac, DeltaR

def use_xenon_plot_style():
    params = {
        'font.family': 'serif',
        'font.size' : 24, 'axes.titlesize' : 24,
        'axes.labelsize' : 24, 'axes.linewidth' : 2,
        # ticks
        'xtick.labelsize' : 22, 'ytick.labelsize' : 22, 'xtick.major.size' : 16, 'xtick.minor.size' : 8,
        'ytick.major.size' : 16, 'ytick.minor.size' : 8, 'xtick.major.width' : 2, 'xtick.minor.width' : 2,
        'ytick.major.width' : 2, 'ytick.minor.width' : 2, 'xtick.direction' : 'in', 'ytick.direction' : 'in',
        # markers
        'lines.markersize' : 12, 'lines.markeredgewidth' : 3, 'errorbar.capsize' : 8, 'lines.linewidth' : 3,
        #'lines.linestyle' : None, 'lines.marker' : None,
        'savefig.bbox' : 'tight', 'legend.fontsize' : 24,
        'backend': 'Agg', 'mathtext.fontset': 'dejavuserif', 'legend.frameon' : False,
        # figure
        'figure.facecolor':'w',
        #'figure.figsize':(9,9),
        'figure.figsize':(12,8),
        #pad
        'axes.labelpad':12,
        # ticks
        'xtick.major.pad': 6,   'xtick.minor.pad': 6,
        'ytick.major.pad': 3.5, 'ytick.minor.pad': 3.5,
        # colormap
        #'image.cmap':'viridis'
    }
    plt.rcParams.update(params)