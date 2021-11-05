/** @file
  Header file for post-parsing APCI validation.

  Copyright (c) 2021, Arm Limited. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef ACPI_VALIDATION_H_
#define ACPI_VALIDATION_H_

/**
  ID's for all known validators.
**/
typedef enum {
  ValidatorIdAcpiStandard = 0,     ///< Platform agnostic ACPI spec checks.
  ValidatorIdMax          = 1
} VALIDATOR_ID;

/**
  A function pointer to the entry point of a validator.
**/
typedef VOID (EFIAPI *EDKII_ACPI_VALIDATOR_PROC)(VOID);

/**
  A validator is made up of an ID and function pointer to the validators entry
  point.
**/
typedef struct AcpiValidator {
  VALIDATOR_ID                 Id;
  EDKII_ACPI_VALIDATOR_PROC    ValidatorProc;
} ACPI_VALIDATOR;

/**
  Run the validator with the given ValidatorId.

  @param [in] ValidatorId  The ID of the validator to run.
**/
VOID
EFIAPI
RunValidator (
  IN UINTN  ValidatorId
  );

/**
  Definition in Acpi/AcpiValidator.c
**/
VOID
EFIAPI
AcpiStandardValidate (
  VOID
  );

#endif
