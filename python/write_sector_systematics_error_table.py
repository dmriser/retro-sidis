#!/usr/bin/env python 

import numpy as np 
import pandas as pd 
import ROOT 

def build_histogram_title(observable, hadron, xbin, q2bin, zbin, pt2bin):
    return 'h{}_{}_{}_{}_{}_{}'.format(
        observable, hadron, xbin, q2bin, zbin, pt2bin
    )

def root_histo_to_numpy(histogram):
    ''' Accept a TH1 object and return three 
    numpy arrays which indicate the bin content, 
    the error, and the axis values. '''

    n_bins = histogram.GetNbinsX()
    axis_values = np.zeros(n_bins)
    content = np.zeros(n_bins)
    errors = np.zeros(n_bins)

    for idx in range(n_bins):
        axis_values[idx] = histogram.GetBinCenter(idx + 1)
        content[idx] = histogram.GetBinContent(idx + 1)
        errors[idx] = histogram.GetBinError(idx + 1)

    return axis_values, content, errors

def nathans_error(values, errors):
    ''' Nathan's way of putting 
    sectors together to get an error''' 
    mean = np.mean(values)
    std = np.std(values)
    mean_error = np.sqrt(np.mean(errors**2))
    
    if mean_error > std:
        return 0.0
    else:
        return np.sqrt(std**2 - mean_error**2)

if __name__ == '__main__':

    hadron = 'pip'

    # get file open for this 
    input_filename = '/u/home/dmriser/mysidis-histos/Sector_systematics.root'
    root_file = ROOT.TFile.Open(input_filename)    

    if root_file.IsOpen():
        print('File opened.')
    else:
        print('File not opened.')

    # physics information 
    n_x = 5 
    n_q2 = 2
    n_z = 18
    n_pt2 = 20
    n_sect = 6

    # setup output dataframe structure 
    output_dict = {}
    output_dict['x_bin'] = []
    output_dict['q2_bin'] = []
    output_dict['z_bin'] = []
    output_dict['pt2_bin'] = []

    output_dict['a0_sys_sector'] = []
    output_dict['acos_sys_sector'] = []
    output_dict['acos2_sys_sector'] = []

#    for i in range(n_sect):
#        output_dict['a0_sect{}_value'.format(i)] = []
#        output_dict['acos_sect{}_value'.format(i)] = []
#        output_dict['acos2_sect{}_value'.format(i)] = []
#        output_dict['a0_sect{}_error'.format(i)] = []
#        output_dict['acos_sect{}_error'.format(i)] = []
#        output_dict['acos2_sect{}_error'.format(i)] = []

    
    for xb in range(n_x):
        for q2b in range(n_q2):
            for zb in range(n_z):
                for pt2b in range(n_pt2):

                    if (xb == 4) and (q2b == 1):
                        continue 

                    print('Processing {}, {}, {}, {}'.format(
                        xb, q2b, zb, pt2b
                    ))

                    # add bin information 
                    output_dict['x_bin'].append(xb)
                    output_dict['q2_bin'].append(q2b)
                    output_dict['z_bin'].append(zb)
                    output_dict['pt2_bin'].append(pt2b)
                    
                    # a0 
                    title = build_histogram_title('M', hadron, xb, q2b, zb, pt2b)
                    _, a0, err0 = root_histo_to_numpy(root_file.Get(title))

                    # acos
                    title = build_histogram_title('Ac', hadron, xb, q2b, zb, pt2b)
                    _, acos, errcos = root_histo_to_numpy(root_file.Get(title))
                        
                    # acos2 
                    title = build_histogram_title('Acc', hadron, xb, q2b, zb, pt2b)
                    _, acos2, errcos2 = root_histo_to_numpy(root_file.Get(title))

                    #                    for i in range(n_sect):
                    #                        output_dict['a0_sect{}_value'.format(i)].append(a0[i])
                    #                        output_dict['acos_sect{}_value'.format(i)].append(acos[i])
                    #                        output_dict['acos2_sect{}_value'.format(i)].append(acos2[i])
                    #                        output_dict['a0_sect{}_error'.format(i)].append(err0[i])
                    #                        output_dict['acos_sect{}_error'.format(i)].append(errcos[i])
                    #                        output_dict['acos2_sect{}_error'.format(i)].append(errcos2[i])

                    output_dict['a0_sys_sector'].append(nathans_error(
                        a0, err0
                    ))
                    output_dict['acos_sys_sector'].append(nathans_error(
                        acos, errcos
                    ))
                    output_dict['acos2_sys_sector'].append(nathans_error(
                        acos2, errcos2
                    ))

    df = pd.DataFrame(output_dict)        
    df.to_csv('{}_sector_systematics.csv'.format(hadron), index = False)
