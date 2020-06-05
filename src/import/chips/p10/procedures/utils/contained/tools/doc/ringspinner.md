Ringspinner
===========
Ringspinner generates procedures to set SPYs within scanrings. The generated
procedures take the `uint64_t` SPY values, apply some bitops magic, and spin the
scanring using the `SCAN64` register. Ringspinner replaces tedious manual and
error-prone programming to set SPYs via scanning in procedures. Generated
procedures are optimized to use as few SCOMS as possible by grouping as many SPY
bits as possible into single `SCAN64` operations (highly dependent on ring
layout). The final output is always a single header file.

Usage
-----
Ringspinner consumes a YAML file with content such as:

    # Sample YAML
    ec10: # namespace
        engd_data: $PROJECT_ROOT/chips/p10/engd/p10/10 # path to ENGD directory
        functions: # list of function names (procedures) to generate
            ringspin: # function name
                spy_names: # function input SPYs
                    - ECP.L3.L3_MISC.L3CERRS.BACKING_L3_CTL_REG
                    - ECP.L3.L3_MISC.L3CERRS.MODE_REG0
                    - ECP.L3.L3_MISC.L3CERRS.MODE_REG1

    ec20: # namespace
        engd_data: $PROJECT_ROOT/chips/p10/engd/p10/20 # path to ENGD directory
        functions: # list of function names (procedures) to generate
            ringspin: # function name
                spy_names: # function input SPYs
                    - ECP.L3.L3_MISC.L3CERRS.BACKING_L3_CTL_REG
            ringspin2: # function name
                spy_names: # function input SPYs
                    - ECP.L3.L3_MISC.L3CERRS.MODE_REG0
                    - ECP.L3.L3_MISC.L3CERRS.MODE_REG1

In the above sample YAML file, ringspinner is directed to generate a total of
three procedures within two unique namespaces. To run ringspinner:

    bin/ringspinner <YAML> -o <base filename>

For example, to generate the header file `ringspin.H` from YAML file
`ringspin.yaml`, the ringspinner invocation is:

    bin/ringspinner ringspin.yaml -o ringspin

Note that the `-o` argument is the *base* filename. Ringspinner uses the base
filename to generate header guards and appends the `.H` suffix to the final
header file.

Generated procedure prototypes look like this (first function from the sample
YAML above):

    namespace ec10 {
    static inline fapi2::ReturnCode ec_l3_func_ringspin(
        const fapi2::Target<fapi2::TARGET_TYPE_PERV> &i_cplt,
        const uint64_t i_scan_region_type,
        const uint64_t i_ecp_l3_l3_misc_l3cerrs_backing_l3_ctl_reg,
        const uint64_t i_ecp_l3_l3_misc_l3cerrs_mode_reg0,
        const uint64_t i_ecp_l3_l3_misc_l3cerrs_mode_reg1) {...}
    ... } // namespace ec10

The generated procedure is named `<ring_name>_<function_name>`. Ringspinner
generates a procedure for each ring if the input SPYs to a function span
multiple rings. Generated procedures always take a `fapi2::TARGET_TYPE_PERV`
target and `SCAN_REGION_TYPE` argument.

Notes
-----
There is a simple testing script in `src/ringspinner/test/`. The test can be run
with `src/ringspinner/test/ringspin.test.sh` from a suitable checkpoint (ie.
EQ00 `EC_L3_FUNC` ring in scanable state) on AWAN (or MESA but that's slooow).
The test sets some SPYs in the EQ00 `EC_L3_FUNC` ring using a
ringspinner-generated procedure and checks the resulting scanring using the
ECMD/CRONUS `getspy` utility.

Summary of how to run test:
    1. Load an AWAN/MESA simulation checkpoint where the EQ00 `EC_L3_FUNC` ring
       is in a scanable state
    2. Set `PROJECT_ROOT` environment variable
    3. Potentially modify `src/ringspinner/test/ringsping.test.yaml` to point to
       ENGD which matches the loaded simulation model
    4. Run `src/ringspinner/test/ringsping.test.sh`
