#!/usr/bin/env python 

import os 

if __name__ == '__main__':

    input_list = '/u/home/dmriser/clas/analysis-main/lists/files/data/nathan_goodrun.dat'
    file_dir = '/volatile/clas12/dmriser/data/all/root'

    found_files = []
    missing_files = []

    with open(input_list, 'r') as input_file:
        run_numbers = [line.strip() for line in input_file.readlines()]
        
        # Strip off the leading zero from the run number 
        runs = [run[1:] for run in run_numbers]
        
        for run in runs:
            filename = '{}/{}.root'.format(file_dir, run)

            if os.path.exists(filename):
                found_files.append(filename)
            else:
                missing_files.append(filename)


    with open('found_files.dat', 'w') as output_file:
        for file in found_files:
            output_file.write(file + '\n')

    with open('missing_files.dat', 'w') as output_file:
        for file in missing_files:
            output_file.write(file + '\n')
