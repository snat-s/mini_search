# What is this?

A simple implementation of a local search engine for my notes.

# Why?

I wanted to experiment with tf-idf and this is what I created. It is
usable, but it still needs some work.

# How do I run it?

First run `make` in the command line. With the exececutable produce run:

```bash
./mini-se index <path/to/folder/to/index>
```

To index a specific folder. This command will create a SQLite database
where the information of tf-idf will be saved.

```bash
./mini-se serve
```

To serve the webpage where you can see the indexed pages.


# Requirements

You need to install the following.

```bash
sudo dnf install sqlite-devel sqlitecpp-devel g++
```
In this case it is for Fedora.

# Notice

This is still a work in progress and can change in the future.
