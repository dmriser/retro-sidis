import json 

config = {}
config['base_path'] = '.'
config['project_name'] = 'TestProject'
config['variation'] = 'nominal'
config['job_id'] = 9
config['data_type'] = 'data'
config['change_strictness'] = 0
config['strict_to_change'] = 0
config['strictness'] = 0
config['correct_electron_p'] = 1
config['correct_pion_p'] = 1
config['acceptance_iteration'] = 0
config['files'] = ['/volatile/clas12/dmriser/data/all/root/37748.root',
                   '/volatile/clas12/dmriser/data/all/root/37750.root',
                   '/volatile/clas12/dmriser/data/all/root/37753.root',
                   '/volatile/clas12/dmriser/data/all/root/37762.root',
                   '/volatile/clas12/dmriser/data/all/root/37763.root',
                   '/volatile/clas12/dmriser/data/all/root/37766.root',
                   '/volatile/clas12/dmriser/data/all/root/37767.root',
                   '/volatile/clas12/dmriser/data/all/root/37769.root',
                   '/volatile/clas12/dmriser/data/all/root/37770.root']

with open('config.json', 'w') as outfile:
    json.dump(config, outfile, indent=4)
