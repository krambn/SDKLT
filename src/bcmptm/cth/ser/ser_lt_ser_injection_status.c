/*! \file ser_lt_ser_injection_status.c
 *
 * Functions used to get or update IMM LT SER_INJECTION_STATUS
 */
/*
 * Copyright: (c) 2018 Broadcom. All Rights Reserved. "Broadcom" refers to 
 * Broadcom Limited and/or its subsidiaries.
 * 
 * Broadcom Switch Software License
 * 
 * This license governs the use of the accompanying Broadcom software. Your 
 * use of the software indicates your acceptance of the terms and conditions 
 * of this license. If you do not agree to the terms and conditions of this 
 * license, do not use the software.
 * 1. Definitions
 *    "Licensor" means any person or entity that distributes its Work.
 *    "Software" means the original work of authorship made available under 
 *    this license.
 *    "Work" means the Software and any additions to or derivative works of 
 *    the Software that are made available under this license.
 *    The terms "reproduce," "reproduction," "derivative works," and 
 *    "distribution" have the meaning as provided under U.S. copyright law.
 *    Works, including the Software, are "made available" under this license 
 *    by including in or with the Work either (a) a copyright notice 
 *    referencing the applicability of this license to the Work, or (b) a copy 
 *    of this license.
 * 2. Grant of Copyright License
 *    Subject to the terms and conditions of this license, each Licensor 
 *    grants to you a perpetual, worldwide, non-exclusive, and royalty-free 
 *    copyright license to reproduce, prepare derivative works of, publicly 
 *    display, publicly perform, sublicense and distribute its Work and any 
 *    resulting derivative works in any form.
 * 3. Grant of Patent License
 *    Subject to the terms and conditions of this license, each Licensor 
 *    grants to you a perpetual, worldwide, non-exclusive, and royalty-free 
 *    patent license to make, have made, use, offer to sell, sell, import, and 
 *    otherwise transfer its Work, in whole or in part. This patent license 
 *    applies only to the patent claims licensable by Licensor that would be 
 *    infringed by Licensor's Work (or portion thereof) individually and 
 *    excluding any combinations with any other materials or technology.
 *    If you institute patent litigation against any Licensor (including a 
 *    cross-claim or counterclaim in a lawsuit) to enforce any patents that 
 *    you allege are infringed by any Work, then your patent license from such 
 *    Licensor to the Work shall terminate as of the date such litigation is 
 *    filed.
 * 4. Redistribution
 *    You may reproduce or distribute the Work only if (a) you do so under 
 *    this License, (b) you include a complete copy of this License with your 
 *    distribution, and (c) you retain without modification any copyright, 
 *    patent, trademark, or attribution notices that are present in the Work.
 * 5. Derivative Works
 *    You may specify that additional or different terms apply to the use, 
 *    reproduction, and distribution of your derivative works of the Work 
 *    ("Your Terms") only if (a) Your Terms provide that the limitations of 
 *    Section 7 apply to your derivative works, and (b) you identify the 
 *    specific derivative works that are subject to Your Terms. 
 *    Notwithstanding Your Terms, this license (including the redistribution 
 *    requirements in Section 4) will continue to apply to the Work itself.
 * 6. Trademarks
 *    This license does not grant any rights to use any Licensor's or its 
 *    affiliates' names, logos, or trademarks, except as necessary to 
 *    reproduce the notices described in this license.
 * 7. Limitations
 *    Platform. The Work and any derivative works thereof may only be used, or 
 *    intended for use, with a Broadcom switch integrated circuit.
 *    No Reverse Engineering. You will not use the Work to disassemble, 
 *    reverse engineer, decompile, or attempt to ascertain the underlying 
 *    technology of a Broadcom switch integrated circuit.
 * 8. Termination
 *    If you violate any term of this license, then your rights under this 
 *    license (including the license grants of Sections 2 and 3) will 
 *    terminate immediately.
 * 9. Disclaimer of Warranty
 *    THE WORK IS PROVIDED "AS IS" WITHOUT WARRANTIES OR CONDITIONS OF ANY 
 *    KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WARRANTIES OR CONDITIONS OF 
 *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE OR 
 *    NON-INFRINGEMENT. YOU BEAR THE RISK OF UNDERTAKING ANY ACTIVITIES UNDER 
 *    THIS LICENSE. SOME STATES' CONSUMER LAWS DO NOT ALLOW EXCLUSION OF AN 
 *    IMPLIED WARRANTY, SO THIS DISCLAIMER MAY NOT APPLY TO YOU.
 * 10. Limitation of Liability
 *    EXCEPT AS PROHIBITED BY APPLICABLE LAW, IN NO EVENT AND UNDER NO LEGAL 
 *    THEORY, WHETHER IN TORT (INCLUDING NEGLIGENCE), CONTRACT, OR OTHERWISE 
 *    SHALL ANY LICENSOR BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY DIRECT, 
 *    INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF 
 *    OR RELATED TO THIS LICENSE, THE USE OR INABILITY TO USE THE WORK 
 *    (INCLUDING BUT NOT LIMITED TO LOSS OF GOODWILL, BUSINESS INTERRUPTION, 
 *    LOST PROFITS OR DATA, COMPUTER FAILURE OR MALFUNCTION, OR ANY OTHER 
 *    COMMERCIAL DAMAGES OR LOSSES), EVEN IF THE LICENSOR HAS BEEN ADVISED OF 
 *    THE POSSIBILITY OF SUCH DAMAGES.
 */

/******************************************************************************
 * Includes
 */
#include <bcmimm/bcmimm_int_comp.h>
#include <bsl/bsl.h>
#include <shr/shr_debug.h>
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmptm/bcmptm_ser_cth_internal.h>

#include "ser_lts.h"
#include "ser_lt_ser_injection_status.h"

/******************************************************************************
 * Defines
 */
#define  BSL_LOG_MODULE  BSL_LS_BCMPTM_SER


/******************************************************************************
 * Typedefs
 */

/******************************************************************************
 * Private variables
 */
/* Cache for IMM LT SER_INJECTION_STATUS */
static bcmptm_ser_lt_field_info_t lt_ser_injection_status_cache[] = {
    { SER_INJECTION_STATUSt_PT_IDf, {0}, INVALID_PT_ID },
    { SER_INJECTION_STATUSt_PT_INSTANCEf, {0} },
    { SER_INJECTION_STATUSt_PT_INDEXf, {0} },
    { SER_INJECTION_STATUSt_XOR_BANKf, {0} },
    { SER_INJECTION_STATUSt_SER_ERR_INJECTEDf, {0} },
    { SER_INJECTION_STATUSt_SER_ERR_CORRECTEDf, {0} }
};
static uint32_t lt_ser_injection_status_field_num = COUNTOF(lt_ser_injection_status_cache);

/******************************************************************************
 * Private Functions
 */

/******************************************************************************
 * bcmptm_ser_injection_status_fid_check:
 */
static int
bcmptm_ser_injection_status_fid_check(int unit, uint32_t fid_lt)
{
    /* new field is added to LT */
    if (fid_lt >= lt_ser_injection_status_field_num) {
        return SHR_E_INTERNAL;
    }
    /*
    * When field is inserted/deleted to/from SER_CONTROL LT,
    * and lt_ser_control_cache is not updated accordingly.
    */
    if (lt_ser_injection_status_cache[fid_lt].lt_fid != fid_lt) {
        return SHR_E_INTERNAL;
    }
    return SHR_E_NONE;
}

/******************************************************************************
 * Public Functions
 */

/******************************************************************************
 * bcmptm_ser_injection_status_cache_update: update cache
 * of LT SER_INJECTION_STATUS by SER component
 */
int
bcmptm_ser_injection_status_cache_update(int unit, uint32_t fid_lt, uint32_t data)
{
    int rv = SHR_E_NONE;
    bcmptm_ser_lt_field_info_t *cache_ptr = &lt_ser_injection_status_cache[0];

    SHR_FUNC_ENTER(unit);

    rv = bcmptm_ser_injection_status_fid_check(unit, fid_lt);
    SHR_IF_ERR_EXIT(rv);

    cache_ptr[fid_lt].data[unit] = data;

exit:
    if (SHR_FAILURE(rv)) {
        LOG_ERROR(BSL_LOG_MODULE,
                  (BSL_META_U(unit,
                              "\t Fail to update data to cache of SER_INJECTION_STATUSt\n")));
    }
    SHR_FUNC_EXIT();
}

/******************************************************************************
 * bcmptm_ser_injection_status_imm_update: commit cache data
 * to IMM LT SER_INJECTION_STATUS by SER component
 */
int
bcmptm_ser_injection_status_imm_update(int unit, int insert)
{
    shr_fmm_t** fld_array = NULL;
    int rv = SHR_E_NONE;
    size_t i = 0, update_idx = 0;
    uint32_t fld_num = lt_ser_injection_status_field_num;
    bcmltd_fields_t lt_flds;
    bcmptm_ser_lt_field_info_t *cache_ptr = &lt_ser_injection_status_cache[0];

    SHR_FUNC_ENTER(unit);

    fld_array = bcmptm_ser_imm_flds_allocate(unit, fld_num);
    if (fld_array == NULL) {
        return SHR_E_MEMORY;
    }
    lt_flds.count = fld_num;
    lt_flds.field = fld_array;
    for (i = 0; i < fld_num; i++) {
        /* first time to access SER_INJECTION_STATUSt */
        if (insert) {
            cache_ptr[i].data[unit] = cache_ptr[i].default_val;
        }
        bcmptm_ser_imm_fld_set(lt_flds, update_idx++, cache_ptr[i].lt_fid,
                               0, cache_ptr[i].data[unit]);
    }
    if (insert == 0) {
        rv = bcmimm_entry_update(unit, true, SER_INJECTION_STATUSt, &lt_flds);
    } else {
        rv = bcmimm_entry_insert(unit, SER_INJECTION_STATUSt, &lt_flds);
    }
    SHR_IF_ERR_EXIT(rv);

exit:
    bcmptm_ser_imm_flds_free(unit, fld_array, fld_num);
    if (SHR_FAILURE(rv)) {
        LOG_ERROR(BSL_LOG_MODULE,
                  (BSL_META_U(unit,
                              "\t Fail to commit data to SER_INJECTION_STATUSt\n")));
    }
    SHR_FUNC_EXIT();
}

