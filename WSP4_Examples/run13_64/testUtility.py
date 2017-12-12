#!/usr/bin/env python
# coding=utf-8

import os
import sys
import time

from ctypes import *

util = WinDLL('Utility_4_0.dll')

util.ReportError(c_wchar_p("Error: Could not process."), c_int(0), c_bool(True))
