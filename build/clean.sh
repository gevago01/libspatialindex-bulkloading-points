#!/bin/bash

#delete old build
ls | grep -i make| xargs rm -rf
rm Thesis results.txt compile_commands.json out.dot graph.png
