#!/usr/bin/env python
# coding=utf-8

import os
import sys
import time

from ctypes import *

util = WinDLL('Utility_4_0.dll')

#English output, cannot use c_char_p
util.ReportError(c_wchar_p("Error: Could not process."), c_int(0), c_bool(True))

#Chinese output in cp936 cmd box
util.ReportError(c_wchar_p(u"无法处理abc"), c_int(0), c_bool(True))