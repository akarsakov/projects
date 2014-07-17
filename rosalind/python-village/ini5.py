def print_even(text):
    return "\n".join([str for (num, str) in enumerate(text) if num%2==1])

if __name__ == '__main__':
    text = open("rosalind_ini5.txt", "rt").read().split('\n')
    print print_even(text)