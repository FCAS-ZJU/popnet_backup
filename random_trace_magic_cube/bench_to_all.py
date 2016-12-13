import os
os.system('rm -rf bench.*')

bench="./bench"
file1=open(bench,"r")

while 1:
	trace = file1.readline()
	if file:
		trace_list = trace.split()
		if len(trace_list) < 5:
			break
		ben_star = bench + "."+trace_list[1]+"."+trace_list[2]+"."+trace_list[3]
		file2 = open(ben_star,"a")
		file2.write(trace)
		file2.close()
	else: break
file1.close()