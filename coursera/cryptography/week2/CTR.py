import base64
from Crypto import Random
from Crypto.Cipher import AES
from crypto_util import *

class AESCipher:
    def __init__(self, key):
        self.bs = 16
        if len(key) >= 16:
            self.key = key[:16]
        else:
            self.key = self._pad(key)

    def encrypt(self, raw):
        cipher = AES.new(self.key, AES.MODE_CTR, counter = lambda : os.urandom(AES.block_size))
        return base64.b64encode(cipher.encrypt(raw))

    def decrypt(self, enc):
        enc = base64.b64decode(enc)
        iv = enc[:AES.block_size]
        self.set_counter(iv)
        cipher = AES.new(self.key, AES.MODE_CTR, counter = self.counter)
        return cipher.decrypt(enc[AES.block_size:])

    def set_counter(self, c):
        self.count = int(str2hex(c), base=16) - 1

    def counter(self):
        self.count += 1
        return hex2str(hex(self.count)[2:-1])