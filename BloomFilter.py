# coding=utf-8

import math
import hash

class BloomFilter(object):
    
    #初始化
    def __init__(self, n, fpr):     #n:给定存储元素个数 fpr:假阳性概率(false positive ratio)
        self.m = int(math.ceil((n * math.log(1 / fpr, 2)) / math.log(1.0 / (math.pow(math.log(2.0), 2.0))))) # 计算BF长度m
        self.k = int(round(math.log(2.0) * self.m / n)) #计算最优hash函数个数k
        self.vector = [0] * self.m      #开辟m长度的容器
        self.hashfunc = hash.RSHash     #Hash函数

    #插入key
    def insert(self, key):
        for i in range(self.k):     #产生k个Hash种子
            loc = self.hashfunc(key + str(i)) % self.m
            self.vector[loc] = 1    #对应Hash值置为1

    #判断key是否已存在
    def ifcontain(self, key):
        if not key:
            return False
        for i in range(self.k):     #产生k个Hash种子
            loc = self.hashfunc(key + str(i)) % self.m  
            if not self.vector[loc]:#该key未被插入
                return False
        return True
