#!/usr/bin/env python 
import argparse 
import glob 
import sys, os
import subprocess
import json 

if __name__ == '__main__':

    # Setup code. 
    os.system('cp -r /u/home/dmriser/clas/retro-sidis/mysidis/* .')

    # This should be provided for each job. 
    with open('config.json') as input_json_file:
        config = json.load(input_json_file)

        with open('temp_runfile.dat', 'w') as run_list:
            for f in config['files']:
                run_list.write('{}\n'.format(f))
                
        number_of_files = len(config['files'])
        is_data = int(config['data_type'] == 'data')
        root_cmd = 'root -b -q \'mysidis.C("{}",{},{},{},{},{},{},{},{})\''.format(
            run_list.name, 
            number_of_files, 
            is_data, 
            config['acceptance_iteration'],
            config['correct_electron_p'],
            config['correct_pion_p'],
            config['change_strictness'], 
            config['strict_to_change'], 
            config['strictness'])

        os.system(root_cmd)
        os.system('rm temp_runfile.dat')
