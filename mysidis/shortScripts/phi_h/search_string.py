#!/usr/bin/env python 

import argparse 
import glob 

def process_one_file(input_filename, string_to_count=''):
    counts = 0

    with open(input_filename, 'r') as input_file:
        for line in input_file.readlines():
            if string_to_count in line:
                counts += 1

    return counts 

if __name__ == '__main__':

    ap = argparse.ArgumentParser()
    ap.add_argument('-s', '--search_string', required=True)
    args = ap.parse_args()

    base_directory = '.'

    # Get lists of files to process 
    output_files = glob.glob('{}/*.C'.format(base_directory))
    
    for file in output_files:
        counts = process_one_file(file, args.search_string)
        if counts > 0:
            print('{}: {}'.format(file, counts))
