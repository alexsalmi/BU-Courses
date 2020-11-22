import subprocess
import statistics

__author__ = "Alexander Salmi"
__email__ = "asalmi@bu.edu"
__lab__ = "lab2"

""" # EC441

## Lab 2 -- Traceroute

Traceroute is a simple utility that can be run from any Internet host. When the user
specifies a certain host destination name, Traceroute shows the name and roundtrip
time of each router along the way from the source to the destination (including).

1. Download this file and rename it to lab2_XXX.py, where XXX is your BU email/login name. 
For jdoe@bu.edu, the file would be called lab2_jdoe.py

2. Solve the problems below, and make sure of the following requirements:
    1. Your solution must work in a Python 3 environment.
    2. Replace the **\_\_author\_\_** and **\_\_email\_\_** fields at the top of this file with your name and email.
    3. Do not modify the function names and arguments, as this will interfere with how we validate your solution. You may freely add additional helper functions and classes.
    4. Document your code. This will help us give you partial credit when a solution is wrong, but the intermediary steps make sense. 
"""

def lab2_problem1(host='bu.edu'):
    """ ### Problem 1: Simple Traceroute

You have seen in class how the traceroute command-line utility can be used to measure the roundtrip time to a specific host. A simple example of how to call traceroute from within Python is given here:

    for s in _raw_traceroute('bu.edu'): 
        print(s.split('  ')) 
        # then do something with the extracted output 

By default, traceroute only queries three times per 'TTL'. Look up the manpage of traceroute to find out how to change the number of queries. Then, adapt the helper function (or write your own new implementation below) such that the traceresult variable contains a traceroute run that uses 10 queries. You may have to look up the documentation for Python's 'subprocess' module to see how to change its input.

**Goal:** This function should return a list of traceroute output strings (similar to the helper function) with timing data from 10 query probes.
"""
    # I changed the helper function to includ "-q 10" in the subprocess.run(), so that 10 query probes will be sent
    traceresult = _raw_traceroute(host)
    return traceresult

def lab2_problem2():
    """ ### Problem 2

1.  Run traceroute on the website of the University of Florida, www.ufl.edu.
2.  For each hop, calculate the average roundtrip time, as well as the standard deviation (use the stdev function from the statistics package)
3.  Return a list of (avg, stdev) tuples for each hop.

A code skeleton is given below for reference, feel free to adapt or replace.
    """
    traceresult = _raw_traceroute("www.ufl.edu")
    hopstats = []

    # Call helper function _get_roundtrip_times to get a list of lists containing all 10 roundtrip times for every stop
    times = _get_roundtrip_times(traceresult)

    # Loop through each stop to get the avg and std of each stop's times
    for time in times:
        # if there are not 10 times corresponding to all 10 probes, ignore this stop
        if len(time) < 10:
            continue
        # get the average and stdev of the times for this stop
        avg = sum(time) / len(time)
        std = statistics.stdev(time)
        #append the avg and stdev to the hopstats list
        hopstats.append( (avg, std) )
        
    return hopstats


def lab2_problem3():
    """ ### Problem 3

1. Run traceroutes for the following university domains:
    a. stanford.edu
    b. www.ethz.ch
    c. www.tum.de
2. Return a list that contains integers representing
    * the hops that are common between a. and b.
    * the hops that are common between a. and c.
    * the hops that are common between b. and c.
    """
    # get the traceroute results for ach of the urls
    a_raw = _raw_traceroute("www.stanford.edu")
    b_raw = _raw_traceroute("www.ethz.ch")
    c_raw = _raw_traceroute("www.tum.de")

    # use the helper function _get_ip_adresses to convert the result string into a list of strings, containing the ip adress for each stop
    a = _get_ip_adresses(a_raw)
    b = _get_ip_adresses(b_raw)
    c = _get_ip_adresses(c_raw)

    # get a list of all of the common ip adresses between two traceroutes by getting the intersection of sets of each traceroute's ip adresses
    a_and_b = _get_common_hops(a,b)
    a_and_c = _get_common_hops(a,c)
    b_and_c = _get_common_hops(b,c)

    # add the lists to the commonhops list
    commonhops = [ a_and_b, a_and_c, b_and_c ]

    # return commonhops
    return commonhops


""" ### Helper Functions

Helper functions are defined with a leading underscore (e.g. def _helper_function()). 
These functions are provided for your convenience, you may use them as provided, 
change them, or use an alternative solution altogether.
"""
def _raw_traceroute(host):
    """ The helper function **_raw_traceroute(host)** calls traceroute on a 
    host and returns a list of the results.
    """
    tr = subprocess.run(["traceroute", host, "-q 10"], stdout=subprocess.PIPE) # executing traceroute as a subprocess
    return tr.stdout.decode("utf-8").split('\n') # return a list of strings containing the traceroute output

def _get_roundtrip_times(traceresult):
    # Function to return a list of lists of the roundtrip times for each probe at every step
    times = []
    for t in traceresult:
        # get all the times from the string by filtering to only include parts of the string containing " ms"
        timestrings = list(filter(lambda x : " ms" in x, t.split('  ')))
        # change all the time strings in the format "xx.xxx ms" by removing the " ms" part of the string, then converting to a float
        this_times = list(map(lambda x : float(x.split(' ')[0]), timestrings))
        # append the list of floats (times) to the times list
        times.append(this_times)
    return times

def _get_ip_adresses(traceresult):
    ips = []
    for t in traceresult:
        ipstrings = list(filter(lambda x : "(" in x, t.split('  ')))
        this_ips = list(map(lambda x : x.split('(')[1].split(')')[0], ipstrings))
        if(len(this_ips) == 0):
            continue
        ips.append(this_ips[0])
    return ips

def _get_common_hops(a, b):
    common_hops = 0
    smaller = min(len(a), len(b))
    for i in range(smaller):
        if(a[i] == b[i]):
            common_hops += 1
    return common_hops


# This section is only here so you can run your python code 
# directly from the command line by typing
#   $ python3 lab2_yourname.py
# There is nothing to do here.


def main():
    lab2_problem1()
    lab2_problem2()
    lab2_problem3()    
if __name__ == "__main__":
    main()
