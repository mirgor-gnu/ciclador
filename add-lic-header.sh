#!/bin/bash

#for i in *.cc # or whatever other pattern...
for i in $(find src/ -name '*.c' -o -name '*.h');
do
  if ! grep -q "MIT License" $i
  then
    cat license.h $i >$i.new && mv $i.new $i
  fi
done

