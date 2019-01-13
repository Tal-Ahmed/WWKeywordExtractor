#!/usr/bin/python

train_file = "data/train.txt"
test_file = "data/test.txt"

new_train_file = train_file + ".flatten"
new_test_file = test_file + ".flatten"

train_fd = open(new_train_file, 'a')
test_fd = open(new_test_file, 'a')

l = []
for line in open(train_file, 'r').readlines():
    l.append(line)
    if line == ". . O":
        train_fd.write(" ".join(l))
        l = []