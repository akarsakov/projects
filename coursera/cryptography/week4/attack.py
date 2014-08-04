import urllib2
import sys
import crypto_util
import string
import random

TARGET = 'http://crypto-class.appspot.com/po?er='
BLOCK_SIZE = 16
#--------------------------------------------------------------
# padding oracle
#--------------------------------------------------------------
class PaddingOracle(object):
    def query(self, q):
        target = TARGET + urllib2.quote(q)    # Create query URL
        req = urllib2.Request(target)         # Send HTTP request to server
        try:
            f = urllib2.urlopen(req)          # Wait for response
        except urllib2.HTTPError, e:          
            print "We got: %d" % e.code       # Print response code
            if e.code == 404:
                return True # good padding
            return False # bad padding
            
def int2hex(num):
    return hex2str(hex(self.count)[2:-1])

def attack(cyphertext):
    str_size = len(cyphertext)
    message = ''.join('0' for _ in range(str_size))
    num_blocks = str_size / BLOCK_SIZE
    for block in range(num_blocks-1, 0, -1):
        temp = message[:(block+1)*BLOCK_SIZE]
        for byte in range(BLOCK_SIZE):
            guess = 0
            while True:
                
    
    
    return hex2str(guess_hex)
    
    
    
if __name__ == "__main__":
    po = PaddingOracle()
    po.query(sys.argv[1])       # Issue HTTP query with the given argument