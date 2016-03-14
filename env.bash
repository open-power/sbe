ROOTDIR=.
export SBEROOT=`pwd`

if [ -e ./customrc ]; then
    source ./customrc
fi

export PATH=${PATH}:${SBEROOT}/import/chips/p9/xip/bin/
export PATH=${PATH}:${SBEROOT}/tools/ppetracepp/

if [ -n "${SANDBOXROOT}" ]; then
    if [ -n "${SANDBOXNAME}" ]; then
        export SANDBOXBASE="${SANDBOXROOT}/${SANDBOXNAME}"
    fi
fi

export DEFAULT_MACHINE=nimbus
