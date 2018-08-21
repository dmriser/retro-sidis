#!/usr/bin/env python 
import argparse 
import glob 
import sys, os
import subprocess

def setup_analysis():
    os.system('cp -r /u/home/dmriser/clas/retro-sidis/mysidis/* .')

def run():
    os.system('root -b -q run.cpp')

def rename():
    root_files = glob.glob('*.root')
    
    for index, root_file in enumerate(root_files):
        base_name = root_file.split('.root')[0]

        os.system('mv {} {}.part{}.root'.format(root_file, base_name, index))

    return glob.glob('*.root')

if __name__ == '__main__':
    ''' Run the analysis for a set of files 
    that has been provided.   
    '''
    ap = argparse.ArguementParser() 
    ap.add_argument('-o', '--output_directory', required=True)
    args = ap.parse_args()

    # Copy the files to this node. 
    setup_analysis()
    run() 
    output_files = rename() 
    
    for f in output_files:
        os.system('mv {} {}'.format(f, args.output_directory))
    
    
    
