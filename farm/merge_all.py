#!/usr/bin/env python 

import argparse
import os 
import re 

def find_number_of_files_in_list(input_files):
    nfiles = 0
    with open(input_files, 'r') as input_file:
        files = [line.strip() for line in input_file.readlines()]

        for file in files:
            if os.path.exists(file):
                nfiles += 1
    return nfiles 

def process_folder(dir, files, expected_nfiles, args):

    print('Processing {} with {} files.'.format(dir, len(files)))

    folder_type = determine_folder_type(dir)
    if folder_type == 'variation':
        process_variation(dir, files, expected_nfiles, args)
    elif folder_type == 'nominal':
        process_nominal(dir, files, expected_nfiles, args)
    elif folder_type == 'acceptance':
        process_nominal(dir, files, expected_nfiles, args)
    elif folder_type == 'sector':
        process_sector(dir, files, expected_nfiles, args)
    else:
        print('Unknown folder type {}'.format(folder_type))

def determine_folder_type(dir):
    default_type = 'variation'
    special_types = ['nominal', 'acceptance', 'sector']

    for special_type in special_types:
        if special_type in dir:
            return special_type

    return default_type


def clean_and_merge(filename, dir, files, expected_nfiles, args):
    ''' This function does the brunt of the work.  Taking a stub name and 
    cleaning existing files if needed and merging the sub files. '''

    merge_status = True
    if os.path.exists('{}/{}.root'.format(dir, filename)):
        if args.remake_files:
            os.system('rm {}/{}.root'.format(dir, filename))
            files = [f for f in files if f != '{}.root'.format(filename)]
            merge_status = True
        else:
            merge_status = False
                    
    discovered_files = [file for file in files if filename in file]
    n_missing_files = expected_nfiles - len(discovered_files)
    print('\tVariation: {}, Files: {}/{}'.format(filename, len(discovered_files), expected_nfiles))

    if args.merge:
        print('\t\tStarting Merger Operation.')

        if (n_missing_files == 0 and merge_status):
            print('Merging files from {}'.format(dir))
            merge_command = 'hadd {}/{}.root {}/*{}.root'.format(dir, filename, dir, filename)
            os.system(merge_command)

def process_variation(dir, files, expected_nfiles, args):
    clean_and_merge(filename='tight', dir=dir, files=files, 
        expected_nfiles=expected_nfiles, args=args
    )
    clean_and_merge(filename='loose', dir=dir, files=files, 
        expected_nfiles=expected_nfiles, args=args
    )
        
def process_nominal(dir, files, expected_nfiles, args):
    clean_and_merge(filename='out', dir=dir, files=files, 
        expected_nfiles=expected_nfiles, args=args
    )

def process_sector(dir, files, expected_nfiles, args):
    clean_and_merge(filename='sector1', dir=dir, files=files, 
        expected_nfiles=expected_nfiles, args=args
    )
    clean_and_merge(filename='sector2', dir=dir, files=files, 
        expected_nfiles=expected_nfiles, args=args
    )
    clean_and_merge(filename='sector3', dir=dir, files=files, 
        expected_nfiles=expected_nfiles, args=args
    )
    clean_and_merge(filename='sector4', dir=dir, files=files, 
        expected_nfiles=expected_nfiles, args=args
    )
    clean_and_merge(filename='sector5', dir=dir, files=files, 
        expected_nfiles=expected_nfiles, args=args
    )
    clean_and_merge(filename='sector6', dir=dir, files=files, 
        expected_nfiles=expected_nfiles, args=args
    )

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
    working_path = '{}/{}/{}'.format(args.base_directory, args.project_name, args.data_type)

    print('Project: {} expecting {} files.'.format(working_path, expected_nfiles))

    # Iterate on directories and process each one. 
    for dir, subdirs, files in os.walk(working_path):
        process_folder(dir, files, expected_nfiles, args)
