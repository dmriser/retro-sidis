import sys, os
import subprocess

def setup_files():
    print('starting extraction of sidis package')
    subprocess.call('tar -xvzf mysidis.tar', shell=True)

def run_code():
    print('running code')
    pass

def cleanup():
    print('cleaning project')
    subprocess.call('rm -rf mysidis/', shell=True)

if __name__ == '__main__':
    setup_files() 
    run_code()
#    cleanup()
