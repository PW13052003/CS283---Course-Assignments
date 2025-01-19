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

  -h <br>
<b>Command:</b> ./stringfun.c -h
<br>
<b>Output:-</b> 
<br>
usage: ./stringfun [-h|c|r|w|x] "string" [other args] 
<br><br>
  -c <br>
<b>Command:</b> ./stringfun.c -c "Systems Programming is Great!"
<br>
<b>Output:-</b> 
<br>
Word Count: 4 
<br><br>
  -r <br>
<b>Command:</b> ./stringfun.c -r "Systems Programming is Great!"
<br>
<b>Output:-</b> 
<br>
The string is reversed as follows: .....................!taerG si gnimmargorP smetsyS
<br><br>
  -w <br>
<b>Command:</b> ./stringfun.c -w "Systems Programming is Great!"
<br>
<b>Output:-</b>
<br>
Word Print 
<br>----<br>
1. Systems (7)<br>
2. Programming (11)<br>
3. is (2)<br>
4. Great! (6) 
<br><br>
** EXTRA CREDIT **
  -x <br>
<b>Command:</b> ./stringfun.c -x "Systems Programming is Great!" Programming Architecture
<br>
<b>Output:-</b>
<br>
Modified String: Systems Architecture is Great!....................

