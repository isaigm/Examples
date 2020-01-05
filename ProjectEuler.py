import time
import math
def rational_sum(a, b, c, d):
    numer = a*d + b*c
    den = b*d
    return numer, den
def rational_div(a, b, c, d):
    numer = a*d
    den = b*c
    return numer, den
def P22():
    fileName = "names.txt"
    arch = open(fileName)
    line = arch.readlines()
    words = line[0].split(',')
    words = [word.replace('"', '') for word in words]
    words = sorted(words) 
    score = 0
    for i in range(len(words)):
        summ = 0
        for letter in words[i]:
            summ += (ord(letter) - 64)
        score += ((i+1)*summ)
    print(score)

def P42():
    arch = open("p042_words.txt")
    line = arch.readlines()
    words = line[0].split(',')
    words = [word.replace('"', '') for word in words]
    count = 0
    for w in words:
        k = 0
        for wi in w:
            k += ord(wi) - 64
        x0 = (-1 + math.sqrt(1+8*k))/2
        if x0 > 0 and (x0 - int(x0)) == 0:
            count += 1
    print(count)
def P36():
    def to_binary(k):
        res = ''
        while k >= 1:
            res += str(k%2)
            k //= 2
        res = res[::-1]
        return res
    summ = 0
    for i in range(1000000):
        dec_str = str(i)
        bin_str = to_binary(i)
        if dec_str == dec_str[::-1] and bin_str == bin_str[::-1]:
            print("%s %s\n" % (dec_str, bin_str))
            summ += i
    print(summ)
    
def P57():
    def exp(depth):
        if depth >= 1:
            num, der = exp(depth-1)
            num1, der1 = rational_sum(2, 1, num, der)
            num2, der2 =  rational_div(1, 1, num1, der1)
            return num2, der2
        return 0, 1
    def count_digits(k):
        count = 0
        while k > 0:
            k //= 10
            count += 1
        return count
    count = 0
    for i in range(1, 1000):    
        num, den = exp(i)
        num, den = rational_sum(1, 1, num, den)
        if count_digits(num) > count_digits(den):
            count += 1
    print(count)
def P55():
    def is_Lychrel(k):
        for _ in range(50):
            next_n = k + int(str(k)[::-1])
            k = next_n
            if k == int(str(k)[::-1]):
                return False
        return True
    count = 0
    for i in range(10000):
        if(is_Lychrel(i)):
            count += 1
    print(count)
def P92():
    def sum_digits(k):
        summ = 0
        while k > 0:
            summ += (k % 10)**2
            k //= 10
        return summ
    def _1_or_89(k):
        while True:
            k = sum_digits(k)
            if k == 89:
                return True
            elif k == 1:
                return False
    count = 0
    for i in range(1, 10000000):
        if(_1_or_89(i)):
            count += 1
    print(count)
def P69():
    """
    def primes(n):
        lista = [True for _ in range(n)]
        for i in range(2, int(math.sqrt(n))+1):
            l =  n//i if (n/i - n//i) == 0  else n//i+1
            if lista[i]:
                for j in range(2, l):
                    lista[i*j] = False
        only_primes = []
        for i in range(2, n):
            if lista[i]:
                only_primes.append(i)
        return only_primes
    primes_list = primes(1000000)
    """
    arch = open("primes.txt")
    lines = arch.readlines()
    primes_list = [int(x) for x in lines]
    def factors(n):
        idx = 0
        prime_factors = {}
        while n > 1:
            if n % primes_list[idx] == 0:
                n //= primes_list[idx]
                try:
                    prime_factors[primes_list[idx]] += 1
                except:
                    prime_factors[primes_list[idx]] = 1
            else : idx += 1
        return prime_factors
    def phi_n(prime_factors):
        for p in prime_factors:
            k = p - 1
            exp = prime_factors[p]
            break
        if len(prime_factors) == 1 and exp == 1:
            return k
        else:
            product = 1
            for p in prime_factors:
                product *= (p-1)*(p**(prime_factors[p]-1))
            return product
    maxx = 0
    k = 0
    for i in range(2, 1000000):
        prime_factors = factors(i)
        phi_i = phi_n(prime_factors)
        divv = float(i)/float(phi_i)
        if maxx < divv:
            maxx = divv
            k = i
    print(k)
def P65():
    summ = [1, 2]
    c = 0
    k = 4
    for i in range(2, 100):
        if c < 2:
            summ.append(1)
            c += 1
        else:
            summ.append(k)
            c = 0
            k += 2
    def exp(depth, c):
        if depth > 1:
            num, der = exp(depth-1, c+1)
            num1, der1 = rational_sum(summ[c], 1, num, der)
            num2, der2 =  rational_div(1, 1, num1, der1)
            return num2, der2
        return 0, 1
    num, der = exp(100, 0)
    num, der = rational_sum(2, 1, num, der)
    summ = 0
    while num > 0:
        summ += num % 10
        num //= 10
    print(summ)
def P80():
    with open("irrationals.txt") as arch:
        lines = arch.readlines()
        lines = [line.replace('.', '') for line in lines]
        summ = 0
        for line in lines:
            for i in range(100):
                summ += int(line[i])
            break
        print(summ)
       
def main():
    start_time = time.time()
    P80()
    end_time = time.time()
    print(end_time - start_time)
    
if __name__ == '__main__':
    main()
