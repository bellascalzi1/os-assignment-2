#include <bits/stdc++.h>
using namespace std;

class Customer
{
public:
    int priority;
    int arrival_time;
    int slots_remaining; // how many time slots are still needed
    int response_time;

    Customer(int par_priority, int par_arrival_time, int par_slots_remaining)
    {
        priority = par_priority;
        arrival_time = par_arrival_time;
        slots_remaining = par_slots_remaining;
        response_time = -1;
    }
};

void read_customer_info(std::ifstream &in_file, std::vector<Customer> &customers)
{
    std::string name;
    int priority, arrival_time, slots_requested;

    // read input file line by line
    // https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
    int customer_id = 0;
    while (in_file >> name >> priority >> arrival_time >> slots_requested)
    {
        // structure initialization
        // https://en.cppreference.com/w/c/language/struct_initialization
        Customer customer_from_file(priority, arrival_time, slots_requested);
        customers.push_back(customer_from_file);
        customer_id++;
    }
    vector<Customer> Low_customers;
    vector<Customer> High_customers;
    for (int i = 0; i < customers.size(); i++)
    {
        if (customers[i].priority == 0)
        {
            High_customers.push_back(customers[i]);
        }
        else
        {
            Low_customers.push_back(customers[i]);
        }
        // cout << customers[i].priority << endl;
    }
    cout << "priority=0 "
         << "arrival_time "
         << "slots_remaining " << endl;
    for (auto x : High_customers)
    {
        cout << x.priority << "               " << x.arrival_time
             << "          " << x.slots_remaining << endl;
    }
    cout << "priority=1 "
         << "arrival_time "
         << "slots_remaining " << endl;
    for (auto x : Low_customers)
    {
        cout << x.priority << "               " << x.arrival_time
             << "          " << x.slots_remaining << endl;
    }
};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Provide input and output file names." << std::endl;
        return -1;
    }
    // Customer obj;
    std::ifstream data_file(argv[1]);
    std::ifstream results_file(argv[2]);
    if ((!data_file) || (!results_file))
    {
        std::cerr << "Cannot open files." << std::endl;
        return -1;
    }

    // vector: https://www.geeksforgeeks.org/vector-in-cpp-stl/
    std::vector<Customer> customers; // information about each customer
    read_customer_info(data_file, customers);
}