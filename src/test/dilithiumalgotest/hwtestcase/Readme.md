# ReadMe

## Validating dilithium on HW / SIMICS using KAT

### This document describes validation algo on HW / SIMICS with KAT

### Flow-Diagram
![x86_flow-diagram](./hw_flow_diagram.png)


Test with KAT (Known Answer Test) vectors.
main.c expects two arguments: input_file (KAT file) and output_file (result).
The make command (build) will generate an executable file named hw_dilithium.
For execute: `/hw_dilithium $SBEROOT/src/import/security/algo/crypto/test/PQCsignKAT_4896.rsp hw_kat_bin.bin`
Will generate binary file of max size 300KB each.

### Part A: (Image build process)
1. **Path**: `src/test/dilithiumalgotest/hwtestcase`
2. **To build**:
    * `make`
        * rebuild: `make -B`
        * clean  : `make clean`
        * build  : `make hw_dilithium` (another way of build)

3. **To run**:
    * ```./hw_dilithium $SBEROOT/src/import/security/algo/crypto/test/KAT/PQCsignKAT_4896.rsp hw_kat_bin.bin```

    > Tool changes the output file name hw_kat_bin_0.bin, hw_kat_bin_1.bin ...
    > each file max of 300KB

4. **Copy sbesecureboot.C**:
    * Copy `sbesecureboot.C` to `src/sbefw/verification/sbesecureboot.C`

5. **Build Project**:
    * `make install -j32` (From $SBEROOT level)
    * `make clean` (clean)

6. **Generate HW img**:
    * `./sb cronus`

7. **Append img**:

    1. `images/ipl_image_tool images/v1signed/sbe_seeprom_DD1.bin delete .base`

    2. `images/ipl_image_tool images/v1signed/sbe_seeprom_DD1.bin append .base src/test/dilithiumalgotest/hwtestcase/hw_kat_bin_0.bin`

        * images/ipl_image_tool images/v1signed/sbe_seeprom_DD1.bin append .base src/test/dilithiumalgotest/hwtestcase/hw_kat_bin_0.bin (image1)
        * images/ipl_image_tool images/v1signed/sbe_seeprom_DD1.bin append .base src/test/dilithiumalgotest/hwtestcase/hw_kat_bin_1.bin (image2)
        * images/ipl_image_tool images/v1signed/sbe_seeprom_DD1.bin append .base src/test/dilithiumalgotest/hwtestcase/hw_kat_bin_2.bin (image3)


8. **Copy required file to lcb machine**:

    1. `cp images/v1signed/sbe_seeprom_DD1.bin <lcb_path>/`

    2. `cp images/sbe_verification.syms <lcb_path>/sbe_DD1.syms`

    3. `cp images/sbeVerificationStringFile <lcb_path>/sbeStringFile_DD1`


### Part B: (HW test Process)
1. **Login to cronus**:
    Login to LCB machine

2. **Flash image to HW**:
    a. `setconfig SPI_ACCESS_MODE dd`
    b. `setconfig HW_IMAGE_PATH <image_path>`
    c. `setconfig GLOBAL_DEBUG 6.U6.HU.BU.P`
    d. `setconfig USE_SEEPROM_FASTLOAD on`
    e. `setconfig USE_SBE_FIFO off`
    f. `crorecoversbe -p0`

3. **Run HW**:
    * `istep -s0`

4. **Get trace**:
    Once SBE boot failure, get the trace
    * `./tool/getSbeTrace -t SBE -i <string_file_path> -o "VSBE"`
        * Before executing above command below step have to follow
            * <string_file_path> should contain listed tools: fsp-trace `getSbeTrace`, `ppe2fsp`, `sbe_DD1.syms`, `sbe-debug.py`, `sbe_seeprom_DD1.bin`,  `sbeStringFile_DD1`

5. Above process has to be repeat for all binary kat file


* **Help**:
    * `./dilithium -h`
        * ```
            Usage: ./hw_dilithium [input_file output_file_name] [-h]
            This program convert KAT vectors to Binary file

            positional arguments:
                input_file output_file            Convert given KAT file to output Binary file
                    Optional arg - output_file

            Optional Arguments:
            -h                                  Print this help message.

            Example:
            ./hw_dilithium input.txt output.bin```