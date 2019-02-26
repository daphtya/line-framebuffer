import os

path = os.getcwd()
filenames = os.listdir(path)

count = 0
scale = 8
changedfiles = []
# print(filenames)

try:
	for filename in filenames:
		if filename.split('.')[-1] == "lines":
			with open(filename, "r") as file:
				filecontent = [[int(x)*scale for x in line.split()] for line in file.read().split('\n')]
			with open(filename, "w") as file:
				file.write("\n".join([" ".join([str(x) for x in line])for line in filecontent]))
			count += 1
			changedfiles.append(filename)
except:
	print("oh no, an error")
finally:
	print(count, changedfiles)