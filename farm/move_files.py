#!/usr/bin/env python 

import argparse 
import os 

if __name__ == '__main__':

    ap = argparse.ArgumentParser()
    ap.add_argument('-p', '--project_name',   required=True)
    ap.add_argument('-b', '--base_directory', required=True)
    args = ap.parse_args()

    working_directory = '{}/{}/mc'.format(args.base_directory, args.project_name)
    for dir, subdir, files in os.walk(working_directory):
        target_directory = dir.replace('mc', 'data')
        files_to_move = ['{}/{}'.format(dir,file) for file in files if 'sidis' not in file]
        print(dir, target_directory, len(files), len(files_to_move))

        for file in files_to_move:
            os.system('mv {} {}'.format(' '.join(file), target_directory))
        
