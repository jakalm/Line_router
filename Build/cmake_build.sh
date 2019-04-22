#!/bin/sh

# Clean up first
# Remove all files and directories except '.gitignore' and 'cmake_build.sh'
find . ! \( -name ".gitignore" -o -name "cmake_build.sh" -o -name "." \) -exec rm -rf {} +
rm ../Bin/*

# Configure
cmake ..

