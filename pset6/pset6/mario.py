#Converting Mario in C to Python

import cs50

def main():
    while True:
        print ("ITSAMEE MAAARIO! How high do you wantamee to jump?: ", end="")
        height = cs50.get_int()
        if height > -1 and height < 24:
            break
        
    for rows in range(height):
        print (" " *(height-rows-1), end="")
        print ("#" *(rows+2))
        
if __name__ == "__main__":
    main()