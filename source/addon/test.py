import sys
# the mock-0.3.1 dir contains testcase.py, testutils.py & mock.py
sys.path.append("./intern/")
import convexify

def test():
    print("starting test.py")
    convexify.helloWorld()
    print("completed test.py")
