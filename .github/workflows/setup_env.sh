#!/bin/bash

#-------------------------------------------------------------------------------
# Functions

# Suppress echo (-x) output of commands executed with `quiet`.
# Useful for sourcing files, loading modules, spack, etc.
# set +x, set -x are not echo'd.
quiet() {
    { set +x; } 2> /dev/null;
    $@;
    set -x
}

# `section` is like `echo`, but suppresses output of the command itself.
# https://superuser.com/a/1141026
print_section() {
    builtin echo "$*"
    date
    case "${save_flags}" in
        (*x*)  set -x
    esac
}
alias section='{ save_flags="$-"; set +x; } 2> /dev/null; print_section'


#-------------------------------------------------------------------------------
quiet source /etc/profile

hostname && pwd
export top=$(pwd)

shopt -s expand_aliases


section "======================================== Load compiler"
quiet module load gcc@7.3.0
quiet module load intel-mkl

if [ "${device}" = "gpu_nvidia" ]; then
    section "======================================== Load CUDA"
    export CUDA_HOME=/usr/local/cuda/
    export PATH=${PATH}:${CUDA_HOME}/bin
    export CPATH=${CPATH}:${CUDA_HOME}/include
    export LIBRARY_PATH=${LIBRARY_PATH}:${CUDA_HOME}/lib64
    which nvcc
    nvcc --version
fi

if [ "${device}" = "gpu_amd" ]; then
    section "======================================== Load ROCm"
    export PATH=${PATH}:/opt/rocm/bin
    export CPATH=${CPATH}:/opt/rocm/include
    export LIBRARY_PATH=${LIBRARY_PATH}:/opt/rocm/lib:/opt/rocm/lib64
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/opt/rocm/lib:/opt/rocm/lib64
    which hipcc
    hipcc --version
fi

if [ "${maker}" = "cmake" ]; then
    section "======================================== Load cmake"
    quiet module load cmake
    which cmake
    cmake --version
    cd build
fi
