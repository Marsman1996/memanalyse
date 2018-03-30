f = open('instrace.log', 'r')
f2 = open('in_so.log', 'w')
f3 = open('out_so.log', 'w')

line = f.readline()
line = f.readline()
count = [0 for x in range(0, 6)]
while line:
    instr = line.split()[2]
    temp1 = line.split()[4]
    if(instr == "rep"):
        temp1 = line.split()[5]
    addr = int(temp1, 16)
    # print(addr)
    if   addr > int('0x08048000', 16) and addr < int('0x0804b000', 16):
        flag = 1
    elif addr > int('0xf752b000', 16) and addr < int('0xf7577000', 16):
        falg = 2
    elif addr > int('0xf7578000', 16) and addr < int('0xf759a000', 16):
        flag = 3
    elif addr > int('0xf759e000', 16) and addr < int('0xf759f000', 16):
        flag = 4
    elif addr > int('0xf75a0000', 16) and addr < int('0xf7755000', 16):
        flag = 5
    elif addr > int('0xf7111000', 16) and addr < int('0xf72c2000', 16):
        flag = 6
    else:
        flag = 0
    if(flag > 0):
        f2.write(line)
        w = line.split()[1]
    if(flag == 0 and addr < int('0xff000000', 16)):
        # print("%x" % (addr))
        f3.write(line)
    line = f.readline()
for x in count:
    print(x)