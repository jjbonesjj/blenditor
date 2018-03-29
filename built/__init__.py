import sys
# the mock-0.3.1 dir contains testcase.py, testutils.py & mock.py
sys.path.append("./intern/");
import os;
dirPath = os.path.dirname(os.path.realpath(__file__));
sys.path.append(dirPath);
from ctypes import *;

# need to both import AND use ctypes.CDLL? dafuq?
import convexify;
# convexify = CDLL("convexify.pyd");

def test():
    print("starting test.py")
    convexify.test();
    print("completed test.py")
