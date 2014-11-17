#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import subprocess
import timeit
import json


def main():
    """This Python script launches the parallel calculation multiple
    times, but with a different number of processors each time."""
    sp = subprocess.Popen(["""nproc"""], stdout=subprocess.PIPE)
    max_number_of_threads_string, _ = sp.communicate()
    max_number_of_threads = int(max_number_of_threads_string)
    dict_list = []
    
    for number_of_threads in range(1, max_number_of_threads + 1):
        os.environ["OMP_NUM_THREADS"] = str(number_of_threads)
        print(os.environ.get("OMP_NUM_THREADS"))
        #os.system("""time ./main""")
        #subprocess.call(["""time""", """./main"""])
        my_timeit = timeit.timeit(stmt = """subprocess.call(["./main"])""",
            setup = """import subprocess""", number = 1)
            
        print(my_timeit)
        local_dict = {"nb_of_threads": number_of_threads, "exec_time": my_timeit}
        dict_list.append(local_dict)
        with open('timedata.json', 'w') as jsonfile:
            json.dump(dict_list, jsonfile) # This overwrites the existing file.
            
    print(dict_list)
        

if __name__ == "__main__":
    main()
