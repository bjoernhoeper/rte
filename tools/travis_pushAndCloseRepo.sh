#!/bin/bash
cd rte-www
cp ../rte/logs/* ./logs/.
mkdir ./logs/tmpSafe
cp -v `find ./logs/ -maxdepth 1  -name 'acplt_build*.log' -type f -printf '%T@ %p\n' | sort -n | tail -50 | cut -f2- -d" " ` ./logs/tmpSafe/.
rm -v `find ./logs/ -maxdepth 1 -name 'acplt_build*.log' -type f`
mv -v ./logs/tmpSafe/* ./logs/.
rm -vR ./logs/tmpSafe
git add -A ./logs/*
git config --global user.email "rte_bot@outlook.de"
git config --global user.name "rte-bot"
git config --global push.default simple
git commit -am "updated generated documentation (texinfo) on webpage and created releases by travis-ci [ci skip]"
git push -f -q https://$GH_TOKEN:x-oauth-basic@github.com/acplt/rte-www
cd ..
