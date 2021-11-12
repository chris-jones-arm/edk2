/** @file
  ACPI post-parsing validation framework.

  Copyright (c) 2021, Arm Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include "AcpiValidation.h"

/**
  List of all validators that can be run.
**/
ACPI_VALIDATOR  mValidatorList[] = {
  { ValidatorIdSbbr10,       Sbbr10Validate       },
  { ValidatorIdSbbr11,       Sbbr11Validate       },
  { ValidatorIdSbbr12,       Sbbr12Validate       },
  { ValidatorIdAcpiStandard, AcpiStandardValidate }
};

/**
  Run the validator with the given ValidatorId.

  @param [in] ValidatorId  The ID of the validator to run.
**/
VOID
EFIAPI
RunValidator (
  IN UINTN  ValidatorId
  )
{
  ACPI_VALIDATOR  *Validator;

  if (ValidatorId >= ValidatorIdMax) {
    Print (
      L"\nValidatorId is not recognised." \
      L" ValidatorId = %d.",
      ValidatorId
      );
    return;
  }

  Validator = &mValidatorList[ValidatorId];
  if ((Validator == NULL) || (Validator->Id != ValidatorId)) {
    Print (
      L"\nValidator cannot be retrieved." \
      L" ValidatorId = %d.",
      ValidatorId
      );
    return;
  }

  Validator->ValidatorProc ();
}
