import pyCubbyFlow
import unittest

from AnimationTests import *
from PhysicsAnimationTests import *
from VectorTests import *

def main():
	pyCubbyFlow.Logging.Mute()
	unittest.main()

if __name__ == '__main__':
	main()