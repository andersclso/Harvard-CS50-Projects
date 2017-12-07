#Coverting Greedy in C to Python

import cs50

def main():
    while True:
        print ("How much change do you need back?: ", end="")
        dollar = cs50.get_float()
        if dollar > 0:
            break
        
    change = int(round(dollar * 100))
    
    q = (change//25)
    change %= 25
    
    d = (change//10)
    change %= 10
    
    n = (change//5)
    change %= 5
    
    p = (change//1)
    change %= 1
    
    print ("QUARTERS USED: {}" .format(q))
    print ("QUARTERS USED: {}" .format(d))
    print ("NICKLES USED: {}" .format(n))
    print ("PENNIES USED: {}" .format(p))
    print ("TOTAL COINS USED: {}" .format(q+d+n+p))
    
if __name__ == "__main__":
    main()