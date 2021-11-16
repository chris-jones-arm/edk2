# Validators Framework

The validators framework allows for simple and easy post-parsing validation to
take place. This framework gives AcpiView the ability to perform more complex
validations by allowing validators access to ACPI data from all parsed tables
at the same time.

This framework is intended to be both generic and extensible to allow anyone to
create their own validators with minimal changes needed to parsers and the
AcpiView internal application code.

## Running ACPI Validators

The ACPI standard validator is always run after the parsing stage in AcpiView.
Additional validators can also be run by using the command line flag
`-r ValidatorId`, more information on this flag can be found in the AcpiView
help string (`acpiview -?`).

## Cross-table Validations

The validators framework provides the ability to perform more complex
validations, on ACPI data, than single field validation. By storing the data
during the parsing stage it can later be used to perform validations using data
from multiple tables. This is especially useful with ACPI fields of variable
length as the storage and validation is handled dynamically at runtime.

# ACPI Data Store

The ACPI data store allows for any type of ACPI data to be stored and later
accessed via the relevant methods in `AcpiDataStore.h`.

`StoreAcpiMetaData(ListType, NodeType, Ptr, Length)`:
   Called during the parsing stage to store relevant ACPI meta data as needed
   for validation later. Each call stores one "packet" of meta data, of the
   given length, in a linked list of the given ListType. NodeType can be used
   to store nodes of different types within the same linked list.

`GetMetaDataListHead(Type, Node)`:
   Called during the validation stage to retrieve ACPI meta data stored in the
   ACPI data store. The head of a linked list is returned that contains all
   stored meta data of the given type.

   Note: `GetMetaDataListHead()` returns the *head* of a linked list of meta
   data. This head node is created upon initialisation and does not contain
   any meta data itself, therefore
   `(META_DATA_NODE*)GetFirstNode (&ListHead->Link);` should be used before
   trying to dereference any meta data.

## Example usage

An example parser storing an ACPI table in the data store.
```C
// Parser.c

// Our example table signature to store. This could be any data type as it will
// be casted to VOID* in the data store.
UINT32 Signature = SIGNATURE_32('A', 'B', 'C', 'D');

// Store our table signature into our ACPI data store.
Status = StoreAcpiMetaData (
           MetaDataExampleSig,
           MetaDataExampleNodeSig,
           &Signature,
           sizeof (Siganture)
           );
if (EFI_ERROR (Status)) {
   return Status;
}
```

An example validator retrieving and using meta data from the ACPI data store:
```C
// Validator.c

META_DATA_NODE *ListHead;
META_DATA_NODE *Node;

// Retrieve a list of installed tables from the data store.
Status = GetMetaDataListHead (MetaDataInstalledTables, &ListHead);
if (EFI_ERROR (Status)) {
   return Status;
}

// The first node is just a list head and contains no data, get the first
// "real" node in the list.
Node = (META_DATA_NODE*)GetFirstNode (&ListHead->Link);

// Iterate through nodes and perform some validations with the data.
while (!IsNull (&ListHead->Link, &Node->Link)) {
   // ...
}
```

## Structure

The ACPI data store structure is an array containing linked lists of related
META_DATA_NODE's. These nodes are grouped according to their META_DATA_TYPE
with each META_DATA_TYPE pertaining to one linked list of META_DATA_NODEs.

Each linked list of meta nodes is implemented via the standard doubly linked
list implementation in MdePkg. This allows any of the standard traversal,
query and accessing functions located in `MdePkg/Library/BaseLib/LinkedList.c`
to be used on the linked list returned from calls to `GetMetaDataListHead()`.

An example ACPI data store structure can be seen here:

|---------------------------|
|     MetaDataPpttProcs     | ---> Proc0 ---> Proc1
|---------------------------|
|     MetaDataMadtGicC      | ---> Proc0 ---> Proc1
|---------------------------|
|  MetaDataInstalledTables  | ---> "RSDP" ---> "XSDT" ---> "HMAT" ---> "PPTT"
|---------------------------|
