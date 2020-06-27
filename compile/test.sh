#!/bin/bash
for i in ls html; do cat header.html | xargs -I '{}' sed 's/{HEADER}/{}/g' html/$i; done