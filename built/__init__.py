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
    convexify.helloWorld();
    print("completed test.py")
    print("starting test2.py")
    convexify.helloArgs([1, 5, 3, 4, 7]);
    print("completed test2.py")
