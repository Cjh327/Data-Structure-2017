# coding=utf-8

def RSHash(str):
    b = 378551
    a = 63689
    hash = 0
    for ch in str:
        hash = hash * a + ord(ch)
        a = a * b
    return hash

def JSHash(str):
    hash = 1315423911
    for ch in str:
        hash ^= ((hash << 5) + ch + (hash >> 2));
    return hash

def JSHash(str):
    BitsInUnsignedInt = 4 * 8
    ThreeQuarters = (BitsInUnsignedInt * 3) / 4
    OneEighth = BitsInUnsignedInt / 8
    HighBits = 0xFFFFFFFF << (BitsInUnsignedInt - OneEighth)
    hash = 0
    test = 0
    for ch in str:
        hash = (hash << OneEighth) + ch
        test = hash & HighBits
        if test:
            hash = (hash ^ (test >> ThreeQuarters)) & (~HighBits)
    return hash
