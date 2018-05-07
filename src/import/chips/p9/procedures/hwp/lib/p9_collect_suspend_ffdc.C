/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/lib/p9_collect_suspend_ffdc.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2018                        */
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
#include <fapi2.H>
#include <p9_collect_suspend_ffdc.H>
#include <p9_misc_scom_addresses.H>
#include <p9_quad_scom_addresses.H>

#ifdef DD2 // PPE RAMming is supported on SBE only for DD2
    #include <p9_sbe_ppe_ffdc.H>
    #include <p9_ppe_defs.H>
#endif

#define NUM_SCOMS       28
#define NUM_LOC_VARS    19
#define NUM_BYTE_RANGES 52
#define CME_OFF         0x00000000BADCE0FF

extern "C"
{

    const uint32_t byteranges[NUM_BYTE_RANGES] =
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
        0x01, // QCSR
        0x05, // CCSR 2B for QCSR 3B for CCSR and balance 3B are left unused
        0x47, 0x47, 0x47, 0x47, 0x47, 0x47, //CME[0-5]  IAR
        0x47, 0x47, 0x47, 0x47, 0x47, 0x47  //CME[6-11] IAR
    }; //

//------------------------------------------------------------------------------------------------------

    uint64_t extract(uint64_t data, uint32_t low, uint32_t high)
    {
        uint32_t bitshift = (high - low) * 8;
        return ((data >> (56 - (high << 3))) & (0xFFFFFFFFFFFFFFFF >> (56 - bitshift)));
    }

//------------------------------------------------------------------------------------------------------

    void add_data(uint64_t l_data64, uint32_t& index, uint32_t& byte, uint32_t& scom, uint64_t* ffdcScoms)
    {
        //first nibble hold2 low byte
        uint32_t low = byteranges[index] >> 4;
        //second nibble holds high byte
        uint32_t high = byteranges[index] & 0x0F;
        index++;
        uint32_t length = (high - low) + 1;
        //gets data between low and high byte inclusive
        uint64_t data = extract(l_data64, low, high);

        if(byte - length < 0)//if length of data greater than remaining space in current local variable
        {
            length -= byte;
            ffdcScoms[scom] |= (data >> (length << 3)); //stores what will fit in current variable
            uint32_t bitshift = (high - low) * 8;
            data &= (0xFFFFFFFFFFFFFFFF >> (56 - bitshift)); //sets up remaining data to be stored in next variable
            scom++;
            byte = 8;
        }

        ffdcScoms[scom] |= (data << ((byte - length) << 3)); //stores data in current variable
        byte -= length;

        if(!byte) //if no space left in current local variable move to next
        {
            scom++;
            byte = 8;
        }
    }

//------------------------------------------------------------------------------------------------------

    fapi2::ReturnCode
    p9_collect_suspend_ffdc ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target )
    {
        FAPI_INF (">> p9_collect_suspend_ffdc.exe");
        uint32_t scom = 0;
        uint32_t byte = 8;
        uint64_t ffdcScoms[NUM_LOC_VARS];
        uint32_t index = 0;
        uint32_t sbe_index;

        fapi2::buffer<uint64_t> l_data64;
        std::vector<uint64_t> ppe_regs;

        //Init array meant for compressed FFDC collection with zero.

        for( sbe_index = 0; sbe_index < NUM_LOC_VARS; sbe_index++ )
        {
            ffdcScoms[sbe_index]    =   0x00;
        }

// SBE does not support ppe ffdc on DD1
#ifdef DD2
        ///Collect PGPE Data
        p9_sbe_ppe_ffdc ( i_target,
                          PGPE_BASE_ADDRESS,
                          ppe_regs );

        for(sbe_index = 0; sbe_index < ppe_regs.size(); sbe_index++)
        {
            add_data(ppe_regs[sbe_index], index, byte, scom, ffdcScoms);
        }

        //Collect SGPE Data
        p9_sbe_ppe_ffdc ( i_target,
                          SGPE_BASE_ADDRESS,
                          ppe_regs );

        for(sbe_index = 0; sbe_index < ppe_regs.size(); sbe_index++)
        {
            add_data(ppe_regs[sbe_index], index, byte, scom, ffdcScoms);
        }

#else
        // Each call to p9_sbe_ppe_ffdc would add REG_FFDC_IDX_MAX elements.
        // These are defaulted in DD1 on SBE, so simply account for those in the
        // ffdcScoms index by incrementing accordingly
        scom += 8; // (REG_FFDC_IDX_MAX * 2);
        index = 12;
#endif // DD2

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

            add_data(l_data64, index, byte, scom, ffdcScoms);

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
                add_data(l_data64, index, byte, scom, ffdcScoms);
            }
            else
            {
                add_data(CME_OFF, index, byte, scom, ffdcScoms); //inject data which informs cme is offline
            }
        }

        //Write out compacted SCOM data
        FAPI_ASSERT ( false,
                      fapi2::SUSPEND_FFDC()
                      .set_PROC_CHIP_TARGET                             (i_target)
                      .set_PGPE_XSR_AND_IAR                             (ffdcScoms[0])
                      .set_PGPE_IR_AND_EDR                              (ffdcScoms[1])
                      .set_PGPE_LR_AND_SPRG0                            (ffdcScoms[2])
                      .set_PGPE_SRR0_AND_SRR1                           (ffdcScoms[3])
                      .set_SGPE_XSR_AND_IAR                             (ffdcScoms[4])
                      .set_SGPE_IR_AND_EDR                              (ffdcScoms[5])
                      .set_SGPE_LR_AND_SPRG0                            (ffdcScoms[6])
                      .set_SGPE_SRR0_AND_SRR1                           (ffdcScoms[7])
                      .set_OCCS2_AND_OCCFLG                             (ffdcScoms[8])
                      .set_OPIT2C_0_THROUGH_7_B0                        (ffdcScoms[9])
                      .set_OPIT2C_8_THROUGH_15_B0                       (ffdcScoms[10])
                      .set_OPIT2C_16_THROUGH_23_B0                      (ffdcScoms[11])
                      .set_QCSR_B0_TO_B1_AND_CCSR_B0_TO_B2_AND_3B_EMPTY (ffdcScoms[12])
                      .set_CME0_IAR_AND_CME1_IAR                        (ffdcScoms[13])
                      .set_CME2_IAR_AND_CME3_IAR                        (ffdcScoms[14])
                      .set_CME4_IAR_AND_CME5_IAR                        (ffdcScoms[15])
                      .set_CME6_IAR_AND_CME7_IAR                        (ffdcScoms[16])
                      .set_CME8_IAR_AND_CME9_IAR                        (ffdcScoms[17])
                      .set_CME10_IAR_AND_CME11_IAR                      (ffdcScoms[18]),
                      "Suspend FFDC");

    fapi_try_exit:
        FAPI_INF ("<< p9_collect_suspend_ffdc");

        return fapi2::current_err;
    }

} //extern "C"
