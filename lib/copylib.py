import os

f = open('module.log', 'r')

line = f.readline()
while line:
    path = line.split()[3]
    if(path == "[vdso]"):
        line = f.readline()
        continue
    os.system("cp " + path + " .")
    line = f.readline()
os.system("ls")