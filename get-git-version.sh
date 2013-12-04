#!/bin/bash

# when we get tags, make it possible to get a cleaner output, like the
# current release (tag)

printf "%q\n" "$( git log \
    --pretty="format:%ci, author: \"%aN\" <%aE>, commit: %h" -1 "${1}" || \
    echo no git \
)"
