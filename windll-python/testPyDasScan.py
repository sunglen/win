#!/usr/bin/env python
# coding=utf-8

import os
import sys
import time

from ctypes import *

os.chdir('C:\\Program Files\\test\\')

#There are 2 ways for string buffer

#d="\x00"*512
d=create_string_buffer(512)

#Current Directory will be changed if chdir
windll.kernel32.GetCurrentDirectoryA(c_int(sizeof(d)), d)
#print d.strip('\x00')
print d[:]

f="\x00"*512

#Module File Name will be always C:\Python27\python.exe if we run it by python it.py
windll.kernel32.GetModuleFileNameA(None, c_char_p(f), c_int(512))
print f.strip('\x00')

ds = WinDLL('DasScanSimu.dll')

ds.ScanInit()

ds.LoadProtocol(c_char_p("proto1.xml"))

ds.ScanGo()

#Should use kernel32.SleepEx other than time.sleep to wake by windows timer
while not ds.IsScanFinished():
    windll.kernel32.SleepEx(c_long(1000), True)
