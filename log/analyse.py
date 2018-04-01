f = open('instrace.log', 'r')
f2 = open('in_so.log', 'w')
f3 = open('out_so.log', 'w')
f4 = open('alloc_mem.log', 'w')

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
    elif addr > int('0x8004a000', 16) and addr < int('0x801ff000', 16):
        falg = 2
    elif addr > int('0xb7705000', 16) and addr < int('0xb7750000', 16):
        flag = 3
    elif addr > int('0xb7751000', 16) and addr < int('0xb7773000', 16):
        flag = 4
    elif addr > int('0xb7776000', 16) and addr < int('0xb7777000', 16):
        flag = 5
    elif addr > int('0xb72f6000', 16) and addr < int('0xb74a4000', 16):
        flag = 6
    else:
        flag = 0
    if(flag > 0):
        f2.write(line)
        w = line.split()[1]
    if(flag == 0 and addr < int('0xbf000000', 16)):
        # print("%x" % (addr))
        f3.write(line)
        if(addr > int('0x0804b000', 16) and addr < int('0xb0000000', 16)):
            f4.write(line)
    line = f.readline()
for x in count:
    print(x)