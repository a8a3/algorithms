
import json
import matplotlib.pyplot as plt
import re


# -----------------------------------------------
class TestsStorage:
    regexp = r'^(.*?)<(.*)<.*>/(\d*)'
    tests = {}

    @staticmethod
    def create_test(raw_name, raw_time):
        test = re.search(TestsStorage.regexp, raw_name)
        test_name = test.group(1)
        target_name = test.group(2)
        test_value = test.group(3)

        tr = Result(test_value, raw_time)

        if test_name not in TestsStorage.tests:
            TestsStorage.tests[test_name] = Test(test_name)

        if target_name not in TestsStorage.tests[test_name].targets:
            TestsStorage.tests[test_name].targets[target_name] = Target(target_name)

        TestsStorage.tests[test_name].targets[target_name].results.append(tr)


# -----------------------------------------------
class Test:
    def __init__(self, name):
        self.name = name
        self.targets = {}


# -----------------------------------------------
class Target:
    def __init__(self, name):
        self.name = name
        self.results = []


# -----------------------------------------------
class Result:
    def __init__(self, value, time):
        self.value = int(value)
        self.time = int(time)


# -----------------------------------------------
def plot_test(test):
    fig = plt.figure()
    plt.grid(True)
    plt.title(test.name)
    plt.xlabel("elements")
    plt.ylabel("nanoseconds")

    colors = ["green", "palegreen", "royalblue", "plum"]

    for idx, target in enumerate(test.targets.values()):
        x = []
        y = []
        for result in target.results:
            x.append(result.value)
            y.append(result.time)

        plt.plot(x, y, color=colors[idx], label=target.name)

    plt.legend()
    plt.savefig(test.name + ".png")
    plt.show()


# -----------------------------------------------
def main():
    with open('benchs.json') as f:
        benchs = json.load(f)

    for bench in benchs['benchmarks']:
        TestsStorage.create_test(bench['name'], bench['real_time'])

    # for test in TestsStorage.tests.values():
    #     print("test: " + test.name)
    #     for target in test.targets.values():
    #         print("target: " + target.name)
    #         for test_result in target.results:
    #             print("result :" + str(test_result.value) + "/" + str(test_result.time))


    for idx, test in enumerate(TestsStorage.tests.values()):
        plot_test(test)




if __name__ == "__main__":
    main()
