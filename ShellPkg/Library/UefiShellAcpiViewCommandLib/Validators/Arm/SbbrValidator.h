/** @file
  Header file for SbbrValidator.c

  Copyright (c) 2021, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Glossary:
    - Sbbr or SBBR   - Server Base Boot Requirements
    - Sbsa or SBSA   - Server Base System Architecture

  @par Reference(s):
    - Arm Server Base Boot Requirements 1.2, September 2019
    - Arm Server Base Boot Requirements 1.1, May 2018
    - Arm Server Base Boot Requirements 1.0, March 2016
    - Arm Server Base System Architecture 6.0
**/

#ifndef SBBR_VALIDATOR_H_
#define SBBR_VALIDATOR_H_

#include <IndustryStandard/Acpi.h>

/**
  Arm SBBR specification versions.
**/
typedef enum {
  ArmSbbrVersion_1_0 = 0,
  ArmSbbrVersion_1_1 = 1,
  ArmSbbrVersion_1_2 = 2,
  ArmSbbrVersionMax  = 3
} ARM_SBBR_VERSION;

/**
  The ACPI table instance counter.
**/
typedef struct AcpiTableCounter {
  CONST UINT32    Signature;      /// ACPI table signature
  UINT32          Count;          /// Instance count
} ACPI_TABLE_COUNTER;

/**
  ACPI table SBBR requirements.
**/
typedef struct AcpiSbbrReq {
  CONST UINT32    *Tables;       /// List of required tables
  CONST UINT32    TableCount;    /// Number of elements in Tables
} ACPI_SBBR_REQ;

#endif // SBBR_VALIDATOR_H_
