import sys, os
import subprocess

def setup_analysis():
    os.system('cp -r /u/home/dmriser/clas/retro-sidis/mysidis/* .')

def get_systematics():
    return None 

def run(systematic_settings=None):
    pass 

def end_job():
    pass 

if __name__ == '__main__':
    ''' Run the analysis for a set of files 
    that has been provided.  
    '''

    # Copy the files to this node. 
    setup_analysis()

    # Obtain the set of systematic settings 
    systematic_settings = get_systematics()
    
    # Launch the job 
    run(systematic_settings)

    # Finish the actions for this job
    end_job()
