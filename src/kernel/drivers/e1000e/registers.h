#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

#include "bus/pci.h"

#define IOADDR 0x0000
#define IODATA 0x0004
#define IOEEC 0x0010
#define IOEERD 0x0014
#define IOIMS 0x00d0

typedef struct 
{
	uint32_t FD : 1;
	uint32_t reserverd0 : 1;
	uint32_t GIOMasterDisable : 1;
	uint32_t LRST : 1;
	uint32_t reserverd1 : 2;
	uint32_t SLU : 1;
	uint32_t reserverd2 : 1;
	uint32_t SPEED : 2;
	uint32_t reserverd3 : 1;
	uint32_t FRCSPD : 1;
	uint32_t FRCDPLX : 1;
	uint32_t reserverd4 : 5;
	union 
	{
		uint32_t SDP0_DATA : 1;
		uint32_t DUDStatus : 1;
	};
	uint32_t SDP1_DATA : 1;
	uint32_t ADVD3WUC : 1;
	uint32_t reserverd5 : 1;
	uint32_t SDP0_IODIR : 1;
	uint32_t SDP1_IODIR : 1;
	uint32_t reserverd6 : 2;
	uint32_t RST : 1;
	uint32_t RFCE : 1;
	uint32_t TFCE : 1;
	uint32_t reserverd7 : 1;
	uint32_t VME : 1;
	uint32_t PHY_RST : 1;
} DeviceControlRegister, CTRL;

typedef struct 
{
	uint32_t FD : 1;
	uint32_t LU : 1;
	uint32_t LANID : 2;
	uint32_t TXOFF : 1;
	uint32_t TBIMODE : 1;
	uint32_t SPEED : 2;
	uint32_t ASDV : 2;
	uint32_t PHYRA : 1;
	uint32_t reserverd0 : 8;
	uint32_t GIOMasterEnableStatus : 1;
	uint32_t reserverd1 : 12;
} DeviceStatusRegister, STATUS;

typedef struct 
{
	uint32_t EE_SK : 1;
	uint32_t EE_CS : 1;
	uint32_t EE_DI : 1;
	uint32_t EE_DOO : 1;
	uint32_t FWE : 2;
	uint32_t EE_REQ : 1;
	uint32_t EE_GNT : 1;
	uint32_t EE_PRES : 1;
	uint32_t AUTO_RD : 1;
	uint32_t EE_ADDR_SIZE : 1;
	uint32_t EE_SIZE : 4;
	uint32_t NV_SIZE : 2;
	uint32_t NV_ADDR_SIZE : 3;
	uint32_t AUPDEN : 1;
	uint32_t reserverd1 : 2;
	uint32_t NVMTYPE : 2;
	uint32_t reserverd2 : 7;
} EEPROMControlRegister, EEC;

typedef struct 
{
	uint32_t START : 1;
	uint32_t DONE : 1;
	uint32_t ADDR : 14;
	uint32_t DATA : 16;
} EEPROMReadRegister, EERD;

typedef struct 
{
	uint32_t START : 1;
	uint32_t DONE : 1;
	uint32_t ADDR : 14;
	uint32_t DATA : 16;
} EEPROMWriteRegister, EEWR;

typedef struct 
{
	uint32_t reserverd0 : 1;
	uint32_t EN : 1;
	uint32_t reserverd1 : 1;
	uint32_t PSP : 1;
	uint32_t CT : 8;
	uint32_t COLD : 10;
	uint32_t SWXOFF : 1;
	uint32_t reserverd2 : 1;
	uint32_t RTLC : 1;
	uint32_t UNORTX : 1;
	uint32_t TXCSCMT : 2;
	uint32_t MULR : 1;
	uint32_t reserverd3 : 3;
} TransmitControlRegister, TCTL;

typedef struct 
{
	uint32_t reserverd0 : 1;
	uint32_t EN : 1;
	uint32_t SBP : 1;
	uint32_t UPE : 1;
	uint32_t MPE : 1;
	uint32_t LPE : 1;
	uint32_t LBM : 2;
	uint32_t RDMTS : 2;
	uint32_t DTYP : 2;
	uint32_t MO : 2;
	uint32_t reserverd1 : 1;
	uint32_t BAM : 1;
	uint32_t BSIZE : 2;
	uint32_t VFE : 1;
	uint32_t CFIEN : 1;
	uint32_t CFI : 1;
	uint32_t reserverd2 : 1;
	uint32_t DPF : 1;
	uint32_t PMCF : 1;
	uint32_t reserverd3 : 1;
	uint32_t BSEX : 1;
	uint32_t SECRC : 1;
	uint32_t FLXBUF : 4;
	uint32_t reserverd4 : 1;
} ReceiveControlRegister, RCTL;

typedef struct 
{
	uint32_t TXDW : 1;
	uint32_t TXQE : 1;
	uint32_t LSC : 1;
	uint32_t RXSEQ : 1;
	uint32_t RXDMT0 : 1;
	uint32_t reserverd0 : 1;
	uint32_t RXO : 1;
	uint32_t RXT0 : 1;
	uint32_t reserverd1 : 1;
	uint32_t MDAC : 1;
	uint32_t RXCFG : 1;
	uint32_t reserverd2 : 2;
	uint32_t GPI : 2;
	uint32_t TXD_LOW : 1;
	uint32_t SRPD : 1;
	uint32_t ACK : 1;
	uint32_t reserverd3 : 14;
} InterruptMaskSetRegister, IMS;

#endif
