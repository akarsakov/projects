import sys
from itertools import islice

input_file = sys.argv[1]

classMap = { '10': ["BLCA", "cancer"], '11': ["BLCA", "healthy"], 
             '20': ["BRCA", "cancer"], '21': ["BRCA", "healthy"], 
             '30': ["COAD", "cancer"], '31': ["COAD", "healthy"], 
             '40': ["HNSC", "cancer"], '41': ["HNSC", "healthy"], 
             '50': ["KIRC", "cancer"], '51': ["KIRC", "healthy"], 
             '60': ["KIRP", "cancer"], '61': ["KIRP", "healthy"], 
             '70': ["LIHC", "cancer"], '71': ["LIHC", "healthy"], 
             '80': ["LUAD", "cancer"], '81': ["LUAD", "healthy"], 
             '90': ["LUSC", "cancer"], '91': ["LUSC", "healthy"],
             '100': ["PRAD", "cancer"], '101': ["PRAD", "healthy"],
             '110': ["READ", "cancer"], '111': ["READ", "healthy"],
             '120': ["THCA", "cancer"], '121': ["THCA", "healthy"],
             '130': ["UCEC", "cancer"], '131': ["UCEC", "healthy"] }

with open(input_file) as file:
    while True:
        next_line = list(islice(file, 1))
        if not next_line:
            break
        values = next_line[0].split(',')
        cl = classMap[values[0]]
        with open(cl[0] + ".csv", "a") as out:
            out.write(cl[1])
            for index in range(2, len(values)):
                out.write(', ' + values[index])