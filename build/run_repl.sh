#! /bin/env bash

ROOT_PATH=`pwd`
PIP_PACKAGE_DIRS=`ls ${ROOT_PATH}/../ | grep pip_deps.*`

PYTHONPATH=""
for PIP_PACKAGE in ${PIP_PACKAGE_DIRS}
do
    PYTHONPATH="../${PIP_PACKAGE}/site-packages":${PYTHONPATH}
done

PYTHONPATH=${PYTHONPATH} python3 -S -s
