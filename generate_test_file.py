#!/bin/bash
import random

no_customers = random.randint(5, 70)
time = 0
output = ''
for i in range(no_customers):
    # Customer Number
    output = 'c'
    output += str(i)
    output += ' '

    # Priority
    priority = random.randint(0,1)
    output += str(priority)
    output += ' '
    
    # Time Arrived
    time = random.randint(time, time + 20)
    output += str(time)
    output += ' '

    # Burst time
    burst_time = random.randint(1,100)
    output += str(burst_time)

    # Print the final line 
    print(output)

