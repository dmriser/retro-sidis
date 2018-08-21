#!/usr/bin/env python 
import argparse 
import glob 
import sys, os
import subprocess
import json 

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
    with open('config.json') as input_json_file:
        config = json.load(input_json_file)

        for k,v in config.iteritems():
            print(k,v)
