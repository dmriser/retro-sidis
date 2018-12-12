#!/usr/bin/env python

import numpy as np 
import pandas as pd

def process(sys_file, sector_file, output_file):
    ''' Take in the two files and merge them into
    the output file. ''' 
    df = pd.read_csv(sys_file)
    df_sect = pd.read_csv(sector_file)
    df_merge = pd.merge(
        left = df, 
        right = df_sect, 
        how = 'inner',
        on = ['x_bin', 'q2_bin', 'z_bin', 'pt2_bin']
    )
    df_merge.to_csv(output_file)

if __name__ == '__main__':
    

    process(
        sys_file = 'pip_systematics.csv',
        sector_file = 'pip_sector_systematics.csv',
        output_file = 'pip_total_systemtatics.csv'
    )
    process(
        sys_file = 'pim_systematics.csv',
        sector_file = 'pim_sector_systematics.csv',
        output_file = 'pim_total_systemtatics.csv'
    )
