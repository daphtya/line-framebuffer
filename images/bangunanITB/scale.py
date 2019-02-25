import os

path = os.getcwd()
filenames = os.listdir(path)

count = 0
scale = 8
changedfiles = []
# print(filenames)

try:
	for filename in filenames:
		if filename.split('.')[-1] == "point":
			with open(filename, "r") as file:
				filecontent = [[int(line.split()[0])*scale, int(line.split()[1])*scale] for line in file.read().split('\n')]
			with open(filename, "w") as file:
				file.write("\n".join([ str(line[0])+" "+str(line[1]) for line in filecontent]))
			count += 1
			changedfiles.append(filename)
except:
	print("oh no, an error")
finally:
	print(count, changedfiles)