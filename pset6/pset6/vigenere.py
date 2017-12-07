#Converting Vigenere in C to Python

import cs50
import sys

def main():
    
    key = []
    j = 0
    i = 0
    
    if len(sys.argv) != 2:
        print ("error1")
        exit(1)
    
    keyword = sys.argv[1]
    if str.isalpha(keyword) == False:
        print ("error2")
        exit(1)
        
    print ("plaintext: ", end="")
    msg = cs50.get_string()
    
    for c in keyword:
        if c.isupper():
            i = (ord(c) - 65)
            key.append(i)
        if c.islower():
            i = (ord(c) - 97)
            key.append(i)
            
    print ("ciphertext: ", end="")
    
    k = 0
    
    for d in msg:
        if d.isupper():
            j = (((ord(d) - 65) + (key[k%len(keyword)]))%26) + 65
            print (chr(j), end="")
            k += 1
        if d.islower():
            j = (((ord(d) - 97) + (key[k%len(keyword)]))%26) + 97
            print (chr(j), end="")
            k += 1
        else:
            print (d, end="")
            
    print ("\n")
            
if __name__ == "__main__":
    main()