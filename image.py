# coding=utf-8
import urllib
from pip._vendor import requests
import urllib2    
import re
import BloomFilter
import math
import hash
import threading
import os
import sys

reload(sys)
sys.setdefaultencoding('utf-8')

MaxUrlNum = 2500
MaxImgNum = 10000
UrlNum = 0
ImgNum = 0
JpgNum = 0
BmpNum = 0
PngNum = 0

bf = BloomFilter.BloomFilter(MaxUrlNum, 0.0001)

class Crawler(object):
    #初始化
    def __init__(self, startUrl):
        self.startUrl = startUrl    #初始网页
        self.depth = 0

    #获取网页url中的内容
    def GetContent(self, url):
        if url.find('http') == -1:
            url = 'https:' + url
        try:
            response = urllib2.urlopen(url,timeout=3)
        except:
            return None
        else:
            try:
                content = response.read() 
            except:
                return None
            else:
                return content  

    #实际运行时发现某些包含特定字符串的网页访问
    #时较慢且内容中不包含网页和图片，将它们屏蔽
    def BlackList(self, url):
        if url.find('.css') != -1:
            return True
        if url.find('.liangle') != -1:
            return True
        if url.find('.ico') != -1:
            return True
        if url.find('.apk') != -1:
            return True
        if url.find('//assets') != -1:
            return True
        if url.find('apple.com') != -1:
            return True
        if url.find('//t1.') != -1:
            return True
        if url.find('//t2.') != -1:
            return True
        if url.find('//t3.') != -1:
            return True
        return False

    #从网页内容中匹配网址链接
    def GetUrls(self, content):
        pattern_url = r"(?<=href=\").+?(?=\")|(?<=href=\').+?(?=\')"  #匹配网址链接
        links = re.findall(pattern_url, content, re.I | re.S | re.M)
        return links

    #从网页内容中匹配图片链接
    def GetImg(self, content):
        global UrlNum
        global ImgNum
        global JpgNum
        global PngNum
        global BmpNum

        pattern_jpg = r'src="(.*?\.jpg)" alt'       #匹配jpg图片
        jpglist = re.findall(pattern_jpg, content)
        self.SaveImg(jpglist, 0)

        pattern_png = r'src="(.*?\.png)" alt'       #匹配png图片
        pnglist = re.findall(pattern_png, content)
        self.SaveImg(pnglist, 1)

        pattern_bmp = r'src="(.*?\.bmp)" alt'       #匹配bmp图片
        bmplist = re.findall(pattern_bmp, content)
        self.SaveImg(bmplist, 2)

    #保存图片
    def SaveImg(self, imglist, type):
        global UrlNum
        global ImgNum
        global JpgNum
        global BmpNum
        global PngNum

        suffix = None
        dir = None
        num = 0

        if type == 0:                                   #jpg格式
            dir = 'D:/image/jpg/'
            suffix = '.jpg'
            num = JpgNum
            if not os.path.exists(r'D:/image/jpg'):
                os.makedirs(r'D:/image/jpg')
        elif type == 1:                                 #png格式
            dir = 'D:/image/png/'
            suffix = '.png'
            num = PngNum
            if not os.path.exists(r'D:/image/png'):
                os.makedirs(r'D:/image/png')
        elif type == 2:                                 #bmp格式
            dir = 'D:/image/bmp/'
            suffix = '.bmp' 
            num = BmpNum
            if not os.path.exists(r'D:/image/bmp'):
                os.makedirs(r'D:/image/bmp')

        for url in imglist:
            path = dir + str(num) + suffix
            f = open(path, "wb")                        #打开图片
            if url.find('http') == -1:
                url = 'https:' + url
            try:
                req = urllib2.urlopen(url, None, timeout = 3)
                buf = req.read()                        #读出图片
            except:
                f.close()
                try:
                    os.remove(path)
                except:
                    continue
                else:
                    continue
            else:
                print 'img', ImgNum, url  
                f.write(buf)                            #写入图片
                f.close()
                ImgNum = ImgNum + 1                     #总图片数量加一
                num = num + 1
                if type == 0:                           #对应格式图片数量加一
                    JpgNum = JpgNum + 1
                elif type == 1:
                    PngNum = PngNum + 1
                elif type == 2:
                    BmpNum = BmpNum + 1

    #爬取网页       
    def crawl(self, url):
        global UrlNum
        global ImgNum
        if self.depth >= 10:
            return
        if (UrlNum >= MaxUrlNum) & (ImgNum >= MaxImgNum):   #网页和图片数量已达到要求，返回
            return
        if url.find('http') == -1:          #填补https:
            url = 'https:' + url
        if self.BlackList(url):             #屏蔽某些特定网页，返回
            return
        if bf.ifcontain(url):               #已经访问过，返回
            return
        bf.insert(url)                      #未访问过，在布隆过滤器中插入该网址
        content = self.GetContent(url)
        if not content:                     #获取网页内容失败，返回
            return
        print 'url', UrlNum, url
        UrlNum = UrlNum + 1                 #已爬取网页数量加一
        self.GetImg(content)                #获取图片
        links = self.GetUrls(content)       #获取网页
        for link in links:                  #深度优先，递归调用
            self.depth = self.depth + 1
            self.crawl(link)
            self.depth = self.depth - 1

    #已封装好的对外接口
    def Crawl(self):
        url = self.startUrl
        self.crawl(url)



crawler1 = Crawler('http://www.sina.com.cn/')
crawler2 = Crawler('http://www.163.com/')
crawler3 = Crawler('http://www.sohu.com/')
crawler4 = Crawler('http://www.nflchina.com/')
crawler5 = Crawler('http://news.ifeng.com/')
crawler6 = Crawler('https://www.hupu.com/')

threading.Thread(target=crawler1.Crawl).start()
threading.Thread(target=crawler2.Crawl).start()
threading.Thread(target=crawler3.Crawl).start()
threading.Thread(target=crawler4.Crawl).start()
threading.Thread(target=crawler5.Crawl).start()
threading.Thread(target=crawler6.Crawl).start()
