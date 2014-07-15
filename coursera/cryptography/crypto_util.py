def xor_strings(xs, ys):
    return "".join(chr(ord(x) ^ ord(y)) for x, y in zip(xs, ys))

def str2hex(str):
    return str.encode('hex')

def hex2str(h):
    return h.decode('hex')
