
import json

with open('benchs.json') as json_file:
    data = json.load(json_file)

benchmarks = data['benchmarks']

for benchmark in benchmarks:
    print(benchmark['name'])
