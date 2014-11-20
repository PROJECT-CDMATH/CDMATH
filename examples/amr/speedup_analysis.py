#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json
import glob

import numpy
import matplotlib.pyplot as plt; plt.rcdefaults()
import matplotlib.pyplot as plt

     
def main():
    """This Python script reads JSON files to make the analysis
    of the speedup due to OpenMP."""
    json_list = glob.glob("""* timedata.json""") # Notice the space before.
    # This requires to have formatted the JSON files a little.
    timespent_archive = []
    # Structure of this list:
    # [[6543.5, 35.4, 3.5435], [2.65]]
    # Only 1 calculation was made with 2 threads and it took 2.65s.
    for i in range(len(json_list)):
        json_file_name = json_list[i]
        with open(json_file_name, 'r') as jsonfile:
            data = json.load(jsonfile)
        timedata = data["timedata"]
        if len(timedata) > len(timespent_archive):
            for j in range(len(timedata) - len(timespent_archive)):
                timespent_archive.append([])
        for j in range(len(timedata)):
            timespent_archive[j].append(timedata[j]["exec_time"])
    print(timespent_archive)
    
    nb_threads = numpy.arange(len(timespent_archive)) + 1
    timespent_global_sum = range(len(timespent_archive))
    timespent_global = range(len(timespent_archive))
    speedup_global = range(len(timespent_archive))
    max_speedup = 1.
    for i in range(len(timespent_archive)):
        timespent_global_sum[i] = 0.
        for j in range(len(timespent_archive[i])):
            timespent_global_sum[i] += timespent_archive[i][j]
        timespent_global[i] = timespent_global_sum[i] / len(timespent_archive[i])
        speedup_global[i] = timespent_global[0] / timespent_global[i]
        if speedup_global[i] > max_speedup:
            max_speedup = speedup_global[i]
    print(timespent_global)
    print(speedup_global)
    print(max_speedup)

    plt.bar(nb_threads, speedup_global, align='center', alpha=0.4)
    plt.plot(nb_threads, speedup_global)
    plt.axis([0, len(timespent_archive)+1, 0, max_speedup*1.1])
    plt.xlabel("""Number of threads""")
    plt.ylabel("""Speed-up""")
    plt.title("""Speed-up as a function of the number of threads""")

    plt.show()
    
    
if __name__ == "__main__":
    main()
