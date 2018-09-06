#!/usr/bin/env python 

import glob 

def process_one_file(input_filename, string_to_count=''):
    counts = 0

    with open(input_filename, 'r') as input_file:
        for line in input_file.readlines():
            if string_to_count in line:
                counts += 1

    return counts 

if __name__ == '__main__':

    base_directory = '/u/home/dmriser/.farm_out'
    search_string = '38518.root'

    # Get lists of files to process 
    output_files = glob.glob('{}/*.out'.format(base_directory))
    error_files  = glob.glob('{}/*.out'.format(base_directory))
    
    for file in output_files:
        counts = process_one_file(file, search_string)
        if counts > 0:
            print('{}: {}'.format(file, counts))
    
    for file in error_files:
        counts = process_one_file(file, search_string)
        if counts > 0:
            print('{}: {}'.format(file, counts))
