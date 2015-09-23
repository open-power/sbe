#!/bin/sh
if [ -z $SBE_CI_ENV_SETUP ];
then
    unset $SANDBOXBASE
    unset $SANDBOXNAME
    source "$SBEROOT/sbe/build/citest/setup-env"
fi

#   Front end to autocitest - script to execute unit tests under simics.
#
##  when jenkins runs it will create a workspace with the built code tree
##  and drop us into it.
autocitest ${BACKING_BUILD}

exit $?
