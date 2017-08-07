/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/lib/p9_collect_suspend_ffdc.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017                             */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
#if 0 //  tmp dummy file to enable ppe manual mirror
#include <fapi2.H>
#include <p9_collect_suspend_ffdc.H>
#include <p9_sbe_ppe_ffdc.H>
#include <p9_ppe_defs.H>

#define NUM_SCOMS       28
#define NUM_LOC_VARS    19
#define NUM_BYTE_RANGES 52
#define CME_OFF         0x00000000BADCE0FF

uint32_t g_scom = 0;
uint32_t g_byte = 8;
uint64_t g_ffdcScoms[NUM_LOC_VARS];
uint32_t g_index = 0;

const uint32_t g_byteranges[NUM_BYTE_RANGES] =
{
    0x07, //PGPE XIR
    0x07, //PGPE XIR
    0x07, //PGPE XIR
    0x03, //PGPE SPR
    0x03, //PGPE SPR
    0x03, //PGPE SPR
    0x07, //SGPE XIR
    0x07, //SGPE XIR
    0x07, //SGPE XIR
    0x03, //SGPE SPR
    0x03, //SGPE SPR
    0x03, //SGPE SPR
    0x03, //OCCS2
    0x03, //OCCFLG
    0x00, 0x00, 0x00, 0x00, //OPIT2 C[0-3]
    0x00, 0x00, 0x00, 0x00, //OPIT2 C[4-7]
    0x00, 0x00, 0x00, 0x00, //OPIT2 C[8-11]
    0x00, 0x00, 0x00, 0x00, //OPIT2 C[12-15]
    0x00, 0x00, 0x00, 0x00, //OPIT2 C[16-19]
    0x00, 0x00, 0x00, 0x00, //OPIT2 C[20-23]
    0x01, //QCSR
    0x02,  //CCSR
    0x47, 0x47, 0x47, 0x47, 0x47, 0x47, //CME[0-5]  IAR
    0x47, 0x47, 0x47, 0x47, 0x47, 0x47  //CME[6-11] IAR
}; //
const uint64_t masks[8] =
{
    0xFF,
    0xFFFF,
    0xFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFFFF,
    0xFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF
};
uint64_t extract(uint64_t data, uint32_t low, uint32_t high)
{
    return ((data >> (56 - (high << 3))) & masks[(high - low)]);
}
void add_data(uint64_t l_data64)
{
    //first nibble hold2 low g_byte
    uint32_t low = g_byteranges[g_index] >> 4;
    //second nibble holds high g_byte
    uint32_t high = g_byteranges[g_index] & 0xFF;
    g_index++;
    uint32_t length = (high - low) + 1;
    //gets data between low and high g_byte inclusive
    uint64_t data = extract(l_data64, low, high);

    if(g_byte - length < 0)//if length of data greater than remaining space in current local variable
    {
        length -= g_byte;
        g_ffdcScoms[g_scom] |= (data >> (length << 3)); //stores what will fit in current variable
        data &= masks[length]; //sets up remaining data to be stored in next variable
        g_scom++;
        g_byte = 8;
    }

    g_ffdcScoms[g_scom] |= (data << ((length - g_byte) << 3)); //stores data in current variable
    g_byte -= length;

    if(!g_byte) //if no space left in current local variable move to next
    {
        g_scom++;
        g_byte = 8;
    }
}

fapi2::ReturnCode
p9_collect_suspend_ffdc (const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_INF (">> p9_collect_suspend_ffdc.exe");

    fapi2::buffer<uint64_t> l_data64;
    std::vector<uint64_t> ppe_regs;
    uint32_t sbe_index;

    for(sbe_index = 0; sbe_index < NUM_LOC_VARS; sbe_index++)
    {
        g_ffdcScoms[sbe_index] = 0;
    }

    ///Collect PGPE Data
    p9_sbe_ppe_ffdc ( i_target,
                      PGPE_BASE_ADDRESS,
                      ppe_regs );

    for(sbe_index = 0; sbe_index < ppe_regs.size(); sbe_index++)
    {
        add_data(ppe_regs[sbe_index]);
    }

    //Collect SGPE Data
    p9_sbe_ppe_ffdc ( i_target,
                      SGPE_BASE_ADDRESS,
                      ppe_regs );

    for(sbe_index = 0; sbe_index < ppe_regs.size(); sbe_index++)
    {
        add_data(ppe_regs[sbe_index]);
    }

    //Collect All other SCOMs
    uint32_t scoms[NUM_SCOMS]     =
    {
        PU_OCB_OCI_OCCS2_SCOM,
        PU_OCB_OCI_OCCFLG_SCOM,
        PU_OCB_OCI_OPIT2C0_SCOM, PU_OCB_OCI_OPIT2C1_SCOM, PU_OCB_OCI_OPIT2C2_SCOM, PU_OCB_OCI_OPIT2C3_SCOM,
        PU_OCB_OCI_OPIT2C4_SCOM, PU_OCB_OCI_OPIT2C5_SCOM, PU_OCB_OCI_OPIT2C6_SCOM, PU_OCB_OCI_OPIT2C7_SCOM,
        PU_OCB_OCI_OPIT2C8_SCOM, PU_OCB_OCI_OPIT2C9_SCOM, PU_OCB_OCI_OPIT2C10_SCOM, PU_OCB_OCI_OPIT2C11_SCOM,
        PU_OCB_OCI_OPIT2C12_SCOM, PU_OCB_OCI_OPIT2C13_SCOM, PU_OCB_OCI_OPIT2C14_SCOM, PU_OCB_OCI_OPIT2C15_SCOM,
        PU_OCB_OCI_OPIT2C16_SCOM, PU_OCB_OCI_OPIT2C17_SCOM, PU_OCB_OCI_OPIT2C18_SCOM, PU_OCB_OCI_OPIT2C19_SCOM,
        PU_OCB_OCI_OPIT2C20_SCOM, PU_OCB_OCI_OPIT2C21_SCOM, PU_OCB_OCI_OPIT2C22_SCOM, PU_OCB_OCI_OPIT2C23_SCOM,
        PU_OCB_OCI_QCSR_SCOM,
        PU_OCB_OCI_CCSR_SCOM
    };

    uint32_t getscom_index;

    for(getscom_index = 0; getscom_index < NUM_SCOMS; getscom_index++)
    {
        fapi2::getScom (     i_target,
                             scoms[getscom_index],
                             l_data64 );
        add_data(l_data64);

    }

    //Collect CME IARs
    const uint32_t SSHSRC[12]     =
    {
        EX_0_PPM_SSHSRC, EX_1_PPM_SSHSRC, EX_2_PPM_SSHSRC, EX_3_PPM_SSHSRC, EX_4_PPM_SSHSRC, EX_5_PPM_SSHSRC,
        EX_6_PPM_SSHSRC, EX_7_PPM_SSHSRC, EX_8_PPM_SSHSRC, EX_9_PPM_SSHSRC, EX_10_PPM_SSHSRC, EX_11_PPM_SSHSRC
    };
    const uint32_t DBGPRO[12]     =
    {
        EX_0_PPE_XIDBGPRO, EX_1_PPE_XIDBGPRO, EX_2_PPE_XIDBGPRO, EX_3_PPE_XIDBGPRO, EX_4_PPE_XIDBGPRO, EX_5_PPE_XIDBGPRO,
        EX_6_PPE_XIDBGPRO, EX_7_PPE_XIDBGPRO, EX_8_PPE_XIDBGPRO, EX_9_PPE_XIDBGPRO, EX_10_PPE_XIDBGPRO, EX_11_PPE_XIDBGPRO
    };

    uint32_t ex;

    for(ex = 0; ex < 12; ex++)
    {
        fapi2::getScom(      i_target,
                             SSHSRC[ex],
                             l_data64 );

        if(!(l_data64 >> 63))//is this ex stop gated?
        {
            fapi2::getScom(  i_target,
                             DBGPRO[ex],
                             l_data64 );
            add_data(l_data64);
        }
        else
        {
            add_data(CME_OFF); //inject data which informs cme is offline
        }
    }

    //Write out compacted SCOM data
    FAPI_ASSERT ( false,
                  fapi2::SUSPEND_FFDC()
                  .set_PROC_CHIP_TARGET                             (i_target)
                  .set_PGPE_XSR_AND_IAR                             (g_ffdcScoms[0])
                  .set_PGPE_IR_AND_EDR                              (g_ffdcScoms[1])
                  .set_PGPE_LR_AND_SPRG0                            (g_ffdcScoms[2])
                  .set_PGPE_SRR0_AND_SRR1                           (g_ffdcScoms[3])
                  .set_SGPE_XSR_AND_IAR                             (g_ffdcScoms[4])
                  .set_SGPE_IR_AND_EDR                              (g_ffdcScoms[5])
                  .set_SGPE_LR_AND_SPRG0                            (g_ffdcScoms[6])
                  .set_SGPE_SRR0_AND_SRR1                           (g_ffdcScoms[7])
                  .set_OCCS2_AND_OCCFLG                             (g_ffdcScoms[8])
                  .set_OPIT2C_0_THROUGH_7_B0                        (g_ffdcScoms[9])
                  .set_OPIT2C_8_THROUGH_15_B0                       (g_ffdcScoms[10])
                  .set_OPIT2C_16_THROUGH_23_B0                      (g_ffdcScoms[11])
                  .set_QCSR_B0_TO_B1_AND_CCSR_B0_TO_B2_AND_3B_EMPTY (g_ffdcScoms[12])
                  .set_CME0_IAR_AND_CME1_IAR                        (g_ffdcScoms[13])
                  .set_CME2_IAR_AND_CME3_IAR                        (g_ffdcScoms[14])
                  .set_CME4_IAR_AND_CME5_IAR                        (g_ffdcScoms[15])
                  .set_CME6_IAR_AND_CME7_IAR                        (g_ffdcScoms[16])
                  .set_CME8_IAR_AND_CME9_IAR                        (g_ffdcScoms[17])
                  .set_CME10_IAR_AND_CME11_IAR                      (g_ffdcScoms[18]),
                  "Suspend FFDC");

fapi_try_exit:
    FAPI_INF ("<< p9_collect_suspend_ffdc");
    return fapi2::current_err;
}

#endif
