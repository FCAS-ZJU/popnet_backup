import os
import random
os.system('rm -f bench')

num = int(raw_input("input the num of the trace: "))


bench="./bench"
file1=open(bench,"w")

i=0
time = 0.0004
while i<num:
  trace = str(time) + " "+str(random.randrange(16))+" "+str(random.randrange(16))+" "+str(random.randrange(16))+" "+str(random.randrange(16))+" "+str(random.randrange(16))+" "+str(random.randrange(16))+" "+str(5)+"\n"
  time = time + random.random()
  i = i+1
  file1.write(trace)

file1.close()
