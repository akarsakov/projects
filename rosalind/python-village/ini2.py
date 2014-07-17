def hyp_squared(values):
    return sum(map(lambda x: int(x) ** 2, values))

if __name__ == '__main__':
    values = open("rosalind_ini2.txt", "rt").read().split()
    print hyp_squared(values)