/*
 * Copyright (c) 2018, Intel Corporation.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _NVMTABLES_H_
#define _NVMTABLES_H_

#include <Types.h>

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4200 )
#endif

/**
 * @defgroup NFIT_TABLE_TYPES NVDIMM Firmware Interface Table (NFIT) types
 * @{
**/

#define NVDIMM_SPA_RANGE_TYPE                 0   ///< SPA Range table
#define NVDIMM_NVDIMM_REGION_TYPE             1   ///< NVDIMM Region type table
#define NVDIMM_INTERLEAVE_TYPE                2   ///< Interleave type table
#define NVDIMM_SMBIOS_MGMT_INFO_TYPE          3   ///< SMBIOS MGMT Info type table
#define NVDIMM_CONTROL_REGION_TYPE            4   ///< Control Region table
#define NVDIMM_BW_DATA_WINDOW_REGION_TYPE     5   ///< BW Data Window Region table
#define NVDIMM_FLUSH_HINT_TYPE                6   ///< Flush Hint table
#define NVDIMM_PLATFORM_CAPABILITIES_TYPE     7   ///< Platform Capabilities (PCAT) table

/**
 * @}
 * Fields offsets in structures
 **/
#define NFIT_TABLE_HEADER_LENGTH_OFFSET              2
#define NFIT_TABLE_HEADER_LENGTH_FIELD_SIZE          2
#define PCAT_TABLE_HEADER_CHECKSUM_OFFSET            9
#define PCAT_TABLE_HEADER_LENGTH_OFFSET              2
#define PCAT_TABLE_HEADER_LENGTH_FIELD_SIZE          2

/** PCAT table types **/
#define PCAT_TYPE_PLATFORM_CAPABILITY_INFO_TABLE      0
#define PCAT_TYPE_INTERLEAVE_CAPABILITY_INFO_TABLE    1
#define PCAT_TYPE_RUNTIME_INTERFACE_TABLE             2
#define PCAT_TYPE_CONFIG_MANAGEMENT_ATTRIBUTES_TABLE  3
#define PCAT_TYPE_SOCKET_SKU_INFO_TABLE               6

#define NUMBER_OF_CHANNEL_WAYS_BITS_NUM  9

#define PCAT_HEADER_REVISION_1 1
#define PCAT_HEADER_REVISION_2 2

/** PMTT table types**/
#define	PMTT_MAX_LEN	4096
#define PMTT_TYPE_SOCKET 0
#define PMTT_TYPE_iMC 1
#define PMTT_TYPE_MODULE 2
#define PMTT_COMMON_HDR_LEN 8
#define PMTT_DDR_DCPMEM_FLAG BIT2
#define PMTT_INVALID_SMBIOS_HANDLE 0xFFFFFFFF

/** NFIT Tables structures **/
#pragma pack(push)
#pragma pack(1)

/** NFIT sub-table header */
typedef struct {
  UINT16 Type;        ///< sub-table type
  UINT16 Length;      ///< sub-table length
} SubTableHeader;

/** ACPI table header. **/
typedef struct {
  UINT32 Signature;       //!< ACPI table signature
  UINT32 Length;          //!< Length in bytes for entire table. It implies the number of Entry fields at the end of the table
  UINT8 Revision;         //!< table revision
  UINT8 Checksum;         //!< Entire table must sum to zero
  UINT8 OemId[6];         //!< OEM ID
  UINT64 OemTableId;      //!< the table ID is the manufacturer model ID
  UINT32 OemRevision;     //!< OEM revision of table for supplied OEM table ID
  UINT32 CreatorId;       //!< Vendor ID of utility that created the table
  UINT32 CreatorRevision; //!< Revision of utility that created the table
} TABLE_HEADER;

/** NFIT table header */
typedef struct {
  TABLE_HEADER Header;    ///< NFIT Header
  UINT8 Reserved[4];      ///< Reserved
} NFitHeader;

/** SPA Range Table */
typedef struct {
  SubTableHeader Header;                      ///< Header
  UINT16 SpaRangeDescriptionTableIndex;       ///< SPA Range Description Table index
  UINT16 Flags;                               ///< Flags
  UINT8 Reserved[4];                          ///< Reserved
  UINT32 ProximityDomain;                     ///< Proximity Domain
  GUID AddressRangeTypeGuid;                  ///< Address Range Type GUID
  UINT64 SystemPhysicalAddressRangeBase;      ///< System Physical Address Range Base
  UINT64 SystemPhysicalAddressRangeLength;    ///< Systems Physical Address Range Length
  UINT64 AddressRangeMemoryMappingAttribute;  ///< Address Range Memory Mapping Attributes
} SpaRangeTbl;

/** NFIT Device Handle */
typedef union {
  struct {
    UINT32 DimmNumber:4;          ///< DIMM Number
    UINT32 MemChannel:4;          ///< Memory Channel
    UINT32 MemControllerId:4;     ///< Memory Controller ID
    UINT32 SocketId:4;            ///< Socket ID
    UINT32 NodeControllerId:12;   ///< Node Controller ID
    UINT32 Reserved:4;            ///< Reserved
  } NfitDeviceHandle;
  UINT32 AsUint32;                ///< Combined value
} NfitDeviceHandle;

/** DIMM Region table */
typedef struct {
  SubTableHeader Header;                              ///< Header
  NfitDeviceHandle DeviceHandle;                      ///< DIMM Handle
  UINT16 NvDimmPhysicalId;                            ///< Physical ID
  UINT16 NvDimmRegionalId;                            ///< Region ID
  UINT16 SpaRangeDescriptionTableIndex;               ///< SPA Range Description table index
  UINT16 NvdimmControlRegionDescriptorTableIndex;     ///< Control Region Descriptor table index
  UINT64 NvDimmRegionSize;                            ///< Region Size
  UINT64 RegionOffset;                                ///< Region Offset
  UINT64 NvDimmPhysicalAddressRegionBase;             ///< Physical Address Range Base
  UINT16 InterleaveStructureIndex;                    ///< Interleave Structure Index
  UINT16 InterleaveWays;                              ///< Interleave Ways
  UINT16 NvDimmStateFlags;                            ///< State Flags
  UINT8 Reserved[2];                                  ///< Reserved
} NvDimmRegionTbl;

/** Interleave table */
typedef struct {
  SubTableHeader Header;            ///< Header
  UINT16 InterleaveStructureIndex;  ///< Interleave structure index
  UINT8 Reserved[2];                ///< Reserved
  UINT32 NumberOfLinesDescribed;    ///< Number of lines described
  UINT32 LineSize;                  ///< Line size
  UINT32 LinesOffsets[0];           ///< Line offsets
} InterleaveStruct;

/** SMBIOS table*/
typedef struct {
  SubTableHeader Header;            ///< Header
  UINT8 Reserved[4];                ///< Reserved
  UINT8 Data[0];                    ///< Data
} SmbiosTbl;

/** Control Region Table */
typedef struct {
  SubTableHeader Header;                                  ///< Header
  UINT16 ControlRegionDescriptorTableIndex;               ///< Control Region Descriptor Table index
  UINT16 VendorId;                                        ///< Vendor ID
  UINT16 DeviceId;                                        ///< Device ID
  UINT16 Rid;                                             ///< Revsion ID
  UINT16 SubsystemVendorId;                               ///< Subsystem Vendor ID
  UINT16 SubsystemDeviceId;                               ///< Subsystem Device ID
  UINT16 SubsystemRid;                                    ///< Subsystem Revision ID
  UINT8 ValidFields;                                      ///< Valid Fields
  UINT8 ManufacturingLocation;                            ///< Manfacturing Location
  UINT16 ManufacturingDate;                               ///< Manufacturing Date
  UINT8 Reserved[2];                                      ///< Reserved
  UINT32 SerialNumber;                                    ///< Serial Number
  UINT16 RegionFormatInterfaceCode;                       ///< Region format interface code
  UINT16 NumberOfBlockControlWindows;                     ///< Number of block control windows
  UINT64 SizeOfBlockControlWindow;                        ///< Size of block control window
  UINT64 CommandRegisterOffsetInBlockControlWindow;       ///< Command register offset in block control window
  UINT64 SizeOfCommandRegisterInBlockControlWindows;      ///< Size of command register
  UINT64 StatusRegisterOffsetInBlockControlWindow;        ///< Status register offset in block control window
  UINT64 SizeOfStatusRegisterInBlockControlWindows;       ///< Size of status register
  UINT16 ControlRegionFlag;                               ///< Control region flags
  UINT8 Reserved1[6];                                     ///< Reserved
} ControlRegionTbl;

/** BW Region table */
typedef struct {
  SubTableHeader Header;                                  ///< Header
  UINT16 ControlRegionStructureIndex;                     ///< Control region structure index
  UINT16 NumberOfBlockDataWindows;                        ///< Number of block data windows
  UINT64 BlockDataWindowStartLogicalOffset;               ///< Block data window starting logical offset
  UINT64 SizeOfBlockDataWindow;                           ///< Size of block data window
  UINT64 AccessibleBlockCapacity;                         ///< Accessible block capacity
  UINT64 AccessibleBlockCapacityStartAddress;             ///< Accessible block capacity start address
} BWRegionTbl;

/** Flush Hint table */
typedef struct {
  SubTableHeader Header;                ///< Header
  NfitDeviceHandle DeviceHandle;        ///< Device handle
  UINT16 NumberOfFlushHintAddresses;    ///< Number of flush hint addresses
  UINT8 Reserved[6];                    ///< Reserved
  UINT64 FlushHintAddress[0];           ///< Flush hint addresses
} FlushHintTbl;

#define CAPABILITY_CACHE_FLUSH    BIT0
#define CAPABILITY_MEMORY_FLUSH   BIT1
#define CAPABILITY_MEMORY_MIRROR  BIT2

/** Platform Capabilities table */
typedef struct {
  SubTableHeader Header;              ///< Header
  UINT8 HighestValidCapability;       ///< Highest valid capability
  UINT8 Reserved[3];                  ///< Reserved
  UINT32 Capabilities;                ///< Capabilities
  UINT32 Reserved_1;                  ///< Reserved
} PlatformCapabilitiesTbl;

/** NFIT ACPI data */
typedef struct {
  NFitHeader *pFit;                                         ///< NFIT Header
  UINT32 SpaRangeTblesNum;                                  ///< Count of SPA Range tables
  SpaRangeTbl **ppSpaRangeTbles;                            ///< SPA Range tables
  UINT32 NvDimmRegionTblesNum;                              ///< Count of Region tables
  NvDimmRegionTbl **ppNvDimmRegionTbles;                    ///< Region tables
  UINT32 InterleaveTblesNum;                                ///< Count of interleave tables
  InterleaveStruct **ppInterleaveTbles;                     ///< Interleave tables
  UINT32 SmbiosTblesNum;                                    ///< Count of SMBIOS tables
  SmbiosTbl **ppSmbiosTbles;                                ///< SMBIOS tables
  UINT32 ControlRegionTblesNum;                             ///< Count of Control Region tables
  ControlRegionTbl **ppControlRegionTbles;                  ///< Control region tables
  UINT32 BWRegionTblesNum;                                  ///< Count of BW Region tables
  BWRegionTbl **ppBWRegionTbles;                            ///< BW Region tables
  UINT32 FlushHintTblesNum;                                 ///< Count of Flush Hint Tables
  FlushHintTbl **ppFlushHintTbles;                          ///< Flush Hint tables
  UINT32 PlatformCapabilitiesTblesNum;                      ///< Count of PCAT tables
  PlatformCapabilitiesTbl **ppPlatformCapabilitiesTbles;    ///< PCAT tables
} ParsedFitHeader;

typedef struct {
  UINT16 Type;     //!< Type of PCAT table
  UINT16 Length;   //!< Length of the table including the header and body
} PCAT_TABLE_HEADER;

typedef struct {
  /**
    HEADER
  **/
  TABLE_HEADER Header; //!< Signature for this table: 'PCAT'
  UINT8 Reserved[4];
  /**
    BODY
  **/
  /**
    A list of PCAT table structures
  **/
  VOID *pPcatTables[0];
} PLATFORM_CONFIG_ATTRIBUTES_TABLE;

typedef
union {
  UINT8 MemoryModes;
  struct {
    UINT8 OneLm           :1;
    UINT8 Memory          :1;
    UINT8 AppDirect       :1;
    UINT8 AppDirectCached :1;
    UINT8 Storage         :1;
    UINT8 SubNUMAClster   :1;
    UINT8 Reserved        :2;
  } MemoryModesFlags;
} SUPPORTED_MEMORY_MODE;

typedef struct {
    UINT8 CurrentVolatileMode : 2;
    UINT8 PersistentMode      : 2;
    UINT8 AllowedVolatileMode : 2;
    UINT8 Reserved            : 1;
    UINT8 SubNumaCluster      : 1;
  } _MEMORY_MODE_SPLIT;

typedef
union {
  UINT8 MemoryMode;
  _MEMORY_MODE_SPLIT MemoryModeSplit;
} CURRENT_MEMORY_MODE;

typedef struct {
  /**
    HEADER
  **/
  PCAT_TABLE_HEADER Header; //!< Type: 0
  /**
    BODY
  **/
  /**
    Bit0
      If set BIOS supports changing configuration through management software.
      If clear BIOS does not allow configure change through management software
    Bit1
      If set BIOS supports runtime interface to validate management configuration change request.
      Refer to BIOS runtime interface data structure.
      Note: this bit is valid only if Bit0 is set.
  **/
  UINT8 MgmtSwConfigInputSupport;
  /**
    Bit0: Set if 1LM Mode supported
    Bit1: Set if 2LM Mode supported
    Bit2: Set if PM-Direct Mode supported
    Bit3: Set if PM-Cached Mode supported
    Bit4: Set if Block Mode supported
  **/
  SUPPORTED_MEMORY_MODE MemoryModeCapabilities;
  /**
    Memory Mode selected in the BIOS setup
    1 - 1LM mode
    2 - 2LM + PM-Direct Mode
    3 - 2LM + PM-Cached Mode
    4 - Auto (2LM if DDR4+PMM with volatile mode present, 1LM otherwise)
    Note: no direct control is given to the management software to switch the mode
  **/
  CURRENT_MEMORY_MODE CurrentMemoryMode;
  /**
    Bit0: If set Persistent Memory region mirroring is supported
    If mirror is supported, management software can select interleave sets for mirror.
  **/
  UINT8 PersistentMemoryRasCapability;
  UINT8 Reserved[8];
} PLATFORM_CAPABILITY_INFO;

typedef
union {
  UINT32 AsUint32;
  struct {
    UINT32 ChannelInterleaveSize:8;
    UINT32 iMCInterleaveSize    :8;
    UINT32 NumberOfChannelWays  :NUMBER_OF_CHANNEL_WAYS_BITS_NUM;
    UINT32 Reserved             :6;
    UINT32 Recommended          :1;
  } InterleaveFormatSplit;
} INTERLEAVE_FORMAT;

typedef struct {
  /**
    HEADER
  **/
  PCAT_TABLE_HEADER Header; //!< Type: 1
  /**
    BODY
  **/
  /**
    Value defines memory mode
    0 - 1LM
    1 - 2LM
    3 - App Direct PM
    4 - App Direct Cached PM
  **/
  UINT8 MemoryMode;
  UINT8 Reserved[3];
  /**
    Interleave alignment size in 2^n bytes.
    n=26 for 64MB
    n=27 for 128MB
  **/
  UINT16 InterleaveAlignmentSize;
  /**
    Number of interleave formats supported by BIOS for the above memory mode. The variable body of this structure
    contains m number of interleave formats.
  **/
  UINT16 NumOfFormatsSupported;
  /**
    This field will have a list of 4byte values that provide information about BIOS supported interleave formats and
    the recommended interleave informations.

    Byte0 - Channel interleave size
    Bit0 - 64B
    Bit1 - 128B
    Bit2 - 256B
    Bit3 - Reserved
    Bit4 - Reserved
    Bit5 - Reserved
    Bit6 - 4KB
    Bit7 - 1GB

    Byte1 - iMC interleave size
    Bit0 - 64B
    Bit1 - 128B
    Bit2 - 256B
    Bit3 - Reserved
    Bit4 - Reserved
    Bit5 - Reserved
    Bit6 - 4KB
    Bit7 - 1GB

    Byte2-3 - Number of channel ways
    Bit0 - 1way
    Bit1 - 2way
    Bit2 - 3way
    Bit3 - 4way
    Bit4 - 6way
    Bit5 - 8way
    Bit6 - 12way
    Bit7 - 16way
    Bit8 - 24way
    Bit9-14 - Reserved

    Byte2-3 - Recommended Interleave format
    Bit15 - If clear, the interleave format is supported but not recommended.
            If set, the interleave format is recommended.
  **/
  INTERLEAVE_FORMAT InterleaveFormatList[0];
} MEMORY_INTERLEAVE_CAPABILITY_INFO;

typedef struct {
  /**
    HEADER
  **/
  PCAT_TABLE_HEADER Header; //!< Type: 2
  /**
    BODY
  **/
  /**
    Verify Trigger GAS Structure
  **/
  /**
    Address space type of command register.
    1 - System I/O
  **/
  UINT8 AddressSpaceId;
  UINT8 BitWidth;       //!< The size in bits of the command register
  UINT8 BitOffset;      //!< The bit offset command register at the given address
  /**
    Command register access size
    0 - Undefined
    1 - Byte Access
    2 - Word Access
    3 - Dword Access
    4 - Qword Access
  **/
  UINT8 AccessSize;
  UINT64 Address;   //!< Register in the given address space
  /**
    Verify Trigger Operation
  **/
  /**
    Type of register operation to submit the command
    0 - Read register
    1 - Write register
  **/
  UINT8 TriggerOperationType;
  UINT8 Reserved2[7];
  UINT64 TriggerValue;        //!< If operation type is write, this field provides the data to be written
  /**
    Mask value to be used to preserve the bits on the write. If the bits are not 1, read the value from
    the address space, mask the value part and then do the write
  **/
  UINT64 TriggerMask;
  /**
    Verify Status Operation
  **/
  /**
    ACPI GAS structure with Address Space ID.
    0 - System memory
  **/
  UINT8 GasStructure[12];
  /**
    Type of register operation to submit the command
    3 - Read Memory
  **/
  UINT8 StatusOperationType;
  UINT8 Reserved3[3];
  /**
    Read the value from given address and mask using this value. Result status:
    0 - None
    1 - Busy
    2 - Done
    Results are updated in the DIMMs' config output structures
  **/
  UINT64 StatusMask;
} RECONFIGURATION_INPUT_VALIDATION_INTERFACE_TABLE;

typedef struct {
  /**
    HEADER
  **/
  PCAT_TABLE_HEADER Header; //!< Type: 3
  /**
    BODY
  **/
  UINT8 Reserved[2];
  UINT16 VendorId;   //!< Vendor ID of generator of the GUID who maintains the format for the GUID data
  EFI_GUID Guid;
  VOID *pGuidData[0];   //!< GUID Data Size must be 8-byte aligned
} CONFIG_MANAGEMENT_ATTRIBUTES_EXTENSION_TABLE;

typedef struct {
  /**
    HEADER
   **/
  PCAT_TABLE_HEADER Header; //!< Type: 6
  /**
    BODY
   **/
  UINT16 SocketId;           //!< Zero indexed NUMA node identifier
  UINT8 Reserved[2];
  UINT64 MappedMemorySizeLimit;      //!< Total amount of physical memory in bytes allowed to be mapped into SPA based on the SKU of the CPU specified by Socket ID
  UINT64 TotalMemorySizeMappedToSpa; //!< Total amount of physical memory in bytes currently mapped into the SPA for the CPU specified by Socket ID
  UINT64 CachingMemorySize;          //!< Total amount of physical memory in bytes used for caching when the system is in 2LM mode
} SOCKET_SKU_INFO_TABLE;

/** PMTT table **/
typedef struct {
  TABLE_HEADER Header;
  UINT8 Reserved[4];
  VOID  *PMTTAggregatedDevices[0];
} PMTT_TABLE;

/* Header common to socket, iMC and Module*/
typedef struct {
  /*
  * Type of aggregated device
  * 0 - Socket
  * 1 - Memory controller
  * 2 - Module
  * 3 - 0xFF
  */
  UINT8 Type;

  UINT8 Reserved1;
  /*
  * Length in bytes for entire table.
  */
  UINT16 Length;
  UINT16 Flags;
  UINT16 Reserved2;
} PMTT_COMMON_HEADER;

/*
* Memory aggregator device structure
* Type 0 - socket
*/
typedef  struct {
  UINT16 SocketId;
  UINT16 Reserved3;

  // Memory controller comes here
} PMTT_SOCKET;

/*
* Memory aggregator device structure
* Type 1 - iMC
*/
typedef struct {
  UINT32 ReadLatency;
  UINT32 WriteLatency;
  UINT32 ReadBW;
  UINT32 WriteBW;
  UINT16 OptimalAccessUnit;
  UINT16 OptimalAccessAlignment;
  UINT16 Reserved3;
  UINT16 NoOfProximityDomains;
  UINT32 ProximityDomainArray; //!< Supposed to be an array but BIOS is filling in 0s for now
  // Module structure follows this
} PMTT_iMC;

/*
* Memory aggregator device structure
* Type 2 - Module
*/
typedef struct {
  UINT16 PhysicalComponentId;
  UINT16 Reserved3;
  UINT32 SizeOfDimm;
  UINT32 SmbiosHandle;
} PMTT_MODULE;

#pragma pack(pop)

typedef struct {
  PLATFORM_CONFIG_ATTRIBUTES_TABLE *pPlatformConfigAttr;
  PLATFORM_CAPABILITY_INFO **ppPlatformCapabilityInfo;
  UINT32 PlatformCapabilityInfoNum;
  MEMORY_INTERLEAVE_CAPABILITY_INFO **ppMemoryInterleaveCapabilityInfo;
  UINT32 MemoryInterleaveCapabilityInfoNum;
  RECONFIGURATION_INPUT_VALIDATION_INTERFACE_TABLE **ppRuntimeInterfaceValConfInput;
  UINT32 RuntimeInterfaceValConfInputNum;
  CONFIG_MANAGEMENT_ATTRIBUTES_EXTENSION_TABLE **ppConfigManagementAttributesInfo;
  UINT32 ConfigManagementAttributesInfoNum;
  SOCKET_SKU_INFO_TABLE **ppSocketSkuInfoTable;
  UINT32 SocketSkuInfoNum;
} ParsedPcatHeader;

/**
  Frees the memory associated in the parsed PCAT table.

  @param[in, out] pParsedPcat pointer to the PCAT header.
**/
VOID
FreeParsedPcat(
  IN OUT ParsedPcatHeader *pParsedPcat
  );

/**
  Frees the memory associated in the parsed NFit table.

  @param[in] pParsedNfit pointer to the NFit header.
**/
VOID
FreeParsedNfit(
  IN     ParsedFitHeader *pParsedNfit
  );

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#endif /** _NVMTABLES_H_ **/
