/** @file
  State and accessors for 'acpiview' configuration.

  Copyright (c) 2016 - 2021, ARM Limited. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>

#include "AcpiViewConfig.h"

// Report variables
STATIC BOOLEAN         mConsistencyCheck;
STATIC BOOLEAN         mColourHighlighting;
STATIC EREPORT_OPTION  mReportType;
STATIC BOOLEAN         mValidatorStatus;
STATIC UINTN           mValidatorId;

// User selection of which ACPI table should be checked
SELECTED_ACPI_TABLE  mSelectedAcpiTable;

/**
 Reset the AcpiView user configuration to defaults
**/
VOID
EFIAPI
AcpiConfigSetDefaults (
  VOID
  )
{
  mReportType              = ReportAll;
  mSelectedAcpiTable.Type  = 0;
  mSelectedAcpiTable.Name  = NULL;
  mSelectedAcpiTable.Found = FALSE;
  mConsistencyCheck        = TRUE;
  mValidatorStatus         = FALSE;
  mValidatorId             = 0;
}

/**
  This function converts a string to ACPI table signature.

  @param [in] Str   Pointer to the string to be converted to the
                    ACPI table signature.

  @retval The ACPI table signature.
**/
STATIC
UINT32
ConvertStrToAcpiSignature (
  IN CONST CHAR16  *Str
  )
{
  UINT8  Index;
  CHAR8  Ptr[4];

  ZeroMem (Ptr, sizeof (Ptr));
  Index = 0;

  // Convert to Upper case and convert to ASCII
  while ((Index < 4) && (Str[Index] != 0)) {
    if ((Str[Index] >= L'a') && (Str[Index] <= L'z')) {
      Ptr[Index] = (CHAR8)(Str[Index] - (L'a' - L'A'));
    } else {
      Ptr[Index] = (CHAR8)Str[Index];
    }

    Index++;
  }

  return *(UINT32 *)Ptr;
}

/**
  This function selects an ACPI table in current context.
  The string name of the table is converted into UINT32
  table signature.

  @param [in] TableName The name of the ACPI table to select.
**/
VOID
EFIAPI
SelectAcpiTable (
  IN CONST CHAR16  *TableName
  )
{
  ASSERT (TableName != NULL);

  mSelectedAcpiTable.Name = TableName;
  mSelectedAcpiTable.Type = ConvertStrToAcpiSignature (mSelectedAcpiTable.Name);
}

/**
  This function returns the selected ACPI table.

  @param [out] SelectedAcpiTable Pointer that will contain the returned struct.
**/
VOID
EFIAPI
GetSelectedAcpiTable (
  OUT SELECTED_ACPI_TABLE  **SelectedAcpiTable
  )
{
  *SelectedAcpiTable = &mSelectedAcpiTable;
}

/**
  This function returns the colour highlighting status.

  @retval TRUE Colour highlighting is enabled.
**/
BOOLEAN
EFIAPI
GetColourHighlighting (
  VOID
  )
{
  return mColourHighlighting;
}

/**
  This function sets the colour highlighting status.

  @param [in] Highlight The highlight status.
**/
VOID
EFIAPI
SetColourHighlighting (
  BOOLEAN  Highlight
  )
{
  mColourHighlighting = Highlight;
}

/**
  This function returns the consistency checking status.

  @retval TRUE Consistency checking is enabled.
**/
BOOLEAN
EFIAPI
GetConsistencyChecking (
  VOID
  )
{
  return mConsistencyCheck;
}

/**
  This function sets the consistency checking status.

  @param [in] ConsistencyChecking   The consistency checking status.
**/
VOID
EFIAPI
SetConsistencyChecking (
  BOOLEAN  ConsistencyChecking
  )
{
  mConsistencyCheck = ConsistencyChecking;
}

/**
  This function returns the report options.

  @return The current report option.
**/
EREPORT_OPTION
EFIAPI
GetReportOption (
  VOID
  )
{
  return mReportType;
}

/**
  This function sets the report options.

  @param [in] ReportType The report option to set.
**/
VOID
EFIAPI
SetReportOption (
  EREPORT_OPTION  ReportType
  )
{
  mReportType = ReportType;
}

/**
  Return the ValidatorStatus flag.

  @retval TRUE   Validator should be run.
  @retval FALSE  Validator should not be run.
 **/
BOOLEAN
EFIAPI
GetValidatorStatus (
  VOID
  )
{
  return mValidatorStatus;
}

/**
  Set the ValidatorStatus flag.

  @param [in] Status  Enable (True)/Disable (False) running the optional
                      Validator.
**/
VOID
EFIAPI
SetValidatorStatus (
  BOOLEAN  Status
  )
{
  mValidatorStatus = Status;
}

/**
  Return the ID of validator to run against the parsed ACPI tables.

  @return ID of validator to run.
**/
UINTN
EFIAPI
GetValidatorId (
  VOID
  )
{
  return mValidatorId;
}

/**
  Set the ID of the validator to run against the parsed ACPI tables.

  @param [in] ValidatorId  ID of validator.
**/
VOID
EFIAPI
SetValidatorId (
  UINTN  ValidatorId
  )
{
  mValidatorId = ValidatorId;
}
