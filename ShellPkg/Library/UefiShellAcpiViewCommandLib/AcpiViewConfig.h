/** @file
  Header file for 'acpiview' configuration.

  Copyright (c) 2016 - 2021, ARM Limited. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef ACPI_VIEW_CONFIG_H_
#define ACPI_VIEW_CONFIG_H_

/**
  This function returns the colour highlighting status.

  @retval TRUE Colour highlighting is enabled.
**/
BOOLEAN
EFIAPI
GetColourHighlighting (
  VOID
  );

/**
  This function sets the colour highlighting status.

  @param [in] Highlight The highlight status.
**/
VOID
EFIAPI
SetColourHighlighting (
  BOOLEAN  Highlight
  );

/**
  This function returns the consistency checking status.

  @retval TRUE Consistency checking is enabled.
**/
BOOLEAN
EFIAPI
GetConsistencyChecking (
  VOID
  );

/**
  This function sets the consistency checking status.

  @param [in] ConsistencyChecking   The consistency checking status.
**/
VOID
EFIAPI
SetConsistencyChecking (
  BOOLEAN  ConsistencyChecking
  );

/**
  Return the ValidatorStatus flag.

  @retval TRUE   Validator should be run.
  @retval FALSE  Validator should not be run.
 **/
BOOLEAN
EFIAPI
GetValidatorStatus (
  VOID
  );

/**
  Set the ValidatorStatus flag.

  @param [in] Status  Enable (True)/Disable (False) running the optional
                      Validator.
**/
VOID
EFIAPI
SetValidatorStatus (
  BOOLEAN  Status
  );

/**
  Return the ID of validator to run against the parsed ACPI tables.

  @return ID of validator to run.
**/
UINTN
EFIAPI
GetValidatorId (
  VOID
  );

/**
  Set the ID of the validator to run against the parsed ACPI tables.

  @param [in] ValidatorId  ID of validator.
**/
VOID
EFIAPI
SetValidatorId (
  UINTN  ValidatorId
  );

/**
  The EREPORT_OPTION enum describes ACPI table Reporting options.
**/
typedef enum {
  ReportAll,          ///< Report All tables.
  ReportSelected,     ///< Report Selected table.
  ReportTableList,    ///< Report List of tables.
  ReportDumpBinFile,  ///< Dump selected table to a file.
  ReportMax,
} EREPORT_OPTION;

/**
  This function returns the report options.

  @return The current report option.
**/
EREPORT_OPTION
EFIAPI
GetReportOption (
  VOID
  );

/**
  This function sets the report options.

  @param [in] ReportType The report option to set.
**/
VOID
EFIAPI
SetReportOption (
  EREPORT_OPTION  ReportType
  );

/**
  A structure holding the user selection detailing which
  ACPI table is to be examined by the AcpiView code.
**/
typedef struct {
  UINT32          Type;        ///< 32bit signature of the selected ACPI table.
  CONST CHAR16    *Name;       ///< User friendly name of the selected ACPI table.
  BOOLEAN         Found;       ///< The selected table has been found in the system.
} SELECTED_ACPI_TABLE;

/**
  This function returns the selected ACPI table.

  @param [out] SelectedAcpiTable Pointer that will contain the returned struct.
**/
VOID
EFIAPI
GetSelectedAcpiTable (
  OUT SELECTED_ACPI_TABLE  **SelectedAcpiTable
  );

/**
  This function selects an ACPI table in current context.
  The string name of the table is converted into UINT32
  table signature.

  @param [in] TableName The name of the ACPI table to select.
**/
VOID
EFIAPI
SelectAcpiTable (
  CONST CHAR16  *TableName
  );

/**
  Reset the AcpiView user configuration to defaults.
**/
VOID
EFIAPI
AcpiConfigSetDefaults (
  VOID
  );

#endif // ACPI_VIEW_CONFIG_H_
