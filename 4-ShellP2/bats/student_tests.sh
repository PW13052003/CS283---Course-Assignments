#!/usr/bin/env bats

@test "Basic command execution without arguments -> ls" {
	run "./dsh" <<EOF
ls
EOF

	# Strip all whitespace from the output
	stripped_output=$(echo "$output" | tr -d '[:space:]')

	# Get expected ls output 
	expected_ls_output=$(ls | sort | tr -d '[:space:]')
	
	# Expected prompt formatting
	expected_prompt="dsh2>dsh2>cmdloopreturned0"	
	
	# Get expected output
	expected_output="${expected_ls_output}${expected_prompt}"

	# These echo commands will help with debugging and will only print if the test fails	
	echo "Captured stdout:"
	echo "Output: $output"
	echo "Exit Status: $status"
	echo "${stripped_output} -> ${expected_output}"
	
	# Check exact match
	[ "$stripped_output" = "$expected_output" ]
}	


@test "Echo with multiple spaces" {
	run "./dsh" <<EOF
echo        Hello     World
EOF

	# Strip all whitespace from the output
        stripped_output=$(echo "$output" | tr -d '[:space:]')

	# Get expected output
	expected_output="HelloWorlddsh2>dsh2>cmdloopreturned0"

	# These echo commands will help with debugging and will only print if the test fails
	echo "Captured stdout:"
        echo "Output: $output"
        echo "Exit Status: $status"
        echo "${stripped_output} -> ${expected_output}"
	
	# Check exact match
        [ "$stripped_output" = "$expected_output" ]
}


@test "Mixed quotes (single/double)" {
	run "./dsh" <<EOF
echo "Hello 'world'"
EOF

	# Strip all whitespace from the output
        stripped_output=$(echo "$output" | tr -d '[:space:]')

	# Get expected output
	expected_output="Hello'world'dsh2>dsh2>cmdloopreturned0"

	# These echo commands will help with debugging and will only print if the test fails
        echo "Captured stdout:"
        echo "Output: $output"
        echo "Exit Status: $status"
        echo "${stripped_output} -> ${expected_output}"

	# Check exact match
        [ "$stripped_output" = "$expected_output" ]
}


@test "cd command without any arguments" {
	run "./dsh" <<EOF
cd
EOF

	# Strip all whitespace from the output
        stripped_output=$(echo "$output" | tr -d '[:space:]')

	# Detect number of extra prompts printed by dsh
    	prompt_count=$(echo "$output" | grep -o "dsh2>" | wc -l)

	# Construct expected prompt string dynamically
    	expected_prompt=$(printf 'dsh2>%.0s' $(seq 1 $prompt_count))
    	expected_prompt="${expected_prompt}cmdloopreturned0"

	# Get expected output
    	expected_output="${expected_home}${expected_prompt}"

	# These echo commands will help with debugging and will only print if the test fails
        echo "Captured stdout:"
        echo "Output: $output"
        echo "Exit Status: $status"
        echo "${stripped_output} -> ${expected_output}"

        # Check exact match
        [ "$stripped_output" = "$expected_output" ]
}


@test "cd command to parent directory" {
	
	# Get to the working directory before running the test
	working_dir=$(pwd)

	run "./dsh" <<EOF
cd ..
pwd
EOF

	# Strip all whitespace from the output
	stripped_output=$(echo "$output" | tr -d '[:space:]')

	# Get expected directory
	expected_dir=$(dirname "$working_dir" | tr -d '[:space:]')

	# Detect number of extra prompts printed by dsh
	prompt_count=$(echo "$output" | grep -o "dsh2>" | wc -l)

	# Construct expected prompt string dynamically
	expected_prompt=$(printf 'dsh2>%.0s' $(seq 1 $prompt_count))
	expected_prompt="${expected_prompt}cmdloopreturned0"

	# Get expected output
	expected_output="${expected_dir}${expected_prompt}"

	# These echo commands will help with debugging and will only print if the test fails
	echo "Captured stdout:"
        echo "Output: $output"
        echo "Exit Status: $status"
        echo "${stripped_output} -> ${expected_output}"

	# Check exact match
	[ "$stripped_output" = "$expected_output" ]
}


@test "cd command to valid directory" {

	# Check if directory in question exists
	mkdir -p sample_dir

	run "./dsh" <<EOF
cd sample_dir
pwd
EOF

	# Strip all whitespace from the output
        stripped_output=$(echo "$output" | tr -d '[:space:]')

	# Get expected directory
	expected_dir="$(pwd)/sample_dir"
	expected_dir=$(cd "$expected_dir" && pwd | tr -d '[:space:]')

	# Detect number of extra prompts printed by dsh
    	prompt_count=$(echo "$output" | grep -o "dsh2>" | wc -l)

    	# Construct expected prompt string dynamically
    	expected_prompt=$(printf 'dsh2>%.0s' $(seq 1 $prompt_count))
	expected_prompt="${expected_prompt}cmdloopreturned0"

	# Get expected output
	expected_output="${expected_dir}${expected_prompt}"

	# These echo commands will help with debugging and will only print if the test fails
        echo "Captured stdout:"
        echo "Output: $output"
        echo "Exit Status: $status"
        echo "${stripped_output} -> ${expected_output}"

	# Check match
	[ "$stripped_output" = "$expected_output" ]

	# Remove this temp directory everytime the test is run
	rmdir sample_dir
}


@test "cd command to invalid directory" {
	
	run "./dsh" <<EOF
cd invalid_dir
EOF

	# Strip all whitespace from the output
        stripped_output=$(echo "$output" | tr -d '[:space:]')

	# Get expected output
	expected_output="cdfailed:Nosuchfileordirectorydsh2>dsh2>cmdloopreturned0"

	# These echo commands will help with debugging and will only print if the test fails
        echo "Captured stdout:"
        echo "Output: $output"
        echo "Exit Status: $status"
        echo "${stripped_output} -> ${expected_output}"

	# Check match
        [ "$stripped_output" = "$expected_output" ]
}


@test "exit command" {
	run "./dsh" <<EOF
exit 
EOF

	# Strip all whitespace from the output
        stripped_output=$(echo "$output" | tr -d '[:space:]')

    	# Expected output
    	expected_output="dsh2>"

	# These echo commands will help with debugging and will only print if the test fails
        echo "Captured stdout:"
        echo "Output: $output"
        echo "Exit Status: $status"
        echo "${stripped_output} -> ${expected_output}"

	# Check match
	[ "$stripped_output" = "$expected_output" ]
}


@test "Invalid command" {
	run "./dsh" <<EOF
foo
EOF

	# Strip all whitespace from the output
        stripped_output=$(echo "$output" | tr -d '[:space:]')

	# Get expected output
	expected_output="execvpfailed:Nosuchfileordirectorydsh2>dsh2>dsh2>cmdloopreturned0"

	# These echo commands will help with debugging and will only print if the test fails
        echo "Captured stdout:"
        echo "Output: $output"
        echo "Exit Status: $status"
        echo "${stripped_output} -> ${expected_output}"

	# Check match
	[ "$stripped_output" = "$expected_output" ]
}


@test "Handling empty input" {
	run "./dsh" <<EOF

EOF

	# Strip all whitespace from the output
        stripped_output=$(echo "$output" | tr -d '[:space:]')

	# Get expected output
	expected_output="dsh2>warning:nocommandsprovideddsh2>cmdloopreturned0"

	# These echo commands will help with debugging and will only print if the test fails
        echo "Captured stdout:"
        echo "Output: $output"
        echo "Exit Status: $status"
        echo "${stripped_output} -> ${expected_output}"

	# Check match
        [ "$stripped_output" = "$expected_output" ]
}


@test "Handling empty echo" {
	run "./dsh" <<EOF
echo ""
EOF

	# Strip all whitespace from the output
        stripped_output=$(echo "$output" | tr -d '[:space:]')

	# Get expected output
	expected_output="dsh2>dsh2>cmdloopreturned0"

	# These echo commands will help with debugging and will only print if the test fails
        echo "Captured stdout:"
        echo "Output: $output"
        echo "Exit Status: $status"
        echo "${stripped_output} -> ${expected_output}"

	# Check match
        [ "$stripped_output" = "$expected_output" ]
}



