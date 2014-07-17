def slice(string, ranges):
    zipped = zip(ranges[0::2], ranges[1::2])
    return " ".join([string[s:e+1] for (s,e) in zipped])

if __name__ == '__main__':
    values = open("rosalind_ini3.txt", "rt").read().split()
    str = values[0]
    ranges = [int(x) for x in values[1:]]
    print slice(str, ranges)