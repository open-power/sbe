# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/import/chips/p10/procedures/utils/contained/tools/src/ringspinner/prcd_templates.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2020
# [+] International Business Machines Corp.
#
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied. See the License for the specific language governing
# permissions and limitations under the License.
#
# IBM_PROLOG_END_TAG
import logging
import jinja2


logger = logging.getLogger(__name__)


PROCEDURE = """{% if cpp_namespace %}namespace {{ cpp_namespace }}{{ ' {' }}{% endif %}
static inline fapi2::ReturnCode {{ fname }}(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV> &i_cplt,
    const uint64_t i_scan_region_type,
    {%- for var in spyvars %}
    const uint64_t {{ var }}{% if loop.last %}){% else %},{% endif %}
    {%- endfor %}
{
    fapi2::buffer<uint64_t> tmp = 0;

    FAPI_TRY(fapi2::putScom(i_cplt, scomt::perv::SCAN_REGION_TYPE, i_scan_region_type));
    FAPI_TRY(svs::scan64_put(i_cplt, 0x00BAD666BAD66600, 0));

    // ---
    {%- for insn in insns %}
    {
        {%- for expr in insn.exprs %}
        {{ expr }}{% if expr[-1] not in [';', '|', '&', '^', '{', '}'] and '//' not in expr %};{% endif %}
        {%- endfor %}
    }
    {%- endfor %}
    // ---

    if (tmp != 0x00BAD666BAD66600) {
        FAPI_ERR("Header mismatch: %016llx != %016llx", 0x00BAD666BAD66600, tmp);
        return fapi2::FAPI2_RC_FALSE;
    }

    FAPI_TRY(fapi2::putScom(i_cplt, scomt::perv::SCAN_REGION_TYPE, 0));

fapi_try_exit:
    return fapi2::current_err;
}
{% if cpp_namespace %}{{ '}' }}// namespace {{ cpp_namespace }}{% endif %}"""
PROCEDURE = jinja2.Template(PROCEDURE)


SCANIN = "FAPI_TRY(svs::scan64_put(i_cplt, {{ data }}, {{ numbits }}))"
SCANIN = jinja2.Template(SCANIN)


ROTATE = "FAPI_TRY(svs::rotate(i_cplt, {{ numbits }}, tmp))"
ROTATE = jinja2.Template(ROTATE)
