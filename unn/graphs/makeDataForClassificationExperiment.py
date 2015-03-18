import sys
from itertools import islice
from os import listdir, mkdir
from os.path import isfile, join
from random import shuffle

def chunker(seq, size):
    return (seq[pos:pos + size] for pos in xrange(0, len(seq), size))

data_path = sys.argv[1]
files = [ f for f in listdir(data_path) if isfile(join(data_path,f)) ]

f = files[0]

cancerIndexes = []
healthyIndexes = []
with open(join(data_path,f)) as file:
    index = 0
    while True:
        next_line = list(islice(file, 1))
        if not next_line:
            break
        
        if next_line[0].startswith("cancer"):
            cancerIndexes.append(index)
        else:
            healthyIndexes.append(index)
        index = index + 1

if len(healthyIndexes) < 50:
    numChunks = 2
else:
    numChunks = 4

shuffle(healthyIndexes)
print healthyIndexes
numHealthy = len(healthyIndexes)
chunks = list(chunker(healthyIndexes, numHealthy/numChunks))

work_dir = join(data_path, f[0:4])
mkdir(work_dir)

for chunkIdx in range(numChunks):
    output_dir = join(work_dir, "test_" + str(chunkIdx))
    mkdir(output_dir)
    
    train_file = open(join(output_dir, "train.csv"), "w")
    test_file = open(join(output_dir, "test.csv"), "w")
    test_labels_file = open(join(output_dir, "test_labels.csv"), "w")
    
    with open(join(data_path,f)) as file:
        index = 0
        while True:
            next_line = list(islice(file, 1))
            if not next_line:
                break
            data = next_line[0].split(", ")
            if (index in cancerIndexes) or not (index in chunks[chunkIdx]):
                test_file.write(", ".join(data[1:]))
                if (index in cancerIndexes):
                    test_labels_file.write("cancer\n")
                else:
                    test_labels_file.write("healthy\n")
            else:
                train_file.write(", ".join(data[1:]))
            
            index = index + 1
    
    train_file.close()
    test_file.close()
    test_labels_file.close()
    
    
print cancerIndexes
print healthyIndexes
