#!/usr/bin/env python3
import subprocess


text = input("Input text:\n")
f = open("text.txt", "w")
f.write(text)
f.close()


cp = subprocess.run("./a.out text.txt wordlist.txt", shell = True, stdout = subprocess.PIPE)

print(cp.stdout.decode().replace("\n", ", "))
