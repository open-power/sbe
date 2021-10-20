This document describes how we can test ECDSA Algorithm changes on a x86 machine.

Make changes to ECDSA algo as required in ecverify.c and .h.

1) Level 1: Test with known test vectors.
main.c has valid test vectors to test the changes.

To build:
make clean
make ecdsa

To run:
./ecdsa

2)Level 2: Test with NIST test vectors (for P-521,SHA-512)

a) Copy src/test/ecdsaAlgoTest/x86TestCase/NIST/NISTCombined.c into
src/test/ecdsaAlgoTest/x86TestCase/main.c.
b) Build :
    make clean
    make ecdsa
C) Run src/test/ecdsaAlgoTest/x86TestCase/testEcdsaNistVectors.py.
Ex: testEcdsaNistVectors.py <path_to_output_file.rpt>

NOTE: Output is printed on console.

3)Level 3: Test with random test vector (i.e with known pub key but random
message hash and signature)

a) Copy src/test/ecdsaAlgoTest/x86TestCase/RANDOM/random_main.c into
src/test/ecdsaAlgoTest/x86TestCase/main.c.
b) Build :
    make clean
    make ecdsa
C) Run src/test/ecdsaAlgoTest/x86TestCase/testEcdsaRandomVectors.py.
Ex: testEcdsaRandomVectors.py <path_to_output_file.rpt>

NOTE: Check .rpt file for result.

----------------------------------------------------------------------------------------
Once changes are good copy the changes into src/sbefw/verification/ecverify.H
and src/sbefw/verification/ecverify.C from current dir to test changes on HW.

NOTE: For HW testing steps refer src/test/ecdsaAlgoTest/hwTestCase/README.txt
