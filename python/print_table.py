#!/usr/bin/env python 

import argparse 
import pandas as pd 

if __name__ == '__main__':

    ap = argparse.ArgumentParser()
    ap.add_argument(
        '-i',
        '--input_file',
        required=True
        )
    ap.add_argument(
        '-o',
        '--output_file',
        required=True
        )
    args = ap.parse_args()


    data = pd.read_csv(args.input_file)
    data.to_latex(args.output_file, index=False)
