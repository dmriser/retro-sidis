#!/usr/bin/env python 

import argparse
import os 
import re 
import logging 

# Setup logging
logging.basicConfig(level=logging.DEBUG)
log = logging.getLogger(__file__)

def check_and_create_directory(directory_path):
    assert(type(directory_path) == str)
    
    if os.path.exists(directory_path):
        if os.path.isdir(directory_path):
            print('Directory {} already exists'.format(directory_path))
        else:
            raise ValueError('{} exists and is not a directory!'.format(directory_path))
    else:
        os.mkdirs(directory_path)

def find_number_of_files_in_list(input_files):
    nfiles = 0
    with open(input_files, 'r') as input_file:
        files = [line.strip() for line in input_file.readlines()]

        for file in files:
            if os.path.exists(file):
                nfiles += 1
    return nfiles 
 
def extract_run_number(input_string):
    return re.findall(r'\D(\d{5})\D', input_string)[0]

def load_expected_files_from_list(input_files):
    with open(input_files, 'r') as input_file:
        files = [line.strip() for line in input_file.readlines()]

    # Verify that files exists or do not exist. 
    found_files, missing_files = [], []

    for file in files:
        if os.path.exists(file):
            found_files.append(file)
        else:
            missing_files.append(file)

    # Change from names of files, to run numbers. 
    found_runs   = map(extract_run_number, found_files)
    missing_runs = map(extract_run_number, missing_files)
    return found_runs, missing_runs 

def process_folder(dir, files, expected_nfiles, args):
    # Check the directory structure and make sure 
    # that things aren't already there. 
    merge_tight = True
    if os.path.exists('{}/tight.root'.format(dir)):
        if args.remake_files:
            os.system('rm {}/tight.root'.format(dir))
            files = [f for f in files if f != 'tight.root']
            merge_tight = True
        else:
            merge_tight = False

    merge_loose = True
    if os.path.exists('{}/loose.root'.format(dir)):
        if args.remake_files:
            os.system('rm {}/loose.root'.format(dir))
            files = [f for f in files if f != 'loose.root']
            merge_loose = True
        else:
            merge_loose = False

    merge_nominal = True
    if os.path.exists('{}/merged.root'.format(dir)):
        if args.remake_files:
            os.system('rm {}/merged.root'.format(dir))
            files = [f for f in files if f != 'merged.root']
            merge_nominal = True
        else:
            merge_nominal = False

    print('Processing {} with {} files.'.format(dir, len(files)))
    tight_files = [file for file in files if 'tight' in file]
    loose_files = [file for file in files if 'loose' in file]
    other_files = [file for file in files if file not in tight_files and file not in loose_files]
    
    missing_tight = expected_nfiles - len(tight_files)
    missing_loose = expected_nfiles - len(loose_files)
    missing_other = expected_nfiles - len(other_files)
    
    if 'nominal' in dir:
        missing_tight = 0
        missing_loose = 0
    elif 'acceptance' in dir:
        missing_tight = 0
        missing_loose = 0
    else:
        missing_other = 0 
        
    print('\tTight: {} files, Missing {}'.format( len(tight_files), missing_tight))
    print('\tLoose: {} files, Missing {}'.format( len(loose_files), missing_loose))
    print('\tOther: {} files, Missing {}'.format( len(other_files), missing_other))

    # Start to combine files if we're requested to do so.  
    if args.merge:
        print('\t\tStarting Merger Operation.')

        if ('nominal' in dir) and (missing_other == 0) and merge_nominal:
            print('Merging nominal files from {}'.format(dir))
            merge_command = 'hadd {}/merged.root {}/*.root'.format(dir, dir)
            os.system(merge_command)

        elif ('acceptance' in dir) and (missing_other == 0) and merge_nominal:
            print('Merging nominal files from {}'.format(dir))
            merge_command = 'hadd {}/merged.root {}/*.root'.format(dir, dir)
            os.system(merge_command)

        # Do the loose and tight if this isn't nominal or acceptance. 
        if 'nominal' not in dir and 'acceptance' not in dir:
            if (missing_tight == 0 and merge_tight):
                print('Merging nominal files from {}'.format(dir))
                merge_command = 'hadd {}/tight.root {}/*tight.root'.format(dir, dir)
                os.system(merge_command)

            if (missing_loose == 0 and merge_loose):
                print('Merging nominal files from {}'.format(dir))
                merge_command = 'hadd {}/loose.root {}/*loose.root'.format(dir, dir)
                os.system(merge_command)

if __name__ == '__main__':

    ap = argparse.ArgumentParser()
    ap.add_argument('-i', '--input_files',    required=True)
    ap.add_argument('-t', '--data_type',      required=True)
    ap.add_argument('-p', '--project_name',   required=True)
    ap.add_argument('-b', '--base_directory', required=True)
    ap.add_argument('-m', '--merge',          action='store_true')
    ap.add_argument('-r', '--remake_files',   action='store_true')
    args = ap.parse_args()

    expected_nfiles = find_number_of_files_in_list(args.input_files)
    working_path  = '{}/{}/{}'.format(args.base_directory, args.project_name, args.data_type)

    print('Project: {} expecting {} files.'.format(working_path, expected_nfiles))

    # Iterate on directories and process each one. 
    for dir, subdirs, files in os.walk(working_path):
        process_folder(dir, files, expected_nfiles, args)
