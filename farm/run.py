#!/usr/bin/env python 
import argparse 
import glob 
import sys, os
import subprocess
import json 
import tempfile 

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

    with open('config.json') as input_json_file:
        config = json.load(input_json_file)

        # Create a tempfile that contains the files
        # for analysis. 
        run_list = tempfile.NamedTemporaryFile() 
        for f in config['files']:
            run_list.write(f)

        n_files = len(config['files'])
        is_data = int(config['data_type'] == 'data')
        root_cmd = 'root -b -q \'mysidis.C("{}",{},{},0,1,1,{},{},{})\''.format(
            run_list.name, n_files, is_data, config['change_strict'], 
            config['strict_to_change'], config['strictness'])
        print(root_cmd)

        run_list.close() 
