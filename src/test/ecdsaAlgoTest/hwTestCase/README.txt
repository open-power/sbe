This document describes how we can test ECDSA Algorithm changes on HW.

After testing the changes as described in src/test/ecdsaAlgoTest/README.txt,
copy the algo changes into src/sbefw/verification/ecverify.H and
src/sbefw/verification/ecverify.C.
Copy sbesecureboot.C in current dir into src/sbefw/verification/.

1) Testing ECDSA changes with NIST test vectors on HW:
Build PPE image:
make clean
make install -j32

Append the NIST test vector binary into PPE image.
./ipl_image_tool sbe_seeprom_DD1.bin append .sbh_firmware <NIST_TEST_VECTOR.bin>

NOTE:
1) NIST_TEST_VECTOR.bin can be found in below path
   src/test/ecdsaAlgoTest/x86TestCase/NIST/NIST_test_vectors.bin.
2) NIST_test_vectors.bin is generated as a part of x86 testcase.

Flash image on HW.
Once test is completed PPE is halted so as to check the result by grabbing SBE
traces.
All NIST test vectors are good vectors.

2) Repeat above 1) procedure by modifying NIST_test_vectors.bin and check if
algo verification fails.


NOTE:
NIST Test vectors file can be found
src/test/ecdsaAlgoTest/x86TestCase/NIST/SigGenComponent.txt.

Download Path:
https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Algorithm-Validation-Program/documents/components/186-3ecdsasiggencomponenttestvectors.zip
