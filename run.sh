#!/bin/zsh

# Compile the solver and validator
g++ -std=c++20 -o build/solver solver.cpp -lprofiler -I/opt/homebrew/include -L/opt/homebrew/lib
g++ -std=c++20 -O2 -o build/validator validator.cpp

# Run the solver on the input file and produce a solution
./build/solver tiny.txt tiny_solution.txt

# Capture the output of the validator
output=$(./build/validator tiny_solution.txt)

# Check the validity of the initial solution
if [ "$output" = "true" ]; then
    echo "Initial solution is valid."
elif [ "$output" = "false" ]; then
    echo "Initial solution is invalid."
    exit 1
else
    echo "Validator error: $output"
    exit 1
fi

# Run the solver multiple times and measure execution time
for I in {1..30}; do
    echo "Run $I:"
    time ./build/solver tiny.txt tiny_solution.txt
    echo ""
done
