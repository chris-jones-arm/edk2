/** @file
  Header file for storing and accessing ACPI data from parsers.

  Copyright (c) 2021, Arm Limited. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef ACPI_DATA_STORE_H_
#define ACPI_DATA_STORE_H_

/**
  Types of data that can be stored and accessed in the ACPI data store.
**/
typedef enum MetaDataType {
  MetaDataPpttProcs       = 0,  ///< List of all PPTT processor structures.
  MetaDataMadtGicC        = 1,  ///< List of all MADT GICC structures.
  MetaDataInstalledTables = 2,  ///< Signatures of all installed ACPI tables.
  MetaDataMax
} META_DATA_TYPE;

/**
  A node containing data about an ACPI table.
**/
typedef struct MetaDataNode {
  LIST_ENTRY        Link;   ///< Linked list entry.
  META_DATA_TYPE    Type;   ///< Type of meta data.
  UINT8             Length; ///< Length of meta data.
  VOID              *Data;  ///< Pointer to the meta data.
} META_DATA_NODE;

/**
  Initialise the ACPI data store.
**/
VOID
EFIAPI
InitAcpiDataStore (
  VOID
  );

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
  );

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
  );

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
  );

/**
  Free all ACPI data currently stored in the data store.
**/
VOID
EFIAPI
FreeAcpiDataStore (
  VOID
  );

#endif
