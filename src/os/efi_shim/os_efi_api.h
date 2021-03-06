/*
 * Copyright (c) 2018, Intel Corporation.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OS_EFI_API_H_
#define	OS_EFI_API_H_

#include <Uefi.h>
#include <stdio.h>
#include <Dimm.h>
#include <UefiBaseType.h>
#include <FwUtility.h>

typedef struct _pass_thru_record_req
{
  UINT32 DimmId;
  UINT8 Opcode;
  UINT8 SubOpcode;
  UINT32 InputPayloadSize;
  UINT8 Input[];
}pass_thru_record_req;


typedef struct _pass_thru_record_resp
{
  EFI_STATUS PassthruReturnCode;
  UINT32 DimmId;
  UINT8 Status;
  UINT32 OutputPayloadSize;
  UINT8 Output[];
}pass_thru_record_resp;

VOID
EFIAPI
GetVendorDriverVersion(CHAR16 * pVersion, UINTN VersionStrSize);

/**
Saves a copy of a table to a file

@param[in] destFile - the file to save the passed table to
@param[in] table - the table to save

@retval EFI_SUCCESS  The count was returned properly
@retval Other errors failure of io
**/
EFI_STATUS
save_table_to_file(
  IN char* destFile,
  IN EFI_ACPI_DESCRIPTION_HEADER *table
);

/**
Loads a copy of a table from a file

@param[in] sourceFile - the file to save the passed table to
@param[out] size - the size of the table loaded
@param[out] table - EFI_ACPI_DESCRIPTION_HEADER the table

@retval EFI_SUCCESS  The count was returned properly
@retval Other errors failure of io
**/
EFI_STATUS
load_table_from_file(
  IN char* sourceFile,
  OUT EFI_ACPI_DESCRIPTION_HEADER ** table
);

/**
Obtains a copy of the NFIT table

@param[out] bufferSize the size of the returned buffer
@param[out] table - EFI_ACPI_DESCRIPTION_HEADER the table

@retval EFI_SUCCESS  The count was returned properly
@retval Other errors failure of io
**/
EFI_STATUS
get_nfit_table(
  OUT EFI_ACPI_DESCRIPTION_HEADER** pTable
);

/**
Obtains a copy of the PCAT table

@param[out] bufferSize the size of the returned buffer
@param[out] table - EFI_ACPI_DESCRIPTION_HEADER the table

@retval EFI_SUCCESS  The count was returned properly
@retval Other errors failure of io
**/
EFI_STATUS
get_pcat_table(
  OUT EFI_ACPI_DESCRIPTION_HEADER ** table
);

/**
Obtains a copy of the PMTT table

@param[out] bufferSize the size of the returned buffer
@param[out] table - EFI_ACPI_DESCRIPTION_HEADER the table

@retval EFI_SUCCESS  The count was returned properly
@retval Other errors failure of io
**/
EFI_STATUS
get_pmtt_table(
  OUT EFI_ACPI_DESCRIPTION_HEADER ** table
);

/**
Obtains a copy of the SMBIOS table

@retval NVM_SUCCESS or error code
**/
UINT32
get_smbios_table(
);

/**
Makes Bios emulated pass thru call and returns the values

@param[in]  pDimm    pointer to current Dimm
@param[out] pBsrValue   Value from passthru

@retval EFI_SUCCESS  The count was returned properly
@retval EFI_INVALID_PARAMETER One or more parameters are NULL
@retval Other errors failure of FW commands
**/
EFI_STATUS
EFIAPI
FwCmdGetBsr(DIMM *pDimm, UINT64 *pBsrValue);

/**

provides os-specific passthru functionality which also applies 
recording/playback as specified

@param[in]  pDimm    pointer to current Dimm
@param[in, out]  pCmd    pointer to command data
@param[in]  Timeout    the command timeout
**/
EFI_STATUS
passthru_os(
  IN     struct _DIMM *pDimm,
  IN OUT FW_CMD *pCmd,
  IN     long Timeout
);

/**
provides playback functionality

@param[in]  pDimm    pointer to current Dimm
**/
EFI_STATUS
passthru_playback(
  IN OUT FW_CMD *pCmd
);

/**
provides the start of record functionality
(prepares the files to record the transactions)

@param[in]  pDimm    pointer to current Dimm
**/

EFI_STATUS
passthru_record_setup(
  FILE **f_passthru_ptr,
  IN OUT FW_CMD *pCmd
);


/**
completes recording functionality started in passthru_record_setup

@param[in]  file    pointer to the file
@param[in]  pDimm    pointer to current Dimm
@param[in]  DimmID    the dimm id in context
**/
EFI_STATUS
passthru_record_finalize(
  FILE * file,
  IN OUT FW_CMD *pCmd,
  UINT32 DimmID,
  EFI_STATUS PassthruReturnCode
);

#endif //OS_EFI_API_H_
