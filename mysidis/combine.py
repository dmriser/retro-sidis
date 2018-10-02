#!/usr/bin/env python 

import argparse 
import numpy as np 
import os 

class HapradDataEntry(object):
    def __init__(self, sig, sib, tail):
        self.sig = sig
        self.sib = sib
        self.tail = tail

if __name__ == '__main__':
    
    ap = argparse.ArgumentParser()
    ap.add_argument('-i', '--input_folder', required=True)
    ap.add_argument('-o', '--output_file', required=True)
    ap.add_argument('-t', '--hadron_type', required=True)
    args = ap.parse_args()

    n_bins = dict(
        x = 5,
        q2 = 2,
        z = 18,
        pt2 = 20,
        phi = 36
    )

    with open(args.output_file, 'w') as output_file:
        for i in range(n_bins['x']):
            for j in range(n_bins['q2']):
                for k in range(n_bins['z']):
                    for m in range(n_bins['pt2']):
                        filename = '{}/{}_BiSc5_x{}QQ{}z{}PT2{}.dat'.format(args.input_folder, args.hadron_type, i, j, k, m)
                        status = os.path.exists(filename)
                        
                        if status:
                            with open(filename, 'r') as input_file:
                                lines = [line.strip() for line in input_file.readlines()]
                                
                                for index, line in enumerate(lines):
                                    tokens = line.split()
                                    output_string = '{},{},{},{},{},{},{},{}\n'.format(i, j, k, m, index, tokens[0], tokens[1], tokens[2])
                                    output_file.write(output_string)
                        else:
                            for index in range(n_bins['phi']):
                                output_string = '{},{},{},{},{},{},{},{}\n'.format(i, j, k, m, index, -123, -123, -123)
                                output_file.write(output_string)
