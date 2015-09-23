ROOTDIR=.

if [ -e ./customrc ]; then
    source ./customrc
fi

export PATH=${PATH}:`pwd`/tools/ppetracepp

export SBEROOT=`pwd`

if [ -n "${SANDBOXROOT}" ]; then
    if [ -n "${SANDBOXNAME}" ]; then
        export SANDBOXBASE="${SANDBOXROOT}/${SANDBOXNAME}"
    fi
fi

export DEFAULT_MACHINE=nimbus
