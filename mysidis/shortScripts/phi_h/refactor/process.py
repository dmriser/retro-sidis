#!/usr/bin/env python 

import os 

if __name__ == '__main__':


    if os.path.exists('Systematics_v2.root'):
        os.system('rm Systematics_v2.root')

    n_total_bins = 5 * 2 * 18 * 20 - 1
    print('Starting {}.'.format(n_total_bins))
    
    debug_jobs = None
    jobs_done = 0 
    for x_idx in range(0, 5):
        for qq_idx in range(0, 2):
            for z_idx in range(0, 18):
                for pt_idx in range(0, 20):
                    root_command = 'root -b -q \"processOneBinSystematics.cpp({},{},{},{})\"'.format(x_idx, qq_idx, z_idx, pt_idx)
                    
                    if x_idx != 4 and qq_idx != 1:
                        os.system(root_command)
                        jobs_done += 1 
                   
                        print('Done {}/{}'.format(jobs_done, n_total_bins))
     
                    if jobs_done == debug_jobs:
                        exit()
