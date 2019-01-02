#!/usr/bin/env python 

'''

File:    write_5d_systematic_error_table.py 

Purpose: This script loads the different variational files for data and monte carlo and 
         uses Nathan's methods from cpp to combine the different variations and assign 
         systematic errors to each bin.

Date:    Dec. 18, 2018
Author:  David Riser 

'''


import argparse as ap 

if __name__ == '__main__':

    arg_parser = ap.ArgumentParser()
    arg_parser.add_argument(
        '-i', 
        '--input_directory', 
        required = True
    )

    args = arg_parser.parse_args()

    print('Loading files from directory: {}'.format(args.input_directory))
