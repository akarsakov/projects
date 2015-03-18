import sys
from itertools import islice

if len(sys.argv) != 3:
	print "Error! Please specify path to metrics input file and path to file with significant features"
	exit()

input_file = sys.argv[1]
metrics_file = sys.argv[2]
healthy_class = 21
cancer_class = 20

feature_names_full = []
try:
	feature_names_file = open('feature_names.csv', 'r')
	for line in feature_names_file:
		feature_names_full.append(line.rstrip('"\n').lstrip('"'))
except Exception as e:
	print "Error! File feature_names.csv not found or have incorrect format"
	exit()

feature_indexes = []
feature_names = []
with open(metrics_file) as m_file:
	for line in m_file:
		feature_indexes.append(feature_names_full.index(line.rstrip('"\n').lstrip('"')))
		feature_names.append(line.rstrip('"\n').lstrip('"'))

if len(feature_indexes) == 0:
	print "Error! Wrong metrics file"
	exit()

out = open('out.csv', 'w')
# create header
out.write('class, ')
out.write(', '.join(feature_names))
out.write('\n')
# fill table
with open(input_file) as file:
	while True:
		next_line = list(islice(file, 1))
		if not next_line:
			break
		values = next_line[0].split(',')
		if int(values[0]) == cancer_class: 
			out.write('cancer')
			for index in feature_indexes:
				out.write(', ' + values[index + 2])
			out.write('\n')
		if int(values[0]) == healthy_class:
			out.write('healthy')
			for index in feature_indexes:
				out.write(', ' + values[index + 2])
			out.write('\n')

out.close()