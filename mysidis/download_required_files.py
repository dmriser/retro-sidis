#!/usr/bin/env python 

''' 

David Riser, 
University of Connecticut 
September 17, 2018

Use this script to download the required files from the 
mass storage system and place them in a large local 
storage space.  Then ensure that it correctly extracts 
the contents of requiredFiles.tar (smaller).  Finally,
a list of files for data/mc is placed into the directory
requiredFiles/lists/. 

'''

import argparse 
import os 

def setup_storage_dir(storage_directory):
    ''' Setup the main directory to store large files. '''
    os.mkdir(storage_directory)

def setup_sub_dirs(storage_directory):
    os.mkdir('{}/data'.format(storage_directory))
    os.mkdir('{}/mc'.format(storage_directory))

if __name__ == '__main__':

    ap = argparse.ArgumentParser(description='This utility automates the process of getting the files required to perform analysis on the E1-F dataset using the macros of this repository.')
    ap.add_argument('-s', '--storage_directory', required=True, help='This should be a large storage area for data files, as well as analysis files.')
    args = ap.parse_args()

    # Check for file structure. 
    if os.path.exists(args.storage_directory):

        if os.path.exists('{}/data'.format(args.storage_directory)) and os.path.exists('{}/mc'.format(args.storage_directory)):
            pass
        else:
            setup_sub_dirs(args.storage_directory)

    else:
        setup_storage_dir(args.storage_directory)
        setup_sub_dirs(args.storage_directory)

        print('jget /mss/home/dmriser/data/all/root/* {}/data/'.format(args.storage_directory))
        print('jget /mss/home/dmriser/data/all/root/* {}/mc/'.format(args.storage_directory))

    # Checkout the required files (large and time consuming). 
    os.system('jget /mss/home/dmriser/data/all/root/* {}/data/'.format(args.storage_directory))
    os.system('jget /mss/home/dmriser/mc/sidis/* {}/mc/'.format(args.storage_directory))

    # Unload required files. 
    os.system('tar -xzvf requiredFiles.tar')
    os.system('rm requiredFiles/lists/*')
    os.system('ls {}/data/* > requiredFiles/lists/data.dat'.format(args.storage_directory))
    os.system('ls {}/mc/* > requiredFiles/lists/mc.dat'.format(args.storage_directory))
