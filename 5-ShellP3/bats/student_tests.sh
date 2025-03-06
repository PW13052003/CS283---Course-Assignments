#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "No piping with ls" {
    run "./dsh" <<EOF
ls
EOF
    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    
    # Get the expected output
    expected_output="batsdshdsh_cli.cdshlib.cdshlib.hmakefilequestions.mddsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check the exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Two piped commands" {	
    run "./dsh" <<EOF
ls | grep dshlib
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    
    # Get the expected output
    expected_output="dshlib.cdshlib.hdsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check the exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Three piped commands" {
    run "./dsh" <<EOF
ls | grep dsh | wc -l
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    
    # Get the expected output
    expected_output="4dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Double piping" {
    run "./dsh" <<EOF
ls || grep dshlib
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Get the expected output
    expected_output="dshlib.cdshlib.hdsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Triple piping" {
    run "./dsh" <<EOF
ls ||| grep dshlib
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Get the expected output
    expected_output="dshlib.cdshlib.hdsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Long pipeline - 1" {
    run "./dsh" <<EOF
ls | grep dsh | sort | uniq | wc -l
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Get the expected output
    expected_output="4dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Long pipeline - 2" {
    run "./dsh" <<EOF
ls | sort | uniq | grep ".c" | awk '{print \$1}' | sed 's/.c//g'
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Get the expected output
    expected_output="grep::Nosuchfileordirectorydsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Pipeline with sort" {
    run "./dsh" <<EOF
ls | sort
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Get the expected output
    expected_output="batsdshdsh_cli.cdshlib.cdshlib.hmakefilequestions.mddsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Pipeline with echo" {
    run "./dsh" <<EOF
echo "hello world" | grep hello
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Get the expected output
    expected_output="helloworlddsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Pipeline with awk" {
    run "./dsh" <<EOF
echo "1 2 3" | awk '{print \$2}'
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Get the expected output
    expected_output="2dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Pipeline with sed" {
    run "./dsh" <<EOF
echo "hello   world" | sed 's/world/people/'
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Get the expected output
    expected_output="hellopeopledsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Pipeline with exec" {
    run "./dsh" <<EOF
exec echo "hello" | wc -l
EOF
  
    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Get the expected output
    expected_output="execvpfailed:Nosuchfileordirectory0dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Pipeline with nonexistent command" {
    run "./dsh" <<EOF
ls | nonexistent_command | wc -l
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Get the expected output
    expected_output="execvpfailed:Nosuchfileordirectory0dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Pipeline with SIGINT Handling" {
    run "./dsh" <<EOF
yes | head -n 5
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Get the expected output
    expected_output="yyyyydsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Pipeline with command that fails" {
    run "./dsh" <<EOF
ls | grep dsh | false | wc -l
EOF
   
    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Get the expected output
    expected_output="0dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

