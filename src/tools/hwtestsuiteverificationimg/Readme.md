# Hardware testing tool description


This document describes the testing methodology for verifying the end to end
functionality of V3 secure header on hardware.

We propose dividing the testing tool into two components:
* Image auto-generation tool
* hardware auto-test tool


## Image auto-generation tool

Image auto-generation tool which generate corrupted image as per test-case excel to output directory.

Tool will need child tools likes: `testxls2json.py`, `shv3parser.py`, `IPL_IMAGE`,
`ecc` tools

#### Usage:
```
> python3 imagegenerator.py -h
usage: imagegenerator.py [-h] -e EXCEL -l LOOKUP -t TESTXLS2JSON -s
                         SHPARSERTOOL -i IPLTOOL -b SEEPROMBIN [--ecc ECC]
                         [-o OUTPUT] [--skipHbbl SKIPHBBL]

Image generator

optional arguments:
  -h, --help            show this help message and exit
  -e EXCEL, --excel EXCEL
                        Excel file
  -l LOOKUP, --lookup LOOKUP
                        Look up file which have the mapping of Error and
                        status code
  -t TESTXLS2JSON, --testxls2json TESTXLS2JSON
                        Excel to json parse tool
  -s SHPARSERTOOL, --shparsertool SHPARSERTOOL
                        Secure header V3 parser tool
  -i IPLTOOL, --ipltool IPLTOOL
                        ipl image tool
  -b SEEPROMBIN, --seeprombin SEEPROMBIN
                        Seeprom Binary image
  --ecc ECC             Enable the ECC for all images
  -o OUTPUT, --output OUTPUT
                        Output path
  --skipHbbl SKIPHBBL   Skip HBBL

Example: python3 imagegenerator.py -e V3_Verification_Test_case.xlsx -l lookuptable.json -t testxls2json.py -s shv3parser.py -i $SBEROOT/images/ipl_image_tool -b $SBEROOT/sbe_seeprom_p10.bin --ecc  $SBEROOT/src/tools/utils/ecc -o v3_hw_test_image

```

`Note: Excel secure header section and field should match with code syntax`

Generate the images and convert the Excel test cases to JSON in the specified output directories
Example:
```
> ls -ltr output_dir
total 16392
-rw-rw-r-- 1 ranga ranga 343832 Sep  3 23:33 sbe_seeprom_tid5.bin  ---> test Images
-rw-rw-r-- 1 ranga ranga 343832 Sep  3 23:33 sbe_seeprom_tid6.bin
..
..
-rw-rw-r-- 1 ranga ranga 343832 Sep  3 23:33 sbe_seeprom_tid52.bin
drwxrwxr-x 2 ranga ranga   4096 Sep  3 23:33 temp           ---------> temporary directories
-rw-rw-r-- 1 ranga ranga  65111 Sep  3 23:33 testcase.json  ---------> convert the Excel test cases to JSON
```






