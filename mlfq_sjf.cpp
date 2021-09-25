// a1747661, Scalzi, Isabella
// a1715595, Chatha, Balraj
// a1773958, Shah, Ridham
// integration
/*
created by Andrey Kan
andrey.kan@adelaide.edu.au
2021
*/
#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <algorithm>
#include <cmath>

// std is a namespace: https://www.cplusplus.com/doc/oldtutorial/namespaces/
int TIME_ALLOWANCE_0;  // Time Quantum for first queue
int TIME_ALLOWANCE_1;  // Time Quantum for second queue
const int PRINT_LOG = 0; // print detailed execution trace

class Customer
{
public:
    std::string name;
    int priority;
    int arrival_time;
    int slots_remaining; // how many time slots are still needed
    int playing_since;

    Customer(std::string par_name, int par_priority, int par_arrival_time, int par_slots_remaining)
    {
        name = par_name;
        priority = par_priority;
        arrival_time = par_arrival_time;
        slots_remaining = par_slots_remaining;
        playing_since = -1;
    }
};

class Event
{
public:
    int event_time;
    int customer_id;  // each event involes exactly one customer
    int priority;

    Event(int par_event_time, int par_customer_id)
    {
        event_time = par_event_time;
        customer_id = par_customer_id;
    }
};

// Approach for creating a SJF queue taken from https://en.cppreference.com/w/cpp/algorithm/stable_sort
struct SJFHelper
{
    int customer_id;
    int time_remaining;
};

// Used to sort items in the SJF queue
bool operator<(const SJFHelper &lhs, const SJFHelper &rhs)
{
    return lhs.time_remaining < rhs.time_remaining;
}

void initialize_system(
    std::ifstream &in_file,
    std::deque<Event> &arrival_events,
    std::vector<Customer> &customers,
    std::vector<int> &burst_times)
{
    std::string name;
    int priority, arrival_time, slots_requested;

    // read input file line by line
    // https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
    int customer_id = 0;
    while (in_file >> name >> priority >> arrival_time >> slots_requested)
    {
        Customer customer_from_file(name, priority, arrival_time, slots_requested);
        customers.push_back(customer_from_file);

        // new customer arrival event
        Event arrival_event(arrival_time, customer_id);
        arrival_events.push_back(arrival_event);

        // Record the burst time in a separate vector
        burst_times.push_back(slots_requested);

        customer_id++;
    }
}

void print_state(
    std::ofstream &out_file,
    int current_time,
    int current_id,
    const std::deque<Event> &arrival_events,
    const std::deque<int> &high_priority,
    const std::deque<int> &low_priority)
{
    out_file << current_time << " " << current_id << '\n';
    if (PRINT_LOG == 0)
    {
        return;
    }
    std::cout << current_time << ", " << current_id << '\n';
    for (int i = 0; i < arrival_events.size(); i++)
    {
        std::cout << "\t" << arrival_events[i].event_time << ", " << arrival_events[i].customer_id << ", ";
    }
    std::cout << '\n';
    for (int i = 0; i < high_priority.size(); i++)
    {
        std::cout << "Contents of High Priority" << std::endl;
        std::cout << "\t" << high_priority[i] << ", ";
    }
    std::cout << '\n';
}

void print_queue(const std::deque<int> &queue)
{
    for(int i=0; i<queue.size(); i++)
    {
        std::cout << queue.at(i) << " ";
    }
    std::cout << std::endl;
}

// process command line arguments
// https://www.geeksforgeeks.org/command-line-arguments-in-c-cpp/
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Provide input and output file names." << std::endl;
        return -1;
    }
    std::ifstream in_file(argv[1]);
    std::ofstream out_file(argv[2]);
    if ((!in_file) || (!out_file))
    {
        std::cerr << "Cannot open one of the files." << std::endl;
        return -1;
    }

    // deque: https://www.geeksforgeeks.org/deque-cpp-stl/
    // vector: https://www.geeksforgeeks.org/vector-in-cpp-stl/
    std::deque<Event> arrival_events; // new customer arrivals
    std::vector<Customer> customers; // information about each customer
    std::vector<int> burst_times;   // Each customer's burst times, used to calculate the time quantums for the first two queues

    // read information from file, initialize events queue
    initialize_system(in_file, arrival_events, customers, burst_times);

    // Sort all the burst times from lowest to highest
    std::sort(burst_times.begin(), burst_times.end());

    // Find the index of the element that is at the 10th percentile of the burst times
    int tenpercent = std::round(0.1*burst_times.size());

    TIME_ALLOWANCE_0 = burst_times.at(tenpercent);
    TIME_ALLOWANCE_1 = 2*TIME_ALLOWANCE_0;

    int current_id = -1; // who is using the machine now, -1 means nobody
    int time_out = -1; // time when current customer will be preempted
    std::deque<int> queue_0; // waiting queue
    std::deque<int> queue_1;
    std::deque<int> queue_2;

    std::vector<SJFHelper> sjf_vector;

    // step by step simulation of each time slot
    bool all_done = false;
    for (int current_time = 0; !all_done; current_time++)
    {
        // welcome newly arrived customers
        while (!arrival_events.empty() && (current_time == arrival_events[0].event_time))
        {
            // Put all processes on the high priority queue first
            queue_0.push_back(arrival_events[0].customer_id);
            arrival_events.pop_front();
        }
        // check if we need to take a customer off the machine
        if (current_id >= 0)
        {
            if (current_time == time_out)
            {
                int last_run = current_time - customers[current_id].playing_since;
                customers[current_id].slots_remaining -= last_run;

                if (customers[current_id].slots_remaining > 0)
                {
                    // Put customers back on the relevant queue based on their priority
                    if(customers[current_id].priority == 0)
                    {
                        // If the customer started off as high priority send them to the second queue
                        customers[current_id].priority == 1;
                        queue_1.push_back(current_id);
                    }
                    else if(customers[current_id].priority == 1)
                    {
                        // If the customer started off as low priority, send them straight to the SJF queue
                        customers[current_id].priority == 2;

                        SJFHelper helper {current_id, customers[current_id].slots_remaining};
                        sjf_vector.push_back(helper);
                        std::stable_sort(sjf_vector.begin(), sjf_vector.end());
                    }
                    else
                    {
                        // Send customer to SJF queue
                        SJFHelper helper {current_id, customers[current_id].slots_remaining};
                        sjf_vector.push_back(helper);
                        std::stable_sort(sjf_vector.begin(), sjf_vector.end());

                    }
                }
                current_id = -1; // the machine is free now
            }
        }
        // if machine is empty, schedule a new customer
        if (current_id == -1)
        {
            if (!queue_0.empty())
            {
                current_id = queue_0.front();

                // DEBUG
                // std::cout << "Customer " << current_id << " is being scheduled " << "with response time " << current_time - customers.at(current_id).arrival_time << std::endl;

                queue_0.pop_front();
                if (TIME_ALLOWANCE_0 > customers[current_id].slots_remaining)
                {
                    time_out = current_time + customers[current_id].slots_remaining;
                }
                else
                {
                    time_out = current_time + TIME_ALLOWANCE_0;
                }
                customers[current_id].playing_since = current_time;
            }
            // If no-one is waiting in high priority, schedule a customer in low priority
            else if(!queue_1.empty())
            {
                current_id = queue_1.front();

                // DEBUG
                // std::cout << "Customer " << current_id << " is being scheduled" << " with response time "  << customers.at(current_id).arrival_time << std::endl;

                queue_1.pop_front();
                if (TIME_ALLOWANCE_1 > customers[current_id].slots_remaining)
                {
                    time_out = current_time + customers[current_id].slots_remaining;
                }
                else
                {
                    time_out = current_time + TIME_ALLOWANCE_1;
                }
                customers[current_id].playing_since = current_time;

            }
            // If there are no customers in high or low priority, schedule a customer in the FCFS queue
            else if(!sjf_vector.empty())
            {
                // Schedule the customer with the shortest time remaining
                current_id = sjf_vector.front().customer_id;
                sjf_vector.erase(sjf_vector.begin());
                
                time_out = current_time + customers[current_id].slots_remaining;
                customers[current_id].playing_since = current_time;
            }
        }

        print_state(out_file, current_time, current_id, arrival_events, queue_0, queue_1);

        // exit loop when there are no new arrivals, no waiting and no playing customers
        all_done = (arrival_events.empty() && queue_0.empty() && queue_1.empty() && (current_id == -1));
    }

    return 0;
}
