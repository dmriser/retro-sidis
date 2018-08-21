#!/usr/bin/env python 

import numpy as np 
import os 
import glob 

N_SYS = 16

def create_jsub(output_name='out.jsub', job_name='CreatedFarmJob', project='E1-F', 
                track='simulation', time=240, memory=4096, os='CENTOS7', command='ls',
                other_files=None, output_data=None, output_template=None):

    ''' Create a .jsub file for submission to the Jefferson lab batch farm. '''

    with open(output_name, 'w') as output_file:
        output_file.write('NAME: {}\n'.format(job_name))
        output_file.write('PROJECT: {}\n'.format(project))
        output_file.write('TRACK: {}\n'.format(track))
        output_file.write('TIME: {}\n'.format(time))
        output_file.write('MEMORY: {}\n'.format(memory))
        output_file.write('OS: {}\n'.format(os))
        output_file.write('\n')
        output_file.write('COMMAND: {}\n'.format(command))
        output_file.write('\n')
        
        if other_files is not None:
            output_file.write('OTHER_FILES:')
            output_file.write('\n'.join(other_files))
            output_file.write('\n')

        if output_data is not None:
            output_file.write('OUTPUT_DATA: {}'.format(output_data))
            output_file.write('\n')

        if output_template is not None:
            output_file.write('OUTPUT_TEMPLATE: {}'.format(output_template))
            output_file.write('\n')

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

def build_project_directory_tree(base_path='/volatile/clas12/dmriser/farm_out', project_name='untitled'):
    ''' Setup the directory structure for this project. '''

    # Setup root directory. 
    os.system('mkdir -p {}/{}'.format(base_path, project_name))

    # Nominal run. 
    os.system('mkdir -p {}/{}/nominal/data'.format(base_path, project_name))
    os.system('mkdir -p {}/{}/nominal/mc'.format(base_path, project_name))

    # All systematics 
    for index in range(N_SYS):
        os.system('mkdir -p {}/{}/variation{}L/data'.format(base_path, project_name, index))
        os.system('mkdir -p {}/{}/variation{}L/mc'.format(base_path, project_name, index))
        os.system('mkdir -p {}/{}/variation{}T/data'.format(base_path, project_name, index))
        os.system('mkdir -p {}/{}/variation{}T/mc'.format(base_path, project_name, index))

def verify_completed_job(base_path='/volatile/clas12/dmriser/farm_out', 
                         project_name='untitled', variation='nominal', data_type='data', expected_files=0):

    ''' Given a project, go and check that the jobs are successfully completed. '''
    output_files = glob.glob('{}/{}/{}/{}/*.root'.format(base_path, 
                                                         project_name, 
                                                         variation, 
                                                         data_type))
    discovered_files = len(output_files)
    return (discovered_files == expected_files)

if __name__ == '__main__':
#    other_files = ['file1.dat', 'file2.dat', 'file3.dat']
#    output_data = 'OutputData'
#    output_template = 'OutputTemplate'
#    create_jsub(output_name='test.jsub', 
#                other_files=other_files, 
#                output_template=output_template,
#                output_data=output_data) 
#
#    job_files = split_files('../mysidis/files.dat', nodes=20)
#    
#    for job in job_files:
#        print('-' * 60)
#        print('\n'.join(job))

    build_project_directory_tree(base_path='.', project_name='TestProject')
    
    good_status = verify_completed_job(base_path='.', 
                                       project_name='TestProject', 
                                       data_type='data', 
                                       expected_files=3)

    bad_status = verify_completed_job(base_path='.', 
                                      project_name='TestProject', 
                                      data_type='data', 
                                      expected_files=5)

    assert(good_status == True)
    assert(bad_status == False)
    
