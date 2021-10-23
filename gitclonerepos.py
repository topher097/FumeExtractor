import subprocess
import sys
import os
from pathlib import Path
import ast

def install(package):
    subprocess.check_call([sys.executable, "-m", "pip", "install", "--user", package])

# Upgrade pip
subprocess.check_call([sys.executable, "-m", "pip", "install", "--upgrade", "--user", 'pip'])
# Install packages
packages = ['GitPython', 'json5', 'demjson']
for package in packages:
    install(package)


from git import Repo
import json5 as json
import demjson

with open('repos.txt', 'r') as f:
    repos = [line.strip() for line in f]

cloneDir = os.path.join(os.getcwd(), 'lib\\')
cpp_propPath = os.path.join(os.getcwd(), '.vscode\c_cpp_properties.json')
with open(cpp_propPath, 'r') as f:
    data = json.load(f)
    includeStrs = dict(data['configurations'][0])['includePath']

for repo in repos:
    # Clone repository
    clonePath = os.path.join(cloneDir, os.path.splitext(os.path.basename(repo))[0])
    if not os.path.isdir(clonePath):
        try:
            Repo.clone_from(repo, clonePath)
        except Exception as e:
            print(e)
    
    # Add path to c_cpp_properties.json
    # includeStr = "${workspaceFolder}/lib/" + Path(clonePath).stem
    # if not includeStr in includeStrs:
    #     includeStrs.append(includeStr)
    #     print(f"added {includeStr} to configuration file")

# # Update includPath in dictionary
# data['configurations'][0]['includePath'] = includeStrs
# data = f"{data}"
# data = ast.literal_eval(data)
# print(data)

# with open(cpp_propPath, 'w') as f:
#     json.dump(data, f, indent=4, sort_keys=False)
#     print(f"Saved updated c_cpp_properties.json file")
    
print(f'Cloned {len(repos)} repositories to {cloneDir}')