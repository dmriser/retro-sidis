#!/usr/bin/env python 

'''    
Author: David Riser, University of Connecticut 
Date:   09/18/2018       
File:   remove_files.py 
       
Purpose: This short program can be used to clean out a directory 
         which contains too many files to simply rm. 
'''

import argparse
import os 
import glob 

if __name__ == '__main__':

    ap = argparse.ArgumentParser()
    ap.add_argument('-i', '--input_directory', required=True)
    ap.add_argument('-e', '--extension', required=True)
    ap.add_argument('-c', '--chunk_size', type=int, default=500)
    args = ap.parse_args() 

    files_to_remove = glob.glob('{}/*.{}'.format(args.input_directory, args.extension))
    print('In {} found {} files to remove from github.'.format(args.input_directory, len(files_to_remove)))

    n_chunks = len(files_to_remove) // args.chunk_size
    
    for chunk_index in range(n_chunks):
        if chunk_index < (n_chunks - 1):
            files = files_to_remove[args.chunk_size * chunk_index:args.chunk_size * (chunk_index + 1)]
        else:
            files = files_to_remove[args.chunk_size * chunk_index::]

        string_of_files = ' '.join(files)
        command = 'rm {}'.format(string_of_files)

        print('Removing chunk ({}/{})'.format(chunk_index, n_chunks))
        os.system(command)
