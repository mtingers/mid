# mid
A `head` and `tail` CLI tool counterpart -- `mid` prints the middle of a file (line based).


./mid [-o] [-n number-of-lines] [-] [file1] [file2...]
    -o      print offset (e.g. '@@ line,offset @@')
    -n N    print N lines surrounding midpoint

# Examples
```bash
# print 10 lines from the middle of the file
mid file1.txt

# print 20 lines
mid -n 20 file1.txt file2.txt

# use stdin
cat file1.txt | mid -o 

# use stdin and a file by name ('-' required)
cat file1.txt | mid -o - stdin_and_anotherfile_example.txt
```
