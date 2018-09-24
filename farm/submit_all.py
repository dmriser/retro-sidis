#!/usr/bin/env python 

import argparse
import json 
import numpy as np 
import os 

def create_jsub(output_name='out.jsub', job_name='CreatedFarmJob', project='E1F',
                track='simulation', time=480, memory=4096, os='CENTOS7', command='ls',
                other_files=None, output_data=None, output_template=None, input_files=None):

    ''' Create a .jsub file for submission to the Jefferson lab batch farm. '''

    with open(output_name, 'w') as output_file:
        output_file.write('JOB_NAME: {}\n'.format(job_name))
        output_file.write('PROJECT: {}\n'.format(project))
        output_file.write('TRACK: {}\n'.format(track))
        output_file.write('TIME: {}\n'.format(time))
        output_file.write('MEMORY: {} MB\n'.format(memory))
        output_file.write('OS: {}\n'.format(os))
        output_file.write('\n')
        output_file.write('COMMAND: {}\n'.format(command))
        output_file.write('\n')

        if other_files is not None:
            if type(other_files) == list:
                output_file.write('OTHER_FILES:')
                output_file.write('\n'.join(other_files))
                output_file.write('\n')
            elif type(other_files) == str:
                output_file.write('OTHER_FILES: {}'.format(other_files))
                output_file.write('\n')
            else:
                print('Trouble adding other files to jsub file {}.'.format(output_name))

        if output_data is not None:
            output_file.write('OUTPUT_DATA: {}'.format(output_data))
            output_file.write('\n')

        if output_template is not None:
            output_file.write('OUTPUT_TEMPLATE: {}'.format(output_template))
            output_file.write('\n')

        if input_files is not None:
            output_file.write('INPUT_FILES: \n')
            output_file.write('\n'.join(input_files))
            output_file.write('\n')


def build_default_config(args):
    config = {}
    config['base_path']            = args.base_directory
    config['project_name']         = args.project_name
    config['variation']            = 'nominal'
    config['job_id']               = 0
    config['data_type']            = args.data_type
    config['change_strictness']    = 0
    config['strict_to_change']     = 0
    config['strictness']           = 0
    config['correct_electron_p']   = 1
    config['correct_pion_p']       = 1
    config['acceptance_iteration'] = 2

    # This means all sectors (0), 1-6 would
    # indicate only save that sector. 
    config['select_sector']        = 0
    return config 

if __name__ == '__main__':


    ap = argparse.ArgumentParser()
    ap.add_argument('-i', '--input_files',    required=True)
    ap.add_argument('-t', '--data_type',      required=True)
    ap.add_argument('-p', '--project_name',   required=True)
    ap.add_argument('-b', '--base_directory', required=True)
    args = ap.parse_args()

    # Build a default configuration for the analysis 
    # code to run on each node. 
    config = build_default_config(args)
    
    # Open a physical file to write the current configuration
    # dictionary into.  Do so.  Create a unique name for this 
    # configuration file to be saved into.
    configuration_filename = 'config_{}_{}.json'.format(args.project_name, args.data_type)
    with open('configurations/{}'.format(configuration_filename), 'w') as output_json:
        json.dump(config, output_json, indent=4)

    input_files = []
    with open(args.input_files, 'r') as input_file_list:
        input_files = [line.strip() for line in input_file_list.readlines()]

    create_jsub(output_name='submission_files/{}_{}.jsub'.format(args.project_name, args.data_type), 
                job_name='SIDISAnalysis_{}'.format(args.project_name), 
                project='E1F',
                track='simulation', 
                time=240, 
                memory=4096, 
                os='CENTOS7', 
                command='source /u/home/dmriser/environment/jlab-root6.env ; chmod +x run_all.py ; run_all.py',
                other_files=['/u/home/dmriser/clas/retro-sidis/farm/configurations/{}'.format(configuration_filename), 
                             '/u/home/dmriser/clas/retro-sidis/farm/run_all.py'], 
                output_data=None, 
                output_template=None, 
                input_files=input_files)
    
    os.system('jsub submission_files/{}_{}.jsub'.format(args.project_name, args.data_type))
    
