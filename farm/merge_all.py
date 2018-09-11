#!/usr/bin/env python 

import argparse
import os 
import re 

def check_and_create_directory(directory_path):
    assert(type(directory_path) == str)
    
    if os.path.exists(directory_path):
        if os.path.isdir(directory_path):
            print('Directory {} already exists'.format(directory_path))
        else:
            raise ValueError('{} exists and is not a directory!'.format(directory_path))
    else:
        os.mkdir(directory_path)

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

if __name__ == '__main__':

    ap = argparse.ArgumentParser()
    ap.add_argument('-i', '--input_files',    required=True)
    ap.add_argument('-t', '--data_type',      required=True)
    ap.add_argument('-p', '--project_name',   required=True)
    ap.add_argument('-b', '--base_directory', required=True)
    args = ap.parse_args()

    expected_runs, missing_runs = load_expected_files_from_list(args.input_files) 
    working_path  = '{}/{}/{}'.format(args.base_directory, args.project_name, args.data_type)

    if len(missing_runs) > 0:
        print('Warning: Found {} missing runs.'.format(len(missing_runs)))

    print('Project: {} expecting {} runs.'.format(working_path, len(expected_runs)))

    missing_jobs = {}
    for dir, subdirs, files in os.walk(working_path):
        print('Processing {} with {} files.'.format(dir, len(files)))
        
        tight_files = [file for file in files if 'tight' in file]
        loose_files = [file for file in files if 'loose' in file]
        other_files = [file for file in files if file not in tight_files and file not in loose_files]

        missing_tight = len(expected_runs) - len(tight_files)
        missing_loose = len(expected_runs) - len(loose_files)
        missing_other = len(expected_runs) - len(other_files)

        if 'nominal' in dir:
            missing_tight = 0
            missing_loose = 0
        else:
            missing_other = 0 

        print('    Tight: {} files, Missing {}'.format( len(tight_files), missing_tight))
        print('    Loose: {} files, Missing {}'.format( len(loose_files), missing_loose))
        print('    Other: {} files, Missing {}'.format( len(other_files), missing_other))
        
        if missing_tight > 0:
            missing = [run for run in expected_runs if run not in map(extract_run_number, tight_files)]

        if missing_loose > 0:
            missing = [run for run in expected_runs if run not in map(extract_run_number, loose_files)]

        missing_jobs[dir] = missing 

        # Merge. 
        if ('nominal' in dir) and (missing_other == 0):
            print('Merging nominal files from {}'.format(dir))
            merge_command = 'hadd {}/merged.root {}/*.root'.format(dir, dir)
            os.system(merge_command)

        elif (missing_tight == 0 and missing_loose == 0):
            print('Merging tight files from {}'.format(dir))
            merge_command = 'hadd {}/tight.root {}/*tight.root'.format(dir, dir)
            os.system(merge_command)

            print('Merging loose files from {}'.format(dir))
            merge_command = 'hadd {}/loose.root {}/*loose.root'.format(dir, dir)
            os.system(merge_command)
