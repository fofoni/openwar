#!/bin/bash
git log --pretty="format:%ci, author: %aN <%aE>, commit: %h" -1 "${1}" || echo no git
