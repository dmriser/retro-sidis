#!/usr/bin/env python 
import argparse 
import glob 
import sys, os
import subprocess
import json 

def prepare_analysis_files(base_dir='/u/home/dmriser/clas/retro-sidis/mysidis/'):
    os.system('cp -r {}* .'.format(base_dir))
 
def build_root_command(config, run_filename):
    number_of_files = 1
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

def process(config, base_filename):
        os.system(build_root_command(config, 'files.dat'))

        # Create the target directory. 
        os.system('mkdir -p {}/{}/{}/variation_{}/'.format(
            config['base_path'], 
            config['project_name'],
            config['data_type'],
            config['variation'])
        )

        # Move our completed job to the destination. 
        os.system('mv out.root {}/{}/{}/variation_{}/out.{}.root'.format(
            config['base_path'], 
            config['project_name'], 
            config['data_type'],
            config['variation'],
            base_filename)
        )

if __name__ == '__main__':


    prepare_analysis_files() 

    # This should be provided for each job. 
    with open('config.json') as input_json_file:
        config = json.load(input_json_file)

        # Create a file that contains the list 
        # of files to analyze. 

        if os.path.exists('out.root'):
            os.system('rm out.root')

        root_files = glob.glob('*.root')
        print('Found the following root files {}'.format(root_files))
        assert(len(root_files) == 1)
        root_file = root_files[0]
        base_filename = root_file.split('.root')[0]

        with open('files.dat', 'w') as run_file:
            run_file.write(root_file)

        # Run nominal configuration 
        config['change_strictness'] = 0
        config['strict_to_change']  = 0 
        config['strictness']        = 0
        process(config, base_filename)

        # Run settings (Can this be done by passing
        # in a list of variations to run?  For ex:
        # consider that we just need 3, 8, 15)
        config['change_strictness'] = 1
        for index in range(16):

            config['variation'] = str(index)

            # Tight one 
            config['strict_to_change']  = index 
            config['strictness']        = 1
            process(config, base_filename + '_tight')

            # Loose one 
            config['strict_to_change']  = index 
            config['strictness']        = -1
            process(config, base_filename + '_loose')
            
