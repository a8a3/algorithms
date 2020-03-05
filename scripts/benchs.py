
#import numpy as np
#import matplotlib as plt
import json

# -----------------------------------------------
class tests_storage:
    regexp = r'^(.*?)<(.*)<.*/(\d*)'
    tests = []

    def create_test(raw_string):
        pass

# -----------------------------------------------
class test:
    def __init__(self, name):
        self.name = name
        self.targets = []

# -----------------------------------------------
class test_target:
    def __init__(self, name):
        self.name = name
        self.results = []

# -----------------------------------------------
class test_result:
    def __init__(self, value, time):
        self.value = value
        self.time = time

# todo main
with open('bench.json') as f:
    benchs = json.load(f)

for bench in benchs['benchmarks'] :
    print(bench['name'])