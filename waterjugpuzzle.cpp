/*******************************************************************************
 * Name        : waterjugpuzzle.cpp
 * Author      : Collin Smith
 * Date        : October 19, 2023
 * Description : Water Jug Puzzle
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
#include <queue>
#include <set>
#include <cctype>
using namespace std;

// Struct to represent state of water in the jugs.
struct State {
    int a, b, c;
    string directions;
    State *parent;

    // Constructor - no directions nor parent (used for goal at the start of the program since its parent and directions are unknown at this point)
    State(int _a, int _b, int _c) :
        a{_a}, b{_b}, c{_c}, directions{nullptr}, parent{nullptr} { }

    // Constructor - directions, no parent (used for initial, since it has no parent, and its directions are "Initial state.")
    State(int _a, int _b, int _c, string _directions) :
        a{_a}, b{_b}, c{_c}, directions{_directions}, parent{nullptr} { }

    // Constructor - includes both directions and parent (used for all other States created, as its directions and parent are known)
    State(int _a, int _b, int _c, string _directions, State _parent) :
        a{_a}, b{_b}, c{_c}, directions{_directions}, parent{_parent} { }

    // String representation of state in tuple form.
    string to_string() {
        ostringstream oss;
        oss << "(" << a << ", " << b << ", " << c << ")";
        return oss.str();
    }
};


// Start of main
int main(int argc, char argv[]) {
    // Start checking error cases
    if (argc != 7) {
        cerr << "Usage: ./waterjugpuzzle.cpp <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>";
        return 1;
    }
    
    // Error case: input at argv[i] is not an integer
    for (int i = 1; i < argc; i++) {
        if (!(isdigit(argv[i]))) {
            string i; // Either "capacity" or "goal" in error print
            string j; // Either "A", "B", or "C" in error print
            if (i < 4) {
                i = "capacity";
            } else {
                i = "goal";
            }

            if (i % 3 == 0) {
                j = "A";
            }
            else if (i % 3 == 1) {
                j = "B";
            } else {
                j = "C";
            }
            cerr << "Error: Invalid " << i << " '" << arv[i] << "' for jug " << j << "." << endl;
            return 1;
        }
    }

    // Error case: goal sum != total water in problem
    if ((argv[5].stoi() + argv[6].stoi() + argv[7].stoi()) != argv[4].stoi()) {
        cerr << "Error: Total gallons in goal state must be equal to the capacity of jug C." << endl;
        return 1;
    }

    // Error case: Goal > Capacity of the same jug
    for (int i = 1; i < 4; i++) {
        if (argv[i].stoi() < argv[i+3].stoi()) {
            string j;
            if (i == 1) {
                j = "A";
            }
            if (i == 2) {
                j = "B";
            } 
            if (i == 3) {
                j = "C";
            }
            cerr << "Error: Goal cannot exceed capacity of jug " << j << "." << endl;
            return 1;
        }
    }
    // End of all error cases

    // Initialize variables needed before the while loop begins
    int cap_a = argv[1].stoi();
    int cap_b = argv[2].stoi();
    int cap_c = argv[3].stoi();

    // Initializes the queue for BFS and adds the initial State to the queue so the while loop runs as intended
    queue<State> Q;
    Q.push(s); 
    State current_vertex = Q.front();

    // Initialized as empty, will keep track of every State, or vertex, that has been visited in the BFS algorithm
    vector<State> Log; 
    State goal(argv[4].stoi(), argv[5].stoi(), argv[6].stoi());
    State initial(0, 0, cap_c, "Initial state.");
    
    /* 
    Main algorithm:
    Uses BFS; constructs the tree by filtering through the queue of States and adding its valid neighbors to the queue.
    In each iteration, current_vertex is added to the log and updated to the next State in the queue at the end.
    While loop stops once the goal state has been reached (success) or the queue is empty (no solution).
    */
    while (!Q.empty() && (!was_visited(goal, Log))) {

        // Updates current_vertex to the State at the front of the queue before removing it
        current_vertex = Q.pop(); 

        // Adds current_vertex to the Log of visited States
        Log.push_back(current_vertex); 
        vector<State> neighbors = find_neighbors(current_vertex, cap_a, cap_b, cap_c);

        // Some of the possible neighbors in temp may have been previously visited/queued, so we don't add reoccurences back into the queue
        for (State x : neighbors) { 
            if (!is_present(x, Q, Log)) {
                Q.push(x);
            }
        }

    // End of while loop / BFS algorithm    
    }

    // Checks if the last visited State was the goal, which determines the outcome of the algorithm
    if (Log[(Log.size()-1)].to_string() == goal.to_string()) { 
        // Yes, the goal was reached

        State curr = Log[(Log.size()-1)];

        // Initializes a vector<State> that will show the fastest path of States from the initial to the goal by working backwards
        vector<State> path;
        path.push_back(end);

        /* 
        Loops through the Log by working backwards through every visited State in Log.
        In every iteration, we put curr at the beginning of the path vector and update curr to its parent until it reaches the initial State
        */
        while (curr.to_string != initial.to_string()) {
            for (State t : Log) {
                if (t.to_string() == curr.parent.to_string()) {
                    // Creates a temporary save of t to update curr with
                    State save = t;

                    // Inserts t, the parent of curr, to the front of the vector
                    path.insert(path.begin(), t);
                }
            }
            // Updates curr for the next while loop iteration
            curr = save;
        }

        // The initial output is printed, and path now contains every step -after- the initial state down to the goal
        cout << initial.directions << " " <<  initial.to_string() << endl;

        
        // Prints an info-adjusted statement for each turn in the path
        for (State v : path) {
            cout << construct_print(v) << endl;
        }

    } else { 
        // No, the goal was not reached
        cout << "No solution." << endl;
    }
    
  

    // Program ends
    return 0;
}
// End of main


// Helper function to return the string of a pour/turn to print in the correct format:
// EX: "Pour 5 gallons from C to B. (0, 5, 3)"
string construct_print(const State s) {
    ostringstream oss;
    int gals = 0;
    if (s.parent.a > s.a) {
        gals = s.parent.a - s.a;
    }
    if (s.parent.b > s.b) {
        gals = s.parent.b - s.b;
    }
    if (s.parent.c > s.c) {
        gals = s.parent.c - s.c;
    }

    // Accounts for changes in print if "gallon" is plural or not (if only 1 was poured)
    if (gals == 1) {
        oss << "Pour " << gals << " gallon from " << s.directions << " " << s.to_string();
    } else {
        oss << "Pour " << gals << " gallons from " << s.directions << " " << s.to_string();
    }
    return oss.str();
}


/* 
Helper function: Returns a vector<State> of the possible neighbors of current_vertex under the following conditions:
    - Follows the order of operations given
    - All possible water should be poured at once. Fill up to the receiving jar's cap or pour all that is left in the giving jar
    - Water cannot be poured from an empty jug or into a full jug
    - Adds the correct directions string
    - Add given State 's' as the parent of each constructed child State
    - This does --NOT-- account for previously visited/queued States, only all the possible ones - is_present is used to filter out reoccurences.
*/
vector<State> find_neighbors(const State s, const int cap_a, const int cap_b, const int cap_c) { // Might have problems with parameters having same name as the variables in main
    vector<State> ret;
    int new_a;
    int new_b;
    int new_c;
    string new_directions;

    // 1) C to A
    // Checks to make sure that a is not full and c is not empty
    if (s.a != cap_a && s.c != 0) {
        // Adjusts the neighbor State output depending on if it is filled to its cap
        if (s.c + s.a >= cap_a) {
            new_a = cap_a;
            new_c = s.c - cap_a;
        } else {
            new_a = s.a + s.c;
            new_c = 0;
        }
        new_b = s.b;

        // Constructs the neighbor State and adds it to the return vector
        State c(new_a, new_b, new_c, "C to A", s);
        ret.push_back(c);
    }

    // 2) B to A
    // Checks to make sure that a is not full and c is not empty
    if (s.a != cap_a && s.b != 0) {
        // Adjusts the neighbor State output depending on if it is filled to its cap
        if (s.b + s.a >= cap_a) {
            new_a = cap_a;
            new_b = s.b - cap_a;
        } else {
            new_a = s.a + s.b;
            new_b = 0;
        }
        new_c = s.c;

        // Constructs the neighbor State and adds it to the return vector
        State c(new_a, new_b, new_c, "B to A", s);
        ret.push_back(c);
    }

    // 3) C to B
    // Checks to make sure that a is not full and c is not empty
    if (s.b != cap_b && s.c != 0) {
        // Adjusts the neighbor State output depending on if it is filled to its cap
        if (s.c + s.b >= cap_b) {
            new_b = cap_b;
            new_c = s.c - cap_b;
        } else {
            new_b = s.b + s.c;
            new_c = 0;
        }
        new_a = s.a;

        // Constructs the neighbor State and adds it to the return vector
        State c(new_a, new_b, new_c, "C to B", s);
        ret.push_back(c);
    }

    // 4) A to B
    // Checks to make sure that a is not full and c is not empty
    if (s.b != cap_b && s.a != 0) {
        // Adjusts the neighbor State output depending on if it is filled to its cap
        if (s.a + s.b >= cap_b) {
            new_b = cap_b;
            new_a = s.a - cap_b;
        } else {
            new_b = s.b + s.a;
            new_a = 0;
        }
        new_c = s.c;

        // Constructs the neighbor State and adds it to the return vector
        State c(new_a, new_b, new_c, "A to B", s);
        ret.push_back(c);
    }

    // 5) B to C
    // Checks to make sure that a is not full and c is not empty
    if (s.c != cap_c && s.b != 0) {
        // Adjusts the neighbor State output depending on if it is filled to its cap
        if (s.b + s.c >= cap_c) {
            new_c = cap_c;
            new_b = s.b - cap_c;
        } else {
            new_c = s.c + s.b;
            new_b = 0;
        }
        new_a = s.a;

        // Constructs the neighbor State and adds it to the return vector
        State c(new_a, new_b, new_c, "B to C", s);
        ret.push_back(c);
    }

    // 6) A to C
    if (s.c != cap_c && s.a != 0) {
        // Adjusts the neighbor State output depending on if it is filled to its cap
        if (s.a + s.c >= cap_c) {
            new_c = cap_c;
            new_a = s.a - cap_c;
        } else {
            new_c = s.c + s.a;
            new_a = 0;
        }
        new_b = s.b;

        // Constructs the neighbor State and adds it to the return vector
        State c(new_a, new_b, new_c, "A to C", s);
        ret.push_back(c);
    }

    return ret;
}


/* 
Helper function that takes in a given State (vertex) and returns T/F if a State of the same quantities has been reached in the queue/log
These functions can be called when:
    - searching the log to see if the goal state (vertex) has been visited
    - searching to see if a neighboring state (vertex) has already been accounted for in EITHER the queue or log -- find_neighbors part 2
*/
bool is_present(const State s, const queue<State> q, const vector<State> l) {
    // Search through the queue
    queue<State> tempQueue = q; // Since we'd need to edit the queue to access its contents, we make a duplicate to parse through instead
    while (!tempQueue.empty()) {
        if (tempQueue.front().to_string() == s.to_string()) {
            return true; // State found
        }
        tempQueue.pop();
    }

    // Search through the vector using a for each loop
    for (const State& state : l) {
        if (state.to_string() == s.to_string()) {
            return true; // State found
        }
    }

    return false; // State not found in either
}


// Helper: Searches for s in *only* the queue, returns T/F
bool in_queue(const State s, const queue<State> q) {
    // Search through the queue
    queue<State> tempQueue = q; // Since we'd need to edit the queue to access its contents, we make a duplicate to parse through instead
    while (!tempQueue.empty()) {
        if (tempQueue.front().to_string() == s.to_string()) {
            return true; // State found
        }
        tempQueue.pop();
    }
    return false;
}


// Helper: Searches for s in *only* the log, returns T/F
bool was_visited(const State s, const vector<State> l) {
    // Search through the vector using a for each loop
    for (const State& state : l) {
        if (state.to_string() == s.to_string()) {
            return true; // State found
        }
    }

    return false; // State not found in either
}
