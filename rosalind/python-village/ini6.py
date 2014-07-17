def print_frequency_dictionary(text):
    dict = { }
    for word in text.split(" "):
        dict[word] = dict.get(word, 0) + 1
    return "\n".join([word + " " + str(fr) for word, fr in dict.items()])

if __name__ == '__main__':
    text = open("rosalind_ini6.txt", "rt").read().strip()
    print print_frequency_dictionary(text)