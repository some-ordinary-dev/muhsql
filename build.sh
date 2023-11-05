#!/bin/bash

set -ex

# clean bin and obj directory
rm -rf ./obj/*
rm -rf ./bin/*

MUH_FLAGS="-D_XOPEN_SOURCE_EXTENDED -c -ggdb"

# preprocess, compile, assemble
gcc $MUH_FLAGS -o ./obj/muhsql.o muhsql.c
gcc $MUH_FLAGS -o ./obj/sqlite.o drivers/sqlite.c
gcc $MUH_FLAGS -o ./obj/db.o db.c

gcc $MUH_FLAGS -o ./obj/button.o ui/button.c
gcc $MUH_FLAGS -o ./obj/box.o ui/box.c
gcc $MUH_FLAGS -o ./obj/panel.o ui/panel.c
gcc $MUH_FLAGS -o ./obj/window.o ui/window.c
gcc $MUH_FLAGS -o ./obj/command_center.o ui/command_center.c
gcc $MUH_FLAGS -o ./obj/tree_viewer.o ui/tree_viewer.c
gcc $MUH_FLAGS -o ./obj/query_window.o ui/query_window.c

# link
gcc -lncurses -lsqlite3 -o ./bin/muhsql \
    ./obj/sqlite.o \
    ./obj/muhsql.o ./obj/db.o \
    ./obj/button.o ./obj/box.o ./obj/panel.o ./obj/window.o \
    ./obj/command_center.o ./obj/tree_viewer.o ./obj/query_window.o

# run
./bin/muhsql
