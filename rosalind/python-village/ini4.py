def sum_odd(start, end):
    return sum([x for x in range(start, end+1) if x%2==1])

if __name__ == '__main__':
    values = open("rosalind_ini4.txt", "rt").read().split()
    print sum_odd(int(values[0]), int(values[1]))