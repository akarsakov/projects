from Crypto.Hash import SHA256
from functools import partial

CHUNK_SIZE = 1024

def get_hash_from_file(filename):
    file = open(filename, "rb")
    blocks = []
    try:
        for chunk in iter(partial(file.read, CHUNK_SIZE), ''):
            blocks.append(chunk)
        prev_hash = SHA256.new(blocks[len(blocks)-1]).digest()
        for i in range(len(blocks)-1, 1, -1):
            block_with_hash = blocks[i-1] + prev_hash
            prev_hash = SHA256.new(block_with_hash).digest()
        result = SHA256.new(blocks[0] + prev_hash).hexdigest()
    finally:
        file.close()
    return result