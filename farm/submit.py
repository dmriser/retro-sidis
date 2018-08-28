#!/usr/bin/env python 

import json 
import numpy as np 
import os 

def split_files(file_list=None, nodes=20):
    ''' Provided a list of files, load it and split it into n=nodes lists. '''

    if file_list is None: 
        raise ValueError('The input file list must be set.')

    with open(file_list, 'r') as files:
        input_files = [f.strip() for f in files.readlines()]
        
        total_files = len(input_files)
        files_per_node = int(np.ceil( total_files / nodes ))

        node_lists = []
        for node in range(nodes):
            if node != (nodes -1):
                node_lists.append(input_files[files_per_node * node:files_per_node * (node + 1)])
            else:
                node_lists.append(input_files[files_per_node * node:])

    return node_lists 

def create_jsub(output_name='out.jsub', job_name='CreatedFarmJob', project='E1F',
                track='simulation', time=240, memory=4096, os='CENTOS7', command='ls',
                other_files=None, output_data=None, output_template=None, input_files=None):

    ''' Create a .jsub file for submission to the Jefferson lab batch farm. '''

    with open(output_name, 'w') as output_file:
        output_file.write('NAME: {}\n'.format(job_name))
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
            output_file.write('INPUT_FILES: {}'.format(input_files))
            output_file.write('\n')



if __name__ == '__main__':
    FILE_LIST = '../mysidis/mc.dat'
    number_of_nodes = 60 
    
    config = {}
    config['base_path']            = '/volatile/clas12/dmriser/farm_out'
    config['project_name']         = 'TestSIDIS'
    config['variation']            = 'nominal'
    config['job_id']               = 0
    config['data_type']            = 'mc'
    config['change_strictness']    = 0
    config['strict_to_change']     = 0
    config['strictness']           = 0
    config['correct_electron_p']   = 1
    config['correct_pion_p']       = 1
    config['acceptance_iteration'] = 0


    for job_id, files in enumerate(split_files(file_list=FILE_LIST, nodes=number_of_nodes)):
        
        config['files'] = files
        config['job_id'] = job_id

        with open('configurations/config.{}.json'.format(job_id), 'w') as outfile:
            json.dump(config, outfile, indent=4)
            
            create_jsub(output_name='submission_files/out.{}.jsub'.format(job_id), 
                        job_name='SIDISAnalysis_{}_{}'.format(config['project_name'], job_id), 
                        project='E1F',
                        track='simulation', 
                        time=240, 
                        memory=4096, 
                        os='CENTOS7', 
                        command='source /u/home/dmriser/environment/jlab-root6.env ; chmod +x run.py ; run.py',
                        other_files=['/u/home/dmriser/clas/retro-sidis/farm/configurations/config.{}.json'.format(job_id), 
                                     '/u/home/dmriser/clas/retro-sidis/farm/run.py'], 
                        output_data=None, 
                        output_template=None, 
                        input_files=None)

            os.system('jsub submission_files/out.{}.jsub'.format(job_id))
