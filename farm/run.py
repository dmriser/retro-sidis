#!/usr/bin/env python 
import argparse 
import glob 
import sys, os
import subprocess
import json 

def prepare_analysis_files():
    # Setup code. 
    os.system('cp -r /u/home/dmriser/clas/retro-sidis/mysidis/* .')

    # Move configuration file 
    config_files = glob.glob('config.*.json')
    os.system('mv {} config.json'.format(config_files[0]))

def write_runfile(config, filename):
    with open(filename, 'w') as run_list:
        for f in config['files']:
            run_list.write('{}\n'.format(f))
    
def build_root_command(config, run_filename):
    number_of_files = len(config['files'])
    is_data = int(config['data_type'] == 'data')
    root_command = 'root -b -q \'mysidis.C("{}",{},{},{},{},{},{},{},{})\''.format(
        run_filename, 
        number_of_files, 
        is_data, 
        config['acceptance_iteration'],
        config['correct_electron_p'],
        config['correct_pion_p'],
        config['change_strictness'], 
        config['strict_to_change'], 
        config['strictness']
    )

    return root_command

if __name__ == '__main__':

    prepare_analysis_files() 

    # This should be provided for each job. 
    with open('config.json') as input_json_file:
        config = json.load(input_json_file)
                
        # Create a file that contains the list 
        # of files to analyze. 
        write_runfile(config, 'files.dat')
        os.system(build_root_command(config, 'files.dat'))

        # Clean up our file
        os.system('rm files.dat')

        # Create the target directory. 
        os.system('mkdir -p {}/{}/{}'.format(
            config['base_path'], 
            config['project_name'],
            config['data_type'])
        )

        # Move our completed job to the destination. 
        os.system('cp *.root {}/{}/{}/out.{}.root'.format(
            config['base_path'], 
            config['project_name'], 
            config['data_type'],
            config['job_id'])
        )
