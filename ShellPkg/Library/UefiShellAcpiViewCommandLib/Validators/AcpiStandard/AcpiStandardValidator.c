/** @file
  ACPI validations.

  Copyright (c) 2021, Arm Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include "AcpiParser.h"
#include "AcpiViewConfig.h"
#include "AcpiDataStore.h"

/**
  Test whether Id is unique among the IdList.

  @param [in]  Id          ID to check.
  @param [in]  IdList      List of already existing IDs.
  @param [in]  IdListSize  Size of IdList.

  @retval TRUE             Id does not exist in IdList.
  @retval FALSE            Id already exists in IdList.
**/
STATIC
BOOLEAN
IsIdUnique (
  IN CONST UINT32  Id,
  IN CONST UINT32  *IdList,
  IN CONST UINT32  IdListSize
  )
{
  UINT32  Index;

  for (Index = 0; Index < IdListSize; Index++) {
    if (IdList[Index] == Id) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
  Validate that processor ID's match across various ACPI tables.

  @retval EFI_NOT_FOUND          Processor data cannot be found.
  @retval EFI_INVALID_PARAMETER  Processor ID's are not valid.
  @retval EFI_SUCCESS            All processor ID's are valid.
**/
EFI_STATUS
EFIAPI
ValidateProcId (
  )
{
  EFI_STATUS                             Status;
  META_DATA_NODE                         *PpttListHead;
  META_DATA_NODE                         *PpttList;
  META_DATA_NODE                         *MadtListHead;
  META_DATA_NODE                         *MadtList;
  UINTN                                  MadtListLength;
  UINT32                                 *MadtIds;
  UINTN                                  NodeIndex;
  EFI_ACPI_6_4_GIC_STRUCTURE             MadtProc;
  EFI_ACPI_6_4_PPTT_STRUCTURE_PROCESSOR  PpttProc;

  NodeIndex = 0;

  Status = GetMetaDataListHead (MetaDataMadtGicC, &MadtListHead);
  if (EFI_ERROR (Status)) {
    if (Status != EFI_NOT_FOUND) {
      IncrementErrorCount ();
      Print (
        L"\nERROR: Cannot get MADT processor list." \
        L" Status = 0x%x.",
        Status
        );
    }

    return Status;
  }

  Status = GetMetaDataCount (MetaDataMadtGicC, &MadtListLength);
  if (EFI_ERROR (Status)) {
    IncrementErrorCount ();
    Print (
      L"\nERROR: Cannot get MADT processor list length." \
      L" Status = 0x%x.",
      Status
      );
    return Status;
  }

  Status = GetMetaDataListHead (MetaDataPpttProcs, &PpttListHead);
  if (EFI_ERROR (Status)) {
    if (Status != EFI_NOT_FOUND) {
      IncrementErrorCount ();
      Print (
        L"\nERROR: Cannot get PPTT processor list." \
        L" Status = 0x%x.",
        Status
        );
    }

    return Status;
  }

  MadtIds = AllocateZeroPool (MadtListLength * sizeof (*MadtIds));
  if (MadtIds == NULL) {
    IncrementErrorCount ();
    Print (L"\nERROR: Failed to allocate resources for MADT ID list.");
    return EFI_OUT_OF_RESOURCES;
  }

  MadtList = (META_DATA_NODE *)GetFirstNode (&MadtListHead->Link);

  // Extract MADT ID's from structures.
  while (!IsNull (&MadtListHead->Link, &MadtList->Link)) {
    MadtProc           = *(EFI_ACPI_6_4_GIC_STRUCTURE *)MadtList->Data;
    MadtIds[NodeIndex] = MadtProc.AcpiProcessorUid;

    MadtList = (META_DATA_NODE *)GetNextNode (
                                   &MadtListHead->Link,
                                   &MadtList->Link
                                   );
    NodeIndex++;
  }

  PpttList = (META_DATA_NODE *)GetFirstNode (&PpttListHead->Link);

  // Compare PPTT ID's against the MADT.
  while (!IsNull (&PpttListHead->Link, &PpttList->Link)) {
    PpttProc = *(EFI_ACPI_6_4_PPTT_STRUCTURE_PROCESSOR *)PpttList->Data;

    // Make sure this is a real processor and not a cluster.
    if (PpttProc.Flags.NodeIsALeaf == EFI_ACPI_6_4_PPTT_NODE_IS_LEAF) {
      // PpttProc.AcpiProcessorId must be found in our list of MADT ID's.
      if (IsIdUnique (PpttProc.AcpiProcessorId, MadtIds, MadtListLength)) {
        IncrementErrorCount ();
        Print (
          L"\nERROR: PPTT Processor ID %d is not found in the MADT.",
          PpttProc.AcpiProcessorId
          );
        Status = EFI_INVALID_PARAMETER;
      }
    }

    PpttList = (META_DATA_NODE *)GetNextNode (
                                   &PpttListHead->Link,
                                   &PpttList->Link
                                   );
  }

  FreePool (MadtIds);

  return Status;
}

/**
  Entry point for validator, used to run platform agnostic ACPI validations.
**/
VOID
EFIAPI
AcpiStandardValidate (
  VOID
  )
{
  EFI_STATUS  Status;

  if (GetReportOption () != ReportSelected) {
    Status = ValidateProcId ();
    if (EFI_ERROR (Status)) {
      Print (L"\nERROR: Validate processor ID failed.\n");
    }
  }
}
