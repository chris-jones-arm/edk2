/** @file
  Storing and accessing ACPI data collected from parsers.

  Copyright (c) 2021, Arm Limited. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <IndustryStandard/Acpi.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include "AcpiDataStore.h"

STATIC META_DATA_NODE  mAcpiData[MetaDataMax];

/**
  Initialise the ACPI data store.
**/
VOID
EFIAPI
InitAcpiDataStore (
  VOID
  )
{
  UINTN  Index;

  for (Index = 0; Index < MetaDataMax; Index++) {
    InitializeListHead (&mAcpiData[Index].Link);
  }
}

/**
  Return the number of meta data nodes in a linked list of meta data.

  @param [in]  Type      META_DATA_TYPE of data to get length of.
  @param [out] Length    Length of the linked list.

  @retval EFI_NOT_FOUND  ACPI data with the given type cannot be found.
  @retval EFI_SUCCESS    Successfully returned the length of the linked list.
**/
EFI_STATUS
EFIAPI
GetMetaDataCount (
  IN META_DATA_TYPE  Type,
  OUT UINTN          *Length
  )
{
  META_DATA_NODE  *Node;

  *Length = 0;

  if (Type >= MetaDataMax) {
    Print (L"ERROR: Meta data type is not recognised.\n");
    return EFI_INVALID_PARAMETER;
  }

  Node = (META_DATA_NODE *)GetFirstNode (&mAcpiData[Type].Link);

  while (!IsNull (&mAcpiData[Type].Link, &Node->Link)) {
    (*Length)++;
    Node = (META_DATA_NODE *)GetNextNode (&mAcpiData[Type].Link, &Node->Link);
  }

  return EFI_SUCCESS;
}

/**
  Get ACPI meta data of the given type.

  @param [in]  Type        META_DATA_TYPE of data to return.
  @param [out] ListHead    The head of a linked list of META_DATA_NODE's.

  @retval EFI_INVALID_PARAMETER  A parameter is invalid.
  @retval EFI_NOT_FOUND          ACPI data with the given type cannot be found.
  @retval EFI_SUCCESS            Successfully returned the desired list of Nodes.
**/
EFI_STATUS
EFIAPI
GetMetaDataListHead (
  IN  META_DATA_TYPE  Type,
  OUT META_DATA_NODE  **ListHead
  )
{
  if (Type >= MetaDataMax) {
    Print (L"ERROR: Meta data type is not recognised.\n");
    return EFI_INVALID_PARAMETER;
  }

  if (ListHead == NULL) {
    Print (L"ERROR: List head is NULL.\n");
    return EFI_INVALID_PARAMETER;
  }

  if (IsListEmpty (&mAcpiData[Type].Link)) {
    *ListHead = NULL;
    return EFI_NOT_FOUND;
  }

  *ListHead = &mAcpiData[Type];

  return EFI_SUCCESS;
}

/**
  Store ACPI meta data as a new node in the data store.

  @param [in] ListType          Type of linked list to store the data in.
  @param [in] NodeType          Type to give to the node that stores the data.
  @param [in] Ptr               Pointer to the data to store.
  @param [in] Length            Length of the data being stored (Ptr).

  @retval EFI_OUT_OF_RESOURCES  Not enough resources to allocate the data.
  @retval EFI_SUCCESS           Successfully stored the data.
**/
EFI_STATUS
EFIAPI
StoreAcpiMetaData (
  IN META_DATA_TYPE  ListType,
  IN META_DATA_TYPE  NodeType,
  IN VOID            *Ptr,
  IN UINT8           Length
  )
{
  META_DATA_NODE  *Node;
  UINT8           *Data;

  if ((ListType >= MetaDataMax) || (NodeType >= MetaDataMax)) {
    Print (L"ERROR: Meta data type is not recognised.\n");
    return EFI_INVALID_PARAMETER;
  }

  Node = AllocateZeroPool (sizeof (META_DATA_NODE));
  if (Node == NULL) {
    Print (L"ERROR: Failed to allocate resources for new node.\n");
    return EFI_OUT_OF_RESOURCES;
  }

  // Allocate and assign memory for the data to be stored.
  Data = AllocateCopyPool (Length, Ptr);
  if (Data == NULL) {
    FreePool (Node);
    Print (L"ERROR: Failed to allocate resources for node data.\n");
    return EFI_OUT_OF_RESOURCES;
  }

  // Assign fields.
  Node->Type   = NodeType;
  Node->Data   = Data;
  Node->Length = Length;

  InsertTailList (&mAcpiData[ListType].Link, &Node->Link);

  return EFI_SUCCESS;
}

/**
  Free all ACPI data currently stored in the data store.
**/
VOID
EFIAPI
FreeAcpiDataStore (
  VOID
  )
{
  UINTN           Index;
  META_DATA_NODE  *Node;
  META_DATA_NODE  *TmpNode;

  for (Index = 0; Index < MetaDataMax; Index++) {
    Node = (META_DATA_NODE *)GetFirstNode (&mAcpiData[Index].Link);

    while (!IsNull (&mAcpiData[Index].Link, &Node->Link)) {
      TmpNode = Node;
      Node    = (META_DATA_NODE *)RemoveEntryList (&TmpNode->Link);
      FreePool (TmpNode->Data);
      FreePool (TmpNode);
    }
  }
}
