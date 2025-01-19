# CS283 

## Assignment: 1-C-Refresher

## Author: Puranjay Wadhera (pw425@drexel.edu)

## Purpose
To replicate a text line utility in C. The program processes a string provided via the command line and performs operations such as counting words, reversing characters, printing words with lengths, and replacing specified words.

## Options
1. -h : Prints the help information for the program 
2. -c : Counts and prints the number of words in a sample string
3. -r : Reverses the characters of a sample string in place and prints the reversed string
4. -w : Prints each word in a sample string along with its length
5. -x : Replaces a specified word with another in a sample string 

## Sample Testing Routine
1. -h
<b>Command:</b> ./stringfun.c -h
<b>Output:-</b> 
usage: ./stringfun [-h|c|r|w|x] "string" [other args] 

2. -c
<b>Command:</b> ./stringfun.c -c "Systems Programming is Great!"
<b>Output:-</b> 
Word Count: 4 

3. -r
<b>Command:</b> ./stringfun.c -r "Systems Programming is Great!"
<b>Output:-</b> 
The string is reversed as follows: .....................!taerG si gnimmargorP smetsyS

4. -w
<b>Command:</b> ./stringfun.c -w "Systems Programming is Great!"
<b>Output:-</b>
Word Print 
----
1. Systems (7)
2. Programming (11)
3. is (2)
4. Great! (6) 

5. -x
<b>Command:</b> ./stringfun.c -x "Systems Programming is Great!" Programming Architecture
<b>Output:-</b>
Modified String: Systems Architecture is Great!....................

