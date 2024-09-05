# Hardware testing tool description


This document describes the testing methodology for verifying the end to end
functionality of V3 secure header on hardware.

We propose dividing the testing tool into two components:
* Image auto-generation tool
* Hardware auto-test tool


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

Example: python3 imagegenerator.py -e V3_Verification_Test_case.xlsx -l lookuptable.json -t testxls2json.py -s shv3parser.py -i $SBEROOT/images/ipl_image_tool -b $SBEROOT/sbe_seeprom_p10.bin --ecc  $SBEROOT/src/tools/utils/ecc -o ouputdir

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


Generate the images and convert the Excel test cases to JSON in the specified
output directories
Example;
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


## Hardware auto-test tool

Hardware auto-test tool which run the test according to input (json). Create the
testresult along with test in the specified output directories

Copy

`Note: Before run the tool make sure system SBE is booted`


#### Usage:
```
> python3 hw_verificationtestsuite.py -h
usage: Hardware - verification code secure header V2 test suite on Hardware
       [-h] [--ppe PPE] [--sim SIM] [-j JSON] [-i IMGPATH] [--testid TESTID]
       [--skiphbbl SKIPHBBL] [-o OUTPUT]
       chip[.unit]

positional arguments:
  chip[.unit]           Target containing desired PPE

optional arguments:
  -h, --help            show this help message and exit
  --ppe PPE             PPE to talk to, defaults to first PPE matching the
                        target
  --sim SIM             Use simulation shortcuts (yes/no/true/false/1/0),
                        default: detect based on eCmd target
  -j JSON, --json JSON  testcase json file (.json)
  -i IMGPATH, --imgpath IMGPATH
                        Image path / Image directory
  --testid TESTID       continue from given test id number
  --skiphbbl SKIPHBBL   skip hbbl testcase
  -o OUTPUT, --output OUTPUT
                        Output JSON

Cronus arguments like -p#, -a#, -all, -debug are also supported.

Example: python3 hw_verificationtestsuite.py -j ouputdir/testcase.json -i ouputdir/ -o test_result.json pu -p0
```


## Steps

* Run `Image auto-generation tool` in gfw machine
* Copy the full directory which is created by `Image auto-generation tool` to
   LCB machine
* Copy the `Hardware auto-test tool` to LCB machine
* Make sure system is booted
* Run `Hardware auto-test tool`

