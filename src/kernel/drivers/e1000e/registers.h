#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

#include "bus/pci.h"
#include "drivers/e1000e.h"

#define IOADDR 0x0000
#define IODATA 0x0004

#define REGISTER_CTRL 0x0000
#define REGISTER_EEC 0x0010
#define REGISTER_EERD 0x0014
#define REGISTER_IMS 0x00d0
#define REGISTER_RAL 0x5400
#define REGISTER_RAH 0x5404

/*
	General 00000h-00004h CTRL Device Control R/W 282
	General 00008h STATUS Device Status RO 286
	General 00010h EEC EEPROM/Flash Control R/W 288
	General 00014h EERD EEPROM Read R/W 291
	General 00018h CTRL_EXT Extended Device Control R/W 292
	General 0001Ch FLA Flash Access R/W 296
	General 00020h MDIC MDI Control R/W 299
	General 00024h SERDESCTL Serdes_ana R/W 360
	General 00028h FCAL Flow Control Address Low R/W 360
	General 0002Ch FCAH Flow Control Address High R/W 361
	General 00030h FCT Flow Control Type R/W 361
	General 00034h KUMCTRLSTA GLCI Control and Status Register (the 631xESB/632xESB) R/W 361
	General 00038h VET VLAN EtherType R/W 363
	General 00170h FCTTV Flow Control Transmit Timer Value R/W 364
	General 00178h TXCW Transmit Configuration Word R/W 365
	General 00180h RXCW Receive Configuration Word RO 367
	General 00E00h LEDCTL LED Control R/W 370
	General 01000h PBA Packet Buffer Allocation R/W 374
	General 01010h EEMNGCTL MNG EEPROM Control (82571EB) R/W 374
	General 05B5Ch SW_FW_SYNC Software/Firmware Synchronization (631xESB/632xESB) R/W 374
	Interrupt 000C0h ICR Interrupt Cause Read R/CLR 377
	Interrupt 000C4h ITR Interrupt Throttling Rate R/W 380
	Interrupt 000C8h ICS Interrupt Cause Set WO 381
	Interrupt 000D0h IMS Interrupt Mask Set/Read R/W 383
	Interrupt 000D8h IMC Interrupt Mask Clear WO 384
	Interrupt 000E0h IAM Interrupt Acknowledge Auto Mask R/W 385
	Receive 00100h RCTL Receive Control R/W 385
	Receive 02008h ERT Early Receive Threshold (82573E/82573V/82573L only) R/W 390
	Receive 02160h FCRTL Flow Control Receive Threshold Low R/W 391
	Receive 02168h FCRTH Flow Control Receive Threshold High R/W 392
	Receive 02170h PSRCTL Packet Split Receive Control R/W 393
	Receive 02800h RDBAL0 Receive Descriptor Base Low Queue 0 R/W 394
	Receive 02804h RDBAH0 Receive Descriptor Base High Queue 0 R/W 394
	Receive 02808h RDLEN0 Receive Descriptor Length Queue 0 R/W 395
	Receive 02810h RDH0 Receive Descriptor Head Queue 0 R/W 395
	Receive 02818h RDT0 Receive Descriptor Tail Queue 0 R/W 396
	Receive 02820h RDTR Receive Interrupt Packet Delay Timer R/W 396
	Receive 02828h RXDCTL Receive Descriptor Control R/W 397
	Receive 0282Ch RADV Receive Interrupt Absolute Delay Timer R/W 398
	Receive 02900h RDBAL1 Receive Descriptor Base Low Queue 1 R/W 394
	Receive 02904h RDBAH1 Receive Descriptor Base High Queue 1 R/W 394
	Receive 02908h RDLEN1 Receive Descriptor Length Queue 1 R/W 395
	Receive 02910h RDH1 Receive Descriptor Head Queue 1 R/W 395
	Receive 02918h RDT1 Receive Descriptor Tail Queue 1 R/W 396
	Receive 02928h RXDCTL1 Receive Descriptor Control 1 R/W 397
	Receive 02C00h RSRPD Receive Small Packet Detect R/W 403
	Receive 02C08 RAID Receive ACK Interrupt Delay R/W 403
	Receive 02C10h CPUVEC CPU Vector R/W 404
	Receive 05000h RXCSUM Receive Checksum Control R/W 404
	Receive 05008h RFCTL Receive Filter Control R/W 405
	Receive 05200h-053FCh MTA[127:0] Multicast Table Array (n) R/W 425
	Receive 05400h RAL Receive Address Low R/W 427
	Receive 05404h RAH Receive Address High) R/W 427
	Receive 05600h-057FCh VFTA[127:0] VLAN Filter Table Array (n) R/W 428
	Receive 05818h MRQC Multiple Receive Queues Command R/W 429
	Receive 05864h RSSIM RSS Interrupt Mask R/W 430
	Receive 05868h RSSIR RSS Interrupt Request R/W 430
	Receive 05C00-05C3Fh RETA Redirection Table R/W 431
	Receive 05C80-05CA7h RSSRK RSS Random Key R/W 431
	Transmit 00400h TCTL Transmit Control R/W 406
	Transmit 00410h TIPG Transmit IPG R/W 409
	Transmit 00458h AIT Adaptive IFS Throttle (Not applicable to the631xESB/632xESB) R/W 410
	Transmit 03800h TDBAL Transmit Descriptor Base Low R/W 411
	Transmit 03804h TDBAH Transmit Descriptor Base High R/W 411
	Transmit 03808h TDLEN Transmit Descriptor Length R/W 412
	Transmit 03810h TDH Transmit Descriptor Head R/W 412
	Transmit 03818h TDT Transmit Descriptor Tail R/W 413
	Transmit 03820h TIDV Transmit Interrupt Delay Value R/W 413
	Transmit 03828h TXDCTL Transmit Descriptor Control R/W 414
	Transmit 0382Ch TADV Transmit Absolute Interrupt Delay Value R/W 416
	Transmit 03840h TARC0 Transmit Arbitration Counter Queue 0 R/W 424
	Transmit 03900h TDBAL1 Transmit Descriptor Base Low Queue 1 R/W 411
	Transmit 03904h TDBAH1 Transmit Descriptor Base High Queue 1 R/W 411
	Transmit 03908h TDLEN1 Transmit Descriptor Length Queue 1 R/W 412
	Transmit 03910h TDH1 Transmit Descriptor Head Queue 1 R/W 412
	Transmit 03918h TDT1 Transmit Descriptor Tail Queue 1 R/W 413
	Transmit 03928h TXDCTL1 Transmit Descriptor Control 1 R/W 414
	Transmit 03940h TARC1 Transmit Arbitration Counter Queue 1 R/W 416
	Statistics 04000h CRCERRS CRC Error Count R 461
	Statistics 04004h ALGNERRC Alignment Error Count R 462
	Statistics 04008h SYMERRS Symbol Error Count R 462
	Statistics 0400Ch RXERRC RX Error Count R 462
	Statistics 04010h MPC Missed Packets Count R 463
	Statistics 04014h SCC Single Collision Count R 463
	Statistics 04018h ECOL Excessive Collisions Count R 464
	Statistics 0401Ch MCC Multiple Collision Count R 464
	Statistics 04020h LATECOL Late Collisions Count R 464
	Statistics 04028h COLC Collision Count R 465
	Statistics 04030h DC Defer Count R 465
	Statistics 04034h TNC-S Transmit - No CRS R 466
	Statistics 04038h SEC Sequence Error Count R 466
	Statistics 04040h RLEC Receive Length Error Count R 467
	Statistics 04048h XONRXC XON Received Count R 468
	Statistics 0404Ch XONTXC XON Transmitted Count R 468
	Statistics 04050h XOFFRXC XOFF Received Count R 469
	Statistics 04054h XOFFTXC XOFF Transmitted Count R 469
	Statistics 04058h FCRUC FC Received Unsupported Count R 470
	Statistics 0405Ch PRC64 Packets Received (64 Bytes) Count R 470
	Statistics 04060h PRC127 Packets Received (65-127 Bytes) Count R 470
	Statistics 04064h PRC255 Packets Received (128-255 Bytes) Count R 471
	Statistics 04068h PRC511 Packets Received (256-511 Bytes) Count R 471
	Statistics 0406Ch PRC1023 Packets Received (512-1023 Bytes) Count R 472
	Statistics 04070h PRC1522 Packets Received (1024-Max Bytes) R 472
	Statistics 04074h GPRC Good Packets Received Count R 472
	Statistics 04078h BPRC Broadcast Packets Received Count R 473
	Statistics 0407Ch MPRC Multicast Packets Received Count R 473
	Statistics 04080h GPTC Good Packets Transmitted Count R 474
	Statistics 04088h GORCL Good Octets Received Count (Low) R 474
	Statistics 0408Ch GORCH Good Octets Received Count (Hi) R 474
	Statistics 04090h GOTCL Good Octets Transmitted Count (Low) R 475
	Statistics 04094h GOTCH Good Octets Transmitted Count (Hi) R 475
	Statistics 040A0h RNBC Receive No Buffers Count R 475
	Statistics 040A4h RUC Receive Undersize Count R 476
	Statistics 040A8h RFC Receive Fragment Count R 476
	Statistics 040ACh ROC Receive Oversize Count R 476
	Statistics 040B0h RJC Receive Jabber Count R 477
	Statistics 040B4h MPRC Management Packets Received Count R 477
	Statistics 040B8h MPDC Management Packets Dropped Count R 477
	Statistics 040BCh MPTC Management Pkts Transmitted Count R 478
	Statistics 040C0h TORL Total Octets Received (Lo) R 478
	Statistics 040C4h TORH Total Octets Received (Hi) R 478
	Statistics 040C8h TOTL Total Octets Transmitted (Lo) R 478
	Statistics 040CCh TOTH Total Octets Transmitted (Hi) R 478
	Statistics 040D0h TPR Total Packets Received R 479
	Statistics 040D4h TPT Total Packets Transmitted R 479
	Statistics 040D8h PTC64 Packets Transmitted (64 Bytes) Count R 480
	Statistics 040DCh PTC127 Packets Transmitted (65-127 Bytes) Count R 480
	Statistics 040E0h PTC255 Packets Transmitted (128-255 Bytes) Count R 480
	Statistics 040E4h PTC511 Packets Transmitted (256-511 Bytes) Count R 481
	Statistics 040E8h PTC1023 Packets Transmitted (512-1023 Bytes) Count R 481
	Statistics 040ECh PTC1522 Packets Transmitted (1024-1522) Count R 482
	Statistics 040F0h MPTC Multicast Packets Transmitted Count R 482
	Statistics 040F4h BPTC Broadcast Packets Transmitted Count R 482
	Statistics 040F8h TSCTC TCP Segmentation Context Transmitted Count R 483
	Statistics 040FCh TSCTFC TCP Segmentation Context Tx Fail Count R 483
	Statistics 04100h IAC Interrupt Assertion Count (not applicable to the 631xESB/632xESB) R 484
	Statistics 04104h ICRXPTC Interrupt Cause Rx Packet Timer Expire Count R 484
	Statistics 04108h ICRXATC Interrupt Cause Rx Absolute Timer Expire Count R 484
	Statistics 0410Ch ICTXPTC Interrupt Cause Tx Packet Timer Expire Count R 484
	Statistics 04110h ICTXATC Interrupt Cause Tx Absolute Timer Expire Count R 485
	Statistics 04118h ICTXQEC Interrupt Cause Tx Queue Empty Count R 485
	Statistics 0411Ch ICTXDLTC Interrupt Cause Transmit Descriptor Low Threshold Count R 486
	Statistics 04120h ICRXDMTC Interrupt Cause Rx Descriptor Minimum Threshold Count R 486
	Statistics 04124h ICRXOC Interrupt Cause Rx Overrun Count R 486
	Wakeup 05800h WUC Wakeup Control R/W 431
	Wakeup 05808h WUFC Wakeup Filter Control R/W 432
	Wakeup 05810h WUS Wakeup Status R 433
	Wakeup 05838h IPAV IP Address Valid R/W 434
	Wakeup 05840h-05858h IP4AT IPv4 Address Table R/W 434
	Wakeup 05880h-0588Fh IP6AT IPv6 Address Table R/W 435
	Wakeup 05900h WUPL Wakeup Packet Length R/W 436
	Wakeup 05A00h-05A7Ch WUPM Wakeup Packet Memory R/W 436
	Wakeup 05F00h-05F18h FFLT Flexible Filter Length Table R/W 437
*/

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

typedef struct
{
	uint32_t RAL;
} ReceiveAddressLow, RAL;

typedef struct
{
	uint32_t RAH : 16;
	uint32_t ASEL : 2;
	uint32_t reserved : 13;
	uint32_t AV : 1;
} ReceiveAddressHigh, RAH;

#define X(r, R) \
	R e1000e_read_ ## r (e1000e* device); \
	void e1000e_write_ ## r (e1000e* device, R r); \

X(ctrl, CTRL)
X(eec, EEC)
X(eerd, EERD)
X(ims, IMS)
X(ral, RAL)
X(rah, RAH)

#undef X

#endif
