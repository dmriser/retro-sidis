#!/usr/bin/env python 

import numpy as np 
import os 
import glob 

N_SYS = 16

def create_jsub(output_name='out.jsub', job_name='CreatedFarmJob', project='E1F', 
                track='simulation', time=240, memory=4096, os='CENTOS7', command='ls',
                other_files=None, output_data=None, output_template=None, input_files=None):

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

    # Setup sub directories for lists 
    # and jsub files. 
    os.system('mkdir -p {}/{}/lists'.format(base_path, project_name))
    os.system('mkdir -p {}/{}/jsubs'.format(base_path, project_name))

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

def run_initial_setup(base_path='/volatile/clas12/dmriser/farm_out', 
                      project_name='untitled', 
                      data_list='../mysidis/files.dat',
                      mc_list='../mysidis/mc.dat',
                      nodes=20):
    ''' The first time a project is executed, this function will 
    build lists of files and create configuration files. It also builds
    the directory tree for the project. '''
    
    print('Building project {}...'.format(project_name))    

    # Aggregate lists of files. 
    data_lists = split_files(data_list, nodes)
    mc_lists   = split_files(mc_list, nodes)

    # Setup directory structure. 
    build_project_directory_tree(base_path, project_name)
    
    # Save lists to the correct location. 
    index = 0
    for data, mc in zip(data_lists, mc_lists):
        with open('{}/{}/lists/data.{}.dat'.format(base_path, project_name, index), 'w') as output_file:
            output_file.write('\n'.join(data))
        with open('{}/{}/lists/mc.{}.dat'.format(base_path, project_name, index), 'w') as output_file:
            output_file.write('\n'.join(mc))

        index += 1

def create_psuedo_files(base_path='/volatile/clas12/dmriser', 
                        project_name='untitled', 
                        last_finished_variation='nominal',
                        nodes=20):
    ''' Function used for testing.  Creates fake root files. ''' 
    for index in range(nodes): 
        os.system('touch {}/{}/nominal/data/{}.root'.format(base_path, project_name, index))
        os.system('touch {}/{}/nominal/mc/{}.root'.format(base_path, project_name, index))

    if last_finished_variation != 'nominal':
    
        for current_variation_index in range(N_SYS):

            current_variation = 'variation{}L'.format(current_variation_index)
            for index in range(nodes):
                os.system('touch {}/{}/{}/data/{}.root'.format(base_path, project_name, current_variation, index))
                os.system('touch {}/{}/{}/mc/{}.root'.format(base_path, project_name, current_variation, index))
                
            if current_variation == last_finished_variation:
                break 
            else:
                current_variation = 'variation{}T'.format(current_variation_index)
                for index in range(nodes):
                    os.system('touch {}/{}/{}/data/{}.root'.format(base_path, project_name, current_variation, index))
                    os.system('touch {}/{}/{}/mc/{}.root'.format(base_path, project_name, current_variation, index))

            if current_variation == last_finished_variation:
                break
    
def determine_current_stage(base_path='/volatile/clas12/dmriser', project_name='untitled', nodes=20):
    ''' This function inspects the directory and determines the current stage of 
    analysis that needs to be completed. ''' 
    
    # First check nominal configuration. 
    nominal_data_status = verify_completed_job(base_path=base_path, 
                                               project_name=project_name, 
                                               variation='nominal',
                                               data_type='data',
                                               expected_files=nodes)
    nominal_mc_status = verify_completed_job(base_path=base_path, 
                                             project_name=project_name, 
                                             variation='nominal',
                                             data_type='mc',
                                             expected_files=nodes)

    # Provided that these are done, continue to check 
    # all other variations. 
    if (nominal_data_status and nominal_mc_status):        
        for index in range(N_SYS):
            
            # Check loose. 
            current_variation = 'variation{}L'.format(index) 
            data_status = verify_completed_job(base_path=base_path, 
                                               project_name=project_name, 
                                               variation=current_variation,
                                               data_type='data',
                                               expected_files=nodes)
            mc_status = verify_completed_job(base_path=base_path, 
                                             project_name=project_name, 
                                             variation=current_variation,
                                             data_type='mc',
                                             expected_files=nodes)
            if not (data_status and mc_status):
                return current_variation 

            # Check tight. 
            current_variation = 'variation{}T'.format(index) 
            data_status = verify_completed_job(base_path=base_path, 
                                               project_name=project_name, 
                                               variation=current_variation,
                                               data_type='data',
                                               expected_files=nodes)
            mc_status = verify_completed_job(base_path=base_path, 
                                             project_name=project_name, 
                                             variation=current_variation,
                                             data_type='mc',
                                             expected_files=nodes)
            if not (data_status and mc_status):
                return current_variation 

    else:
        return 'nominal'

def submit_variation_job(base_path='/volatile/clas12/dmriser', 
                         project_name='untitled', variation='nominal'):

    ''' Submit data and MC jobs for the specified variation. Basic setup should be finished already. '''
    data_lists = glob.glob('{}/{}/lists/data.*.dat'.format(base_path, project_name))
    mc_lists = glob.glob('{}/{}/lists/mc.*.dat'.format(base_path, project_name))
    
    if len(data_lists) == 0:
        raise ValueError('Could not find data files for project {}'.format(project_name))
    if len(mc_lists) == 0:
        raise ValueError('Could not find mc files for project {}'.format(project_name))

    for index, data_list in enumerate(data_lists):
        create_jsub(output_name='{}/{}/jsubs/data.{}.{}.jsub'.format(base_path, project_name, variation, index),
                    job_name='{}-{}-{}'.format(project_name, variation, index), 
                    other_files=data_list)
        os.system('jsub {}/{}/jsubs/data.{}.{}.jsub'.format(base_path, project_name, variation, index))

    for index, mc_list in enumerate(mc_lists):
        create_jsub(output_name='{}/{}/jsubs/mc.{}.{}.jsub'.format(base_path, project_name, variation, index),
                    job_name='{}-{}-{}'.format(project_name, variation, index), 
                    other_files=mc_list)
        os.system('jsub {}/{}/jsubs/mc.{}.{}.jsub'.format(base_path, project_name, variation, index))    
                
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

#    build_project_directory_tree(base_path='.', project_name='TestProject')
    
#    good_status = verify_completed_job(base_path='.', 
#                                       project_name='TestProject', 
#                                       data_type='data', 
#                                       expected_files=3)

#    bad_status = verify_completed_job(base_path='.', 
#                                      project_name='TestProject', 
#                                      data_type='data', 
#                                      expected_files=5)

#    assert(good_status == True)
#    assert(bad_status == False)
 
    run_initial_setup(
        base_path='.', 
        project_name='TestProject',
        data_list='../mysidis/files.dat',
        mc_list='../mysidis/mc.dat',
        nodes=20
    )

    # This populates directories for testing. 
    create_psuedo_files(        
        base_path='.', 
        project_name='TestProject',
        last_finished_variation='variation4T',
        nodes=20
    )

    # What is our next work? 
    stage = determine_current_stage(
        base_path='/u/home/dmriser/clas/retro-sidis/farm', 
        project_name='TestProject', 
        nodes=20)

    # Send it. 
    submit_variation_job(base_path='.',
                         project_name='TestProject',
                         variation=stage)
