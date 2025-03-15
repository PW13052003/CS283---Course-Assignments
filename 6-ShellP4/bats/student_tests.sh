#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Basic command execution : echo" {
    run "./dsh" <<EOF
echo Hello World
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    
    # Get the expected output
    expected_output="HelloWorldlocalmodedsh4>dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check the exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Handling an invalid command" {
    run "./dsh" <<EOF
fvfvjknfknejn
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    
    # Get the expected output
    expected_output="execvpfailed:Nosuchfileordirectorylocalmodedsh4>localmodedsh4>dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check the exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Handling exit command" {
    run "./dsh" <<EOF
exit
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Get the expected output
    expected_output="execvpfailed:Nosuchfileordirectorylocalmodedsh4>localmodedsh4>dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check the exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Excessive piping" {
    run "./dsh" <<EOF
ls | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Get the expected output
    expected_output="localmodedsh4>error:pipinglimitedto8commandsdsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check the exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Client-Server Connection Test" {

    # Start the server in the background
    ./dsh -s -i 0.0.0.0 -p 5000 &

    # Give it some time to start
    sleep 2

    # Run the client to check if it can connect
    run ./dsh -c 127.0.0.1 -p 5000 <<EOF
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Get the expected output
    expected_output="socketclientmode:addr:127.0.0.1:5000dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check the exact match
    [ "$stripped_output" = "$expected_output" ]

    # Kill the server process after test
    pkill -f "./dsh -s -i 0.0.0.0 -p 5000"
}

@test "Output redirection" {
    run ./dsh <<EOF
echo 'outputtesting' > outfoo.txt
cat outfoo.txt
exit
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Get the expected output
    expected_output="outputtesting>outfoo.txtlocalmodedsh4>cat:outfoo.txt:Nosuchfileordirectorydsh4>execvpfailed:Nosuchfileordirectorydsh4>dsh4>dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check the exact match
    [ "$stripped_output" = "$expected_output" ]

    # Cleanup
    rm -f outfoo.txt
}

@test "Input redirection" {
    run ./dsh <<EOF
echo 'inputtesting' > infoo.txt
cat infoo.txt
exit
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Get the expected output
    expected_output="inputtesting>infoo.txtlocalmodedsh4>cat:infoo.txt:Nosuchfileordirectorydsh4>execvpfailed:Nosuchfileordirectorydsh4>dsh4>dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check the exact match
    [ "$stripped_output" = "$expected_output" ]

    # Cleanup
    rm -f infoo.txt
}

