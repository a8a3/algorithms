
#import numpy as np
#import matplotlib as plt
import json
import re


# -----------------------------------------------
class TestsStorage:
    regexp = r'^(.*?)<(.*)<.*>/(\d*)'
    tests = {}

    @staticmethod
    def create_test(raw_name, raw_time):
        test = re.search(TestsStorage.regexp, raw_name)
        print(test.group(1) + "->" + test.group(2) + "->" + test.group(3))
        test_name = test.group(1);
        target_name = test.group(2)
        test_value = test.group(3)
        TestsStorage.tests[test_name].targets[target_name] = TestResult(test_value, raw_time)


# -----------------------------------------------
class Test:
    def __init__(self, name):
        # self.name = name
        self.targets = {}


# -----------------------------------------------
class TestTarget:
    def __init__(self, name):
        # self.name = name
        self.results = []


# -----------------------------------------------
class TestResult:
    def __init__(self, value, time):
        self.value = value
        self.time = time


# -----------------------------------------------
def main():
    with open('benchs.json') as f:
        benchs = json.load(f)

    for bench in benchs['benchmarks']:
        TestsStorage.create_test(bench['name'], bench['real_time'])


if __name__ == "__main__":
    main()
