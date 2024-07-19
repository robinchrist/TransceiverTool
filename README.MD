# TransceiverTool
## What is this project? (TL;DR version)
This tool is built for the SFF-8472 (SFP/+/28) and SFF-8636 (QSFP/+/28) standards. 
Those standards are used by pluggable transceivers and cables to provide different types information (starting from manufacturer, over required power, wavelength, maximum reach, etc...) 
This tool can decode binary to human-readable(!) JSON, encode JSON to binary, validate (e.g. whether reserved bits are set) and pretty-print.
It offers a programmatic interface to manipulation through the `SFF8472_LowerA0h` and `SFF8636_Upper00h` class (and several helper classes / structs) to make working with those standards easier.

This tool allows creation of every possible programming and is roundtrip-stable. All reserved bytes or things like invalid checksums can be done with it.

Full JSON schemas are provided for the JSON output / input.

Overall, the goal is democratise transceiver programming. Right now your best option is probably using this tool together with REVELPROG programmer and the matching adapter board.

## Why this project?
To my knowledege, there is no other open source project that allows easy creation of new programmings / binaries of SFF-8472 and SFF-8636. Because I often reprogram transceivers and don't want to use proprietary (and very limited) tools like the fs.com FS BOX or the flexoptix FLEXBOX, I decided to write my own tool.

## License
This project is licensed under the MIT license

## Warning
This project was created entirely in my free time. The inconsistency and overall quality of the SFF-8472 and SFF-8636 made me consider cancelling this project so often that I stopped counting at some point. Large parts of this code were written under heavy influence of sleep deprivation and almost deadly amounts of caffeine and/or energy drinks.

I tried to make this code rock-solid and so far haven't encountered any fatal bugs or errors. Fuzzing is on the todo list.

A friend once said

> Consider everything in here unstable and evil. Assume everything will eat your cats and dogs.


## Example Output

<details>
  <summary>SFF-8472 (SFP+) Pretty Print (Brocade 57-1000027-01) </summary>

  ```
TransceiverTool decode --binary-type 8472-128b --pretty-print --from-file Brocade-57-1000027-01.bin
Type                                                                                 : SFF-8472

Identifier [0]                                                                       : SFP or SFP+ (0x03)

Extended Identifier [1]                                                              : GBIC/SFP function is defined by 2-wire interface ID only (0x04)

Connector Type [2]                                                                   : LC (Lucent Connector) (0x07)

Specification Compliance, 10G Ethernet Compliance Codes [3, 7]                       : Not 10GBASE-ER compliant
Specification Compliance, 10G Ethernet Compliance Codes [3, 6]                       : Not 10GBASE-LRM compliant
Specification Compliance, 10G Ethernet Compliance Codes [3, 5]                       : Not 10GBASE-LR compliant
Specification Compliance, 10G Ethernet Compliance Codes [3, 4]                       : Not 10GBASE-SR compliant

Specification Compliance, Infiniband Compliance Codes [3, 3]                         : Not 1X SX compliant
Specification Compliance, Infiniband Compliance Codes [3, 2]                         : Not 1X LX compliant
Specification Compliance, Infiniband Compliance Codes [3, 1]                         : Not 1X Copper Active compliant
Specification Compliance, Infiniband Compliance Codes [3, 0]                         : Not 1X Copper Passive compliant

Specification Compliance, ESCON Compliance Codes [4, 7]                              : Not ESCON MMF, 1310nm LED compliant
Specification Compliance, ESCON Compliance Codes [4, 6]                              : Not ESCON SMF, 1310nm Laser compliant

Specification Compliance, SONET Compliance Codes [4, 5]                              : Not OC-192, short reach compliant
Specification Compliance, SONET Compliance Codes [4, 4-3]                            : SONET SR compliant or not specified (0b00)
Specification Compliance, SONET Compliance Codes [4, 2]                              : Not OC-48, long reach compliant
Specification Compliance, SONET Compliance Codes [4, 1]                              : Not OC-48, intermediate reach compliant
Specification Compliance, SONET Compliance Codes [4, 0]                              : Not OC-48, short reach compliant
Specification Compliance, SONET Compliance Codes [5, 7]                              : N/A
Specification Compliance, SONET Compliance Codes [5, 6]                              : Not OC-12, single mode, long reach compliant
Specification Compliance, SONET Compliance Codes [5, 5]                              : Not OC-12, single mode, intermediate reach compliant
Specification Compliance, SONET Compliance Codes [5, 4]                              : Not OC-12, short reach compliant
Specification Compliance, SONET Compliance Codes [5, 3]                              : N/A
Specification Compliance, SONET Compliance Codes [5, 2]                              : Not OC-3, single mode, long reach compliant
Specification Compliance, SONET Compliance Codes [5, 1]                              : Not OC-3, single mode, intermediate reach compliant
Specification Compliance, SONET Compliance Codes [5, 0]                              : Not OC-3, short reach compliant

Specification Compliance, Ethernet Compliance Codes [6, 7]                           : Not BASE-PX compliant
Specification Compliance, Ethernet Compliance Codes [6, 6]                           : Not BASE-BX10 compliant
Specification Compliance, Ethernet Compliance Codes [6, 5]                           : Not 100BASE-FX compliant
Specification Compliance, Ethernet Compliance Codes [6, 4]                           : Not 100BASE-LX/LX10 compliant
Specification Compliance, Ethernet Compliance Codes [6, 3]                           : Not 1000BASE-T compliant
Specification Compliance, Ethernet Compliance Codes [6, 2]                           : Not 1000BASE-CX compliant
Specification Compliance, Ethernet Compliance Codes [6, 1]                           : Not 1000BASE-LX compliant
Specification Compliance, Ethernet Compliance Codes [6, 0]                           : Not 1000BASE-SX compliant

Specification Compliance, Fibre Channel Link Length [7, 7]                           : Not very long distance (V) compliant
Specification Compliance, Fibre Channel Link Length [7, 6]                           : Not short distance (S) compliant
Specification Compliance, Fibre Channel Link Length [7, 5]                           : Not intermediate distance (I) compliant
Specification Compliance, Fibre Channel Link Length [7, 4]                           : long distance (L) compliant
Specification Compliance, Fibre Channel Link Length [7, 3]                           : Not medium distance (M) compliant

Specification Compliance, Fibre Channel Technology [7, 2]                            : Not Shortwave laser, linear Rx (SA) compliant
Specification Compliance, Fibre Channel Technology [7, 1]                            : Longwave laser (LC) compliant
Specification Compliance, Fibre Channel Technology [7, 0]                            : Not Electrical inter-enclosure (EL) compliant
Specification Compliance, Fibre Channel Technology [8, 7]                            : Not Electrical intra-enclosure (EL) compliant
Specification Compliance, Fibre Channel Technology [8, 6]                            : Not Shortwave laser w/o OFC (SN) compliant
Specification Compliance, Fibre Channel Technology [8, 5]                            : Not Shortwave laser with OFC (SL) 4 compliant
Specification Compliance, Fibre Channel Technology [8, 4]                            : Not Longwave laser (LL) compliant

Specification Compliance, SFP+ Cable Technology [8, 3]                               : Not Active Cable compliant
Specification Compliance, SFP+ Cable Technology [8, 2]                               : Not Passive Cable compliant
Specification Compliance, SFP+ Cable Technology [8, 1]                               : N/A
Specification Compliance, SFP+ Cable Technology [8, 0]                               : N/A

Specification Compliance, Fibre Channel Transmission Media [9, 7]                    : Not Twin Axial Pair (TW) compliant
Specification Compliance, Fibre Channel Transmission Media [9, 6]                    : Not Twisted Pair (TP) compliant
Specification Compliance, Fibre Channel Transmission Media [9, 5]                    : Not Miniature Coax (MI) compliant
Specification Compliance, Fibre Channel Transmission Media [9, 4]                    : Not Video Coax (TV) compliant
Specification Compliance, Fibre Channel Transmission Media [9, 3]                    : Not Multimode, 62.5um (M6) compliant
Specification Compliance, Fibre Channel Transmission Media [9, 2]                    : Not Multimode, 50um (M5, M5E) compliant
Specification Compliance, Fibre Channel Transmission Media [9, 1]                    : N/A
Specification Compliance, Fibre Channel Transmission Media [9, 0]                    : Single Mode (SM) compliant

Specification Compliance, Fibre Channel Speed [10, 7]                                : Not 1200 MBytes/sec compliant
Specification Compliance, Fibre Channel Speed [10, 6]                                : 800 MBytes/sec compliant
Specification Compliance, Fibre Channel Speed [10, 5]                                : Not 1600 MBytes/sec compliant
Specification Compliance, Fibre Channel Speed [10, 4]                                : 400 MBytes/sec compliant
Specification Compliance, Fibre Channel Speed [10, 3]                                : Not 3200 MBytes/sec compliant
Specification Compliance, Fibre Channel Speed [10, 2]                                : 200 MBytes/sec compliant
Specification Compliance, Fibre Channel Speed [10, 1]                                : No "Fibre Channel Speed 2"
Specification Compliance, Fibre Channel Speed [10, 0]                                : Not 100 MBytes/sec compliant

Encoding [11]                                                                        : 8B/10B (0x01)

Signaling rate, nominal [12]                                                         : 8500 MBd

Rate Identifier [13]                                                                 : Unspecified (00h) (0x00)

Length (SMF) [14]                                                                    : 10 km

Length (SMF) [15]                                                                    : 10000 m

Length (OM2 50 um) [16]                                                              : not supported / must be determined from transceiver technology

Length (OM1 62.5 um) [17]                                                            : not supported / must be determined from transceiver technology

Length (OM4 50 um) [18]                                                              : not supported / must be determined from transceiver technology

Length (OM3 50 um) [19]                                                              : not supported / must be determined from transceiver technology

Vendor Name (wrapping quotes added by TransceiverTool) [20-35]                       : "BROCADE"

Extended Specification Compliance [36]                                               : Unspecified (0x00)

Vendor OUI [37-39]                                                                   : 00:05:1e (Vendor name is "Brocade Communications Systems LLC")

Vendor PN (wrapping quotes added by TransceiverTool) [40-55]                         : "57-1000027-01"

Vendor Rev (wrapping quotes added by TransceiverTool) [56-59]                        : "A"

Wavelength [60-61]                                                                   : 1310 nm

Specification Compliance, Fibre Channel Speed 2 [62, 7]                              : N/A
Specification Compliance, Fibre Channel Speed 2 [62, 6]                              : N/A
Specification Compliance, Fibre Channel Speed 2 [62, 5]                              : N/A
Specification Compliance, Fibre Channel Speed 2 [62, 4]                              : N/A
Specification Compliance, Fibre Channel Speed 2 [62, 3]                              : N/A
Specification Compliance, Fibre Channel Speed 2 [62, 2]                              : N/A
Specification Compliance, Fibre Channel Speed 2 [62, 1]                              : N/A
Specification Compliance, Fibre Channel Speed 2 [62, 0]                              : Not 64 GFC compliant

CC_BASE checksum [63]                                                                : 0x11 (correct)

Option Values [64, 7]                                                                : N/A
Option Values [64, 6]                                                                : No High Power Level (Level 4) requirement
Option Values [64, 5]                                                                : No High Power Level (Level 3 or Level 4) requirement
Option Values [64, 4]                                                                : Paging NOT implemented
Option Values [64, 3]                                                                : No internal retimer or data recovery (CDR) circuit present
Option Values [64, 2]                                                                : Conventional Uncooled / Unspecified Laser
Option Values [64, 1]                                                                : Power Level 1 (or unspecified) requirement
Option Values [64, 0]                                                                : Conventional limiting, PAM4 or unspecified transceiver receiver output

Option Values [65, 7]                                                                : Receiver decision threshold NOT implemented
Option Values [65, 6]                                                                : Transmitter NOT tunable
Option Values [65, 5]                                                                : RATE_SELECT functionality NOT implemented
Option Values [65, 4]                                                                : TX_DISABLE implemented
Option Values [65, 3]                                                                : TX_FAULT signal implemented
Option Values [65, 2]                                                                : Loss of Signal with inverted signal NOT implemented
Option Values [65, 1]                                                                : Loss of Signal implemented
Option Values [65, 0]                                                                : N/A

Signaling Rate, max [66]                                                             : 0% Margin (0% above 8500 MBd -> Max 8500 MBd)

Signaling Rate, min [67]                                                             : 0% Margin (0% below 8500 MBd -> Min 8500 MBd)

Vendor S/N (wrapping quotes added by TransceiverTool) [68-83]                        : "UDA112361014441"

Date Code, low order digits of year [84-85]                                          : 12
Date Code, digits of month [86-87]                                                   : 09
Date Code, digits of day [88-89]                                                     : 15
Date Code, lot code (wrapping quotes added by TransceiverTool) [90-91]               : "  "

Option Values [92, 7]                                                                : N/A
Option Values [92, 6]                                                                : Digital diagnostic monitoring implemented
Option Values [92, 5]                                                                : Internally Calibrated
Option Values [92, 4]                                                                : NOT Externally Calibrated
Option Values [92, 3]                                                                : Received power measurements type = Average
Option Values [92, 2]                                                                : Address change NOT required
Option Values [92, 1]                                                                : N/A
Option Values [92, 0]                                                                : N/A

Enhanced Options [93, 7]                                                             : Optional Alarm/warning flags implemented
Enhanced Options [93, 6]                                                             : Optional soft TX_DISABLE control and monitoring implemented
Enhanced Options [93, 5]                                                             : Optional soft TX_FAULT monitoring implemented
Enhanced Options [93, 4]                                                             : Optional soft RX_LOS monitoring implemented
Enhanced Options [93, 3]                                                             : Optional soft RATE_SELECT control and monitoring NOT implemented
Enhanced Options [93, 2]                                                             : Optional Application Select control NOT implemented per SFF-8079
Enhanced Options [93, 1]                                                             : Optional soft Rate Select control NOT implemented per SFF-8431
Enhanced Options [93, 0]                                                             : N/A

SFF-8472 Compliance [94]                                                             : Includes functionality described in Rev 10.2 of SFF-8472 (0x03)

CC_EXT checksum [95]                                                                 : 0x3d (correct)

Vendor Specific (wrapping quotes added by TransceiverTool) [96-127]                  : "AFCT-57D5ATPZ-B1            1   "
  ```

</details>

<details>
  <summary>SFF-8472 (SFP+) JSON (Brocade 57-1000027-01) </summary>

  ```
./build/TransceiverTool decode --binary-type 8472-128b --json-to-stdout --from-file /data/dev/TransceiverTool_Go/Brocade-57-1000027-01.bin
{
    "GeneratedBy": "TransceiverTool v0.0.1",
    "Type": "SFF-8472 Rev 12.4 Lower Page A0h",
    "Identifier": "SFP or SFP+",
    "Extended Identifier": "GBIC/SFP function is defined by 2-wire interface ID only",
    "Connector Type": "LC (Lucent Connector)",
    "10G Ethernet Compliance Codes": {
        "10GBASE-ER compliant (Bit 7)": false,
        "10GBASE-LRM compliant (Bit 6)": false,
        "10GBASE-LR compliant (Bit 5)": false,
        "10GBASE-SR compliant (Bit 4)": false
    },
    "Infiniband Compliance Codes": {
        "1X SX compliant (Bit 3)": false,
        "1X LX compliant (Bit 2)": false,
        "1X Copper Active compliant (Bit 1)": false,
        "1X Copper Passive compliant (Bit 0)": false
    },
    "ESCON Compliance Codes": {
        "ESCON MMF, 1310nm LED compliant (Bit 7)": false,
        "ESCON SMF, 1310nm Laser compliant (Bit 6)": false
    },
    "SONET Compliance Codes": {
        "OC-192, short reach compliant (Byte 4, Bit 5)": false,
        "SONET reach specifier (Byte 4, Bit 4-3)": "SONET SR compliant or not specified",
        "OC-48, long reach compliant (Byte 4, Bit 2)": false,
        "OC-48, intermediate reach compliant (Byte 4, Bit 1)": false,
        "OC-48, short reach compliant (Byte 4, Bit 0)": false,
        "Reserved (Byte 5, Bit 7)": false,
        "OC-12, single mode, long reach compliant (Byte 5, Bit 6)": false,
        "OC-12, single mode, intermediate reach compliant (Byte 5, Bit 5)": false,
        "OC-12, short reach compliant (Byte 5, Bit 4)": false,
        "Reserved (Byte 5, Bit 3)": false,
        "OC-3, single mode, long reach compliant (Byte 5, Bit 2)": false,
        "OC-3, single mode, intermediate reach compliant (Byte 5, Bit 1)": false,
        "OC-3, short reach compliant (Byte 5, Bit 0)": false
    },
    "Ethernet Compliance Codes": {
        "BASE-PX compliant (Bit 7)": false,
        "BASE-BX10 compliant (Bit 6)": false,
        "100BASE-FX compliant (Bit 5)": false,
        "100BASE-LX/LX10 compliant (Bit 4)": false,
        "1000BASE-T compliant (Bit 3)": false,
        "1000BASE-CX compliant (Bit 2)": false,
        "1000BASE-LX compliant (Bit 1)": false,
        "1000BASE-SX compliant (Bit 0)": false
    },
    "Fibre Channel Link Length": {
        "very long distance (V) compliant (Bit 7)": false,
        "short distance (S) compliant (Bit 6)": false,
        "intermediate distance (I) compliant (Bit 5)": false,
        "long distance (L) compliant (Bit 4)": true,
        "medium distance (M) compliant (Bit 3)": false
    },
    "Fibre Channel Technology": {
        "Shortwave laser, linear Rx (SA) compliant (Byte 7, Bit 2)": false,
        "Longwave laser (LC) compliant (Byte 7, Bit 1)": true,
        "Electrical inter-enclosure (EL) compliant (Byte 7, Bit 0)": false,
        "Electrical intra-enclosure (EL) compliant (Byte 8, Bit 7)": false,
        "Shortwave laser w/o OFC (SN) compliant (Byte 8, Bit 6)": false,
        "Shortwave laser with OFC (SL) compliant (Byte 8, Bit 5)": false,
        "Longwave laser (LL) compliant (Byte 8, Bit 4)": false
    },
    "SFP+ Cable Technology": {
        "Active Cable compliant (Bit 3)": false,
        "Passive Cable compliant (Bit 2)": false,
        "Reserved (Bit 1)": false,
        "Reserved (Bit 0)": false
    },
    "Fibre Channel Transmission Media": {
        "Twin Axial Pair (TW) compliant (Bit 7)": false,
        "Twisted Pair (TP) compliant (Bit 6)": false,
        "Miniature Coax (MI) compliant (Bit 5)": false,
        "Video Coax (TV) compliant (Bit 4)": false,
        "Multimode, 62.5um (M6) compliant (Bit 3)": false,
        "Multimode, 50um (M5, M5E) compliant (Bit 2)": false,
        "Reserved (Bit 1)": false,
        "Single Mode (SM) compliant (Bit 0)": true
    },
    "Fibre Channel Speed": {
        "1200 MBytes/sec compliant (Bit 7)": false,
        "800 MBytes/sec compliant (Bit 6)": true,
        "1600 MBytes/sec compliant (Bit 5)": false,
        "400 MBytes/sec compliant (Bit 4)": true,
        "3200 MBytes/sec compliant (Bit 3)": false,
        "200 MBytes/sec compliant (Bit 2)": true,
        "See byte 62 \"Fibre Channel Speed 2\" (Bit 1)": false,
        "100 MBytes/sec compliant (Bit 0)": false
    },
    "Encoding": "8B/10B",
    "Nominal Signaling Rate [MBaud] (Divisible by 100)": 8500,
    "Rate Identifier": "Unspecified (00h)",
    "Link Length": {
        "Type": "Fibre",
        "Length (SMF) [km]": 10,
        "Length (SMF) [m] (Divisible by 100)": 10000,
        "Length (OM2 50 um) [m] (Divisible by 10)": "N/A",
        "Length (OM1 62.5 um) [m] (Divisible by 10)": "N/A",
        "Length (OM4 50 um) [m] (Divisible by 10)": "N/A",
        "Length (OM3 50 um) [m] (Divisible by 10)": "N/A"
    },
    "Vendor Name": "BROCADE",
    "Extended Specification Compliance Codes": "Unspecified",
    "Vendor OUI": {
        "Vendor Name": "Brocade Communications Systems LLC"
    },
    "Vendor Part Number": "57-1000027-01",
    "Vendor Rev": "A",
    "Wavelength Or Cable Specification Compliance": {
        "Type": "Active Cable",
        "Reserved (Byte 60, Bit 7)": false,
        "Reserved (Byte 60, Bit 6)": false,
        "Reserved (Byte 60, Bit 5)": false,
        "Reserved (Byte 60, Bit 4)": false,
        "FC-PI-4 Limiting compliant (Byte 60, Bit 3)": false,
        "SFF-8431 Limiting compliant (Byte 60, Bit 2)": true,
        "FC-PI-4 Appendix H compliant (Byte 60, Bit 1)": false,
        "SFF-8431 Appendix E compliant (Byte 60, Bit 0)": true,
        "Reserved (Byte 61, Bit 7)": false,
        "Reserved (Byte 61, Bit 6)": false,
        "Reserved (Byte 61, Bit 5)": false,
        "Reserved (Byte 61, Bit 4)": true,
        "Reserved (Byte 61, Bit 3)": true,
        "Reserved (Byte 61, Bit 2)": true,
        "Reserved (Byte 61, Bit 1)": true,
        "Reserved (Byte 61, Bit 0)": false
    },
    "Fibre Channel Speed 2": {
        "Reserved (Bit 7)": false,
        "Reserved (Bit 6)": false,
        "Reserved (Bit 5)": false,
        "Reserved (Bit 4)": false,
        "Reserved (Bit 3)": false,
        "Reserved (Bit 2)": false,
        "Reserved (Bit 1)": false,
        "64 GFC compliant (Bit 0)": false
    },
    "CC_BASE": "auto",
    "Option Values": {
        "Reserved (Byte 64, Bit 7)": false,
        "High Power Level (Level 4) requirement (Byte 64, Bit 6)": false,
        "High Power Level (Level 3 or Level 4) requirement (Byte 64, Bit 5)": false,
        "Paging implemented (Byte 64, Bit 4)": false,
        "Internal retimer or data recovery (CDR) circuit present (Byte 64, Bit 3)": false,
        "Cooled Laser (Byte 64, Bit 2)": false,
        "Power Level 2 requirement (Byte 64, Bit 1)": false,
        "Linear Receiver Output implemented (Byte 64, Bit 0)": false,
        "Receiver decision threshold implemented (Byte 65, Bit 7)": false,
        "Transmitter tunable (Byte 65, Bit 6)": false,
        "RATE_SELECT functionality implemented (Byte 65, Bit 5)": false,
        "TX_DISABLE implemented (Byte 65, Bit 4)": true,
        "TX_FAULT signal implemented (Byte 65, Bit 3)": true,
        "Loss of Signal with inverted signal implemented (Byte 65, Bit 2)": false,
        "Loss of Signal implemented (Byte 65, Bit 1)": true,
        "Reserved (Byte 65, Bit 0)": false
    },
    "Extended Signaling Rate": {
        "Type": "Standard",
        "Upper signaling rate margin [%]": 0,
        "Lower signaling rate margin [%]": 0
    },
    "Vendor Serial Number": "UDA112361014441",
    "Date Code": {
        "Year low order digits (Byte 84-85)": 12,
        "Month digits (Byte 86-87)": 9,
        "Day (Byte 88-89)": 15,
        "Vendor Lot Code (Byte 90-91)": ""
    },
    "Diagnostic Monitoring Type": {
        "Reserved (Bit 7)": false,
        "Digital diagnostic monitoring implemented (Bit 6)": true,
        "Internally calibrated (Bit 5)": true,
        "Externally calibrated (Bit 4)": false,
        "Received power measurement type (Bit 3)": "Average",
        "Address change required (Bit 2)": false,
        "Reserved (Bit 1)": false,
        "Reserved (Bit 0)": false
    },
    "Enhanced Options": {
        "Optional Alarm/warning flags implemented (Bit 7)": true,
        "Optional soft TX_DISABLE control and monitoring implemented (Bit 6)": true,
        "Optional soft TX_FAULT monitoring implemented (Bit 5)": true,
        "Optional soft RX_LOS monitoring implemented (Bit 4)": true,
        "Optional soft RATE_SELECT control and monitoring implemented (Bit 3)": false,
        "Optional Application Select control implemented per SFF-8079 (Bit 2)": false,
        "Optional soft Rate Select control implemented per SFF-8431 (Bit 1)": false,
        "Reserved (Bit 0)": false
    },
    "SFF-8472 Compliance": "Includes functionality described in Rev 10.2 of SFF-8472",
    "CC_EXT": "auto",
    "Vendor Specific": {
        "Type": "Base64",
        "Value": "QUZDVC01N0Q1QVRQWi1CMSAgICAgICAgICAgIDEgICA="
    }
}
  ```

</details>

<details>
  <summary>SFF-8636 (QSFP+) Pretty Print (Finisar FTL414QB2N-E5) </summary>

```
TransceiverTool decode --binary-type 8636-256b --pretty-print --from-file FTL414QB2N-E5.bin

Type                                                                                 : SFF-8636

Identifier [128]                                                                     : QSFP+ or later with SFF-8636 or SFF-8436 management interface (SFF-8436, SFF-8635, SFF-8665, SFF-8685 et al.) (0x0d)

Extended Identifier [129, 7-6]                                                       : Power Class 1 (1.5 W max.)
Extended Identifier [129, 5]                                                         : Power Class 8 not implemented
Extended Identifier [129, 4]                                                         : No CLEI code present in Page 02h
Extended Identifier [129, 3]                                                         : No CDR in Tx
Extended Identifier [129, 2]                                                         : No CDR in Rx
Extended Identifier [129, 1-0]                                                       : Power Classes 1 to 4

Connector Type [130]                                                                 : MPO 1x12 (Multifiber Parallel Optic) (0x0c)

Specification Compliance, 10/40G/100G Ethernet Compliance Codes [131, 7]             : Standard
Specification Compliance, 10/40G/100G Ethernet Compliance Codes [131, 6]             : Not 10GBASE-LRM compliant
Specification Compliance, 10/40G/100G Ethernet Compliance Codes [131, 5]             : Not 10GBASE-LR compliant
Specification Compliance, 10/40G/100G Ethernet Compliance Codes [131, 4]             : Not 10GBASE-SR compliant
Specification Compliance, 10/40G/100G Ethernet Compliance Codes [131, 3]             : Not 40GBASE-CR4 compliant
Specification Compliance, 10/40G/100G Ethernet Compliance Codes [131, 2]             : Not 40GBASE-SR4 compliant
Specification Compliance, 10/40G/100G Ethernet Compliance Codes [131, 1]             : Not 40GBASE-LR4 compliant
Specification Compliance, 10/40G/100G Ethernet Compliance Codes [131, 0]             : Not 40G Active Cable (XLPPI) compliant

Specification Compliance, SONET Compliance Codes [132, 7]                            : N/A
Specification Compliance, SONET Compliance Codes [132, 6]                            : N/A
Specification Compliance, SONET Compliance Codes [132, 5]                            : N/A
Specification Compliance, SONET Compliance Codes [132, 4]                            : N/A
Specification Compliance, SONET Compliance Codes [132, 3]                            : N/A
Specification Compliance, SONET Compliance Codes [132, 2]                            : Not OC 48, long reach compliant
Specification Compliance, SONET Compliance Codes [132, 1]                            : Not OC 48, intermediate reach compliant
Specification Compliance, SONET Compliance Codes [132, 0]                            : Not OC 48, short reach compliant

Specification Compliance, SAS/SATA Compliance Codes [133, 7]                         : Not SAS 24.0 Gbps compliant
Specification Compliance, SAS/SATA Compliance Codes [133, 6]                         : Not SAS 12.0 Gbps compliant
Specification Compliance, SAS/SATA Compliance Codes [133, 5]                         : Not SAS 6.0 Gbps compliant
Specification Compliance, SAS/SATA Compliance Codes [133, 4]                         : Not SAS 3.0 Gbps compliant
Specification Compliance, SAS/SATA Compliance Codes [133, 3]                         : N/A
Specification Compliance, SAS/SATA Compliance Codes [133, 2]                         : N/A
Specification Compliance, SAS/SATA Compliance Codes [133, 1]                         : N/A
Specification Compliance, SAS/SATA Compliance Codes [133, 0]                         : N/A

Specification Compliance, Gigabit Ethernet Compliance Codes [134, 7]                 : N/A
Specification Compliance, Gigabit Ethernet Compliance Codes [134, 6]                 : N/A
Specification Compliance, Gigabit Ethernet Compliance Codes [134, 5]                 : N/A
Specification Compliance, Gigabit Ethernet Compliance Codes [134, 4]                 : N/A
Specification Compliance, Gigabit Ethernet Compliance Codes [134, 3]                 : Not 1000BASE-T compliant
Specification Compliance, Gigabit Ethernet Compliance Codes [134, 2]                 : Not 1000BASE-CX compliant
Specification Compliance, Gigabit Ethernet Compliance Codes [134, 1]                 : Not 1000BASE-LX compliant
Specification Compliance, Gigabit Ethernet Compliance Codes [134, 0]                 : Not 1000BASE-SX compliant

Specification Compliance, Fibre Channel Link Length [135, 7]                         : Not Very long distance (V) compliant
Specification Compliance, Fibre Channel Link Length [135, 6]                         : Short distance (S) compliant
Specification Compliance, Fibre Channel Link Length [135, 5]                         : Not Intermediate distance (I) compliant
Specification Compliance, Fibre Channel Link Length [135, 4]                         : Not Long distance (L) compliant
Specification Compliance, Fibre Channel Link Length [135, 3]                         : Not Medium distance (M) compliant

Specification Compliance, Fibre Channel Transmitter Technology [135, 2]              : N/A
Specification Compliance, Fibre Channel Transmitter Technology [135, 1]              : Not Longwave laser (LC) compliant
Specification Compliance, Fibre Channel Transmitter Technology [135, 0]              : Not Electrical inter-enclosure (EL) compliant
Specification Compliance, Fibre Channel Transmitter Technology [136, 7]              : Not Electrical intra-enclosure compliant
Specification Compliance, Fibre Channel Transmitter Technology [136, 6]              : Shortwave laser w/o OFC (SN) compliant
Specification Compliance, Fibre Channel Transmitter Technology [136, 5]              : Not Shortwave laser w OFC (SL) compliant
Specification Compliance, Fibre Channel Transmitter Technology [136, 4]              : Not Longwave Laser (LL) compliant
Specification Compliance, Fibre Channel Transmitter Technology [136, 3]              : N/A
Specification Compliance, Fibre Channel Transmitter Technology [136, 2]              : N/A
Specification Compliance, Fibre Channel Transmitter Technology [136, 1]              : N/A
Specification Compliance, Fibre Channel Transmitter Technology [136, 0]              : N/A

Specification Compliance, Fibre Channel Transmission Media [137, 7]                  : Not Twin Axial Pair (TW) compliant
Specification Compliance, Fibre Channel Transmission Media [137, 6]                  : Not Shielded Twisted Pair (TP) compliant
Specification Compliance, Fibre Channel Transmission Media [137, 5]                  : Not Miniature Coax (MI) compliant
Specification Compliance, Fibre Channel Transmission Media [137, 4]                  : Not Video Coax (TV) compliant
Specification Compliance, Fibre Channel Transmission Media [137, 3]                  : Not Multi-mode 62.5 um (M6) compliant
Specification Compliance, Fibre Channel Transmission Media [137, 2]                  : Not Multi-mode 50 um (M5) compliant
Specification Compliance, Fibre Channel Transmission Media [137, 1]                  : Not Multi-mode 50 um (OM3) compliant
Specification Compliance, Fibre Channel Transmission Media [137, 0]                  : Not Single Mode (SM) compliant

Specification Compliance, Fibre Channel Speed [138, 7]                               : 1200 MBps (per channel) compliant
Specification Compliance, Fibre Channel Speed [138, 6]                               : 800 MBps compliant
Specification Compliance, Fibre Channel Speed [138, 5]                               : 1600 MBps (per channel) compliant
Specification Compliance, Fibre Channel Speed [138, 4]                               : 400 MBps compliant
Specification Compliance, Fibre Channel Speed [138, 3]                               : Not 3200 MBps (per channel) compliant
Specification Compliance, Fibre Channel Speed [138, 2]                               : 200 MBps compliant
Specification Compliance, Fibre Channel Speed [138, 1]                               : Standard
Specification Compliance, Fibre Channel Speed [138, 0]                               : 100 MBps compliant

Encoding [139]                                                                       : 64B/66B (0x05)

Signaling rate, nominal [140]                                                        : 14000 MBd

Extended Rate Select Compliance [141, 7]                                             : N/A
Extended Rate Select Compliance [141, 6]                                             : N/A
Extended Rate Select Compliance [141, 5]                                             : N/A
Extended Rate Select Compliance [141, 4]                                             : N/A
Extended Rate Select Compliance [141, 3]                                             : N/A
Extended Rate Select Compliance [141, 2]                                             : N/A
Extended Rate Select Compliance [141, 1-0]                                           : Reserved / Not Implemented? (00b)

Length (SMF) [142]                                                                   : Not Applicable (e.g. AOC) / SM fiber not supported / must be determined from the free side device

Length (OM3 50 um) [143]                                                             : 60 m

Length (OM2 50 um) [144]                                                             : Not Applicable (e.g. AOC) / OM2 fiber type not supported / must be determined from the free side device

Length (OM1 62.5 um) [145]                                                           : Not Applicable (e.g. AOC) / OM1 fiber type not supported / must be determined from the free side device

Length (OM4 50 um) [146]                                                             : 100 m

Device & Transmitter technology [147, 7-4]                                           : 850 nm VCSEL
Device & Transmitter technology [147, 3]                                             : No wavelength control
Device & Transmitter technology [147, 2]                                             : Unooled transmitter
Device & Transmitter technology [147, 1]                                             : Pin detector
Device & Transmitter technology [147, 0]                                             : Transmitter NOT tunable

Vendor Name (wrapping quotes added by TransceiverTool) [148-163]                     : "FINISAR CORP"

Extended Module Codes [164, 7]                                                       : N/A
Extended Module Codes [164, 6]                                                       : N/A
Extended Module Codes [164, 5]                                                       : No support for HDR (200G) Infiniband
Extended Module Codes [164, 4]                                                       : No support for EDR (100G) Infiniband
Extended Module Codes [164, 3]                                                       : Supports FDR (56G) Infiniband
Extended Module Codes [164, 2]                                                       : Supports QDR (40G) Infiniband
Extended Module Codes [164, 1]                                                       : Supports DDR (20G) Infiniband
Extended Module Codes [164, 0]                                                       : Supports SDR (10G) Infiniband

Vendor OUI [165-167]                                                                 : 00:90:65 (Vendor name is "FINISAR CORPORATION")

Vendor PN (wrapping quotes added by TransceiverTool) [168-183]                       : "FTL414QB2N-E5"

Vendor Rev (wrapping quotes added by TransceiverTool) [184-185]                      : "B"

Wavelength [186-187]                                                                 : 850 nm

Wavelength Tolerance [188-189]                                                       : 10 nm

Max case temperature [190]                                                           : 85 deg C

CC_BASE checksum [191]                                                               : 0x26 (correct)

Extended Specification Compliance [192]                                              : Unspecified (0x00)

Options [193, 7]                                                                     : N/A
Options [193, 6]                                                                     : LPMode/TxDis input signal NOT configurable
Options [193, 5]                                                                     : IntL/RxLOSL output signal NOT configurable
Options [193, 4]                                                                     : Tx input adaptive equalizers NOT freeze capable
Options [193, 3]                                                                     : Tx input equalizers NOT auto-adaptive capable
Options [193, 2]                                                                     : Tx input equalizers fixed-programmable settings NOT implemented
Options [193, 1]                                                                     : Rx output emphasis fixed-programmable settings NOT implemented
Options [193, 0]                                                                     : Rx output amplitude fixed-programmable settings implemented

Options [194, 7]                                                                     : Tx CDR On/Off Control NOT implemented
Options [194, 6]                                                                     : Rx CDR On/Off Control NOT implemented
Options [194, 5]                                                                     : Tx CDR Loss of Lock (LOL) flag NOT implemented
Options [194, 4]                                                                     : Rx CDR Loss of Lock (LOL) flag NOT implemented
Options [194, 3]                                                                     : Rx Squelch Disable NOT implemented
Options [194, 2]                                                                     : Rx Output Disable implemented
Options [194, 1]                                                                     : Tx Squelch Disable NOT implemented
Options [194, 0]                                                                     : Tx Squelch NOT implemented

Options [195, 7]                                                                     : Memory Page 02h provided
Options [195, 6]                                                                     : Memory Page 01h provided
Options [195, 5]                                                                     : Rate select NOT implemented as defined in 6.2.7
Options [195, 4]                                                                     : Tx_Disable is implemented
Options [195, 3]                                                                     : Tx_Fault signal NOT implemented
Options [195, 2]                                                                     : Tx Squelch implemented to reduce OMA
Options [195, 1]                                                                     : Tx Loss of Signal NOT implemented
Options [195, 0]                                                                     : Pages 20-21h NOT implemented

Vendor S/N (wrapping quotes added by TransceiverTool) [196-211]                      : "X0HA6RM"

Date Code, low order digits of year [212-213]                                        : 18
Date Code, digits of month [214-215]                                                 : 11
Date Code, digits of day [216-217]                                                   : 08
Date Code, lot code (wrapping quotes added by TransceiverTool) [218-219]             : "  "

Diagnostic Monitoring Type [220, 7]                                                  : N/A
Diagnostic Monitoring Type [220, 6]                                                  : N/A
Diagnostic Monitoring Type [220, 5]                                                  : Temperature monitoring NOT implemented
Diagnostic Monitoring Type [220, 4]                                                  : Supply voltage monitoring NOT implemented
Diagnostic Monitoring Type [220, 3]                                                  : Received power measurements type = Average
Diagnostic Monitoring Type [220, 2]                                                  : Transmitter power measurement NOT supported
Diagnostic Monitoring Type [220, 1]                                                  : N/A
Diagnostic Monitoring Type [220, 0]                                                  : N/A

Enhanced Options [221, 7]                                                            : N/A
Enhanced Options [221, 6]                                                            : N/A
Enhanced Options [221, 5]                                                            : N/A
Enhanced Options [221, 4]                                                            : Initialization Complete Flag NOT implemented
Enhanced Options [221, 3]                                                            : Rate selection is NOT implemented
Enhanced Options [221, 2]                                                            : N/A
Enhanced Options [221, 1]                                                            : TC readiness flag NOT implemented
Enhanced Options [221, 0]                                                            : Software reset is NOT implemented

Baud Rate, nominal [222]                                                             : Unspecified

CC_EXT checksum [223]                                                                : 0x56 (correct)

Vendor Specific (wrapping quotes added by TransceiverTool) [224-255]                 : " 019-078-048 A01 000000000000000"
```

</details>

<details>
  <summary>SFF-8636 (QSFP+) JSON (Finisar FTL414QB2N-E5) </summary>

```
TransceiverTool decode --binary-type 8636-256b --json-to-stdout --from-file FTL414QB2N-E5.bin
{
    "GeneratedBy": "TransceiverTool v0.0.1",
    "Type": "SFF-8636 Rev 2.11 Upper Page 00h",
    "Identifier": "QSFP+ or later with SFF-8636 or SFF-8436 management interface (SFF-8436, SFF-8635, SFF-8665, SFF-8685 et al.)",
    "Extended Identifier": {
        "Power Class (Bit 7-6)": "Power Class 1 (1.5 W max.)",
        "Power Class 8 implemented (Bit 5)": false,
        "CLEI Code Present (Bit 4)": false,
        "CDR present in Tx (Bit 3)": false,
        "CDR present in Rx (Bit 2)": false,
        "Extended Power Class (Bit 1-0)": "Power Classes 1 to 4"
    },
    "Connector Type": "MPO 1x12 (Multifiber Parallel Optic)",
    "10/40G/100G Ethernet Compliance Codes": {
        "Extended (Bit 7)": false,
        "10GBASE-LRM compliant (Bit 6)": false,
        "10GBASE-LR compliant (Bit 5)": false,
        "10GBASE-SR compliant (Bit 4)": false,
        "40GBASE-CR4 compliant (Bit 3)": false,
        "40GBASE-SR4 compliant (Bit 2)": false,
        "40GBASE-LR4 compliant (Bit 1)": false,
        "40G Active Cable (XLPPI) compliant (Bit 0)": false
    },
    "SONET Compliance Codes": {
        "Reserved (Bit 7)": false,
        "Reserved (Bit 6)": false,
        "Reserved (Bit 5)": false,
        "Reserved (Bit 4)": false,
        "Reserved (Bit 3)": false,
        "OC 48, long reach compliant (Bit 2)": false,
        "OC 48, intermediate reach compliant (Bit 1)": false,
        "OC 48, short reach compliant (Bit 0)": false
    },
    "SAS/SATA Compliance Codes": {
        "SAS 24.0 Gbps compliant (Bit 7)": false,
        "SAS 12.0 Gbps compliant (Bit 6)": false,
        "SAS 6.0 Gbps compliant (Bit 5)": false,
        "SAS 3.0 Gbps compliant (Bit 4)": false,
        "Reserved (Bit 3)": false,
        "Reserved (Bit 2)": false,
        "Reserved (Bit 1)": false,
        "Reserved (Bit 0)": false
    },
    "Gigabit Ethernet Compliance Codes": {
        "Reserved (Bit 7)": false,
        "Reserved (Bit 6)": false,
        "Reserved (Bit 5)": false,
        "Reserved (Bit 4)": false,
        "1000BASE-T compliant (Bit 3)": false,
        "1000BASE-CX compliant (Bit 2)": false,
        "1000BASE-LX compliant (Bit 1)": false,
        "1000BASE-SX compliant (Bit 0)": false
    },
    "Fibre Channel Link Length": {
        "Intermediate distance (I) compliant (Byte 135, Bit 5)": false,
        "Long distance (L) compliant (Byte 135, Bit 4)": false,
        "Medium distance (M) compliant (Byte 135, Bit 3)": false,
        "Short distance (S) compliant (Byte 135, Bit 6)": true,
        "Very long distance (V) compliant (Byte 135, Bit 7)": false
    },
    "Fibre Channel Transmitter Technology": {
        "Electrical inter-enclosure (EL) compliant (Byte 135, Bit 0)": false,
        "Electrical intra-enclosure compliant (Byte 136, Bit 7)": false,
        "Longwave Laser (LL) compliant (Byte 136, Bit 4)": false,
        "Longwave laser (LC) compliant (Byte 135, Bit 1)": false,
        "Reserved (Byte 135, Bit 2)": false,
        "Reserved (Byte 136, Bit 0)": false,
        "Reserved (Byte 136, Bit 1)": false,
        "Reserved (Byte 136, Bit 2)": false,
        "Reserved (Byte 136, Bit 3)": false,
        "Shortwave laser w OFC (SL) compliant (Byte 136, Bit 5)": false,
        "Shortwave laser w/o OFC (SN) compliant (Byte 136, Bit 6)": true
    },
    "Fibre Channel Transmission Media": {
        "Twin Axial Pair (TW) compliant (Bit 7)": false,
        "Shielded Twisted Pair (TP) compliant (Bit 6)": false,
        "Miniature Coax (MI) compliant (Bit 5)": false,
        "Video Coax (TV) compliant (Bit 4)": false,
        "Multi-mode 62.5 um (M6) compliant (Bit 3)": false,
        "Multi-mode 50 um (M5) compliant (Bit 1)": false,
        "Multi-mode 50 um (OM3) compliant (Bit 1)": true,
        "Single Mode (SM) compliant (Bit 0)": false
    },
    "Fibre Channel Speed": {
        "1200 MBps (per channel) compliant (Bit 7)": true,
        "800 MBps compliant (Bit 6)": true,
        "1600 MBps (per channel) compliant (Bit 5)": true,
        "400 MBps compliant (Bit 4)": true,
        "3200 MBps (per channel) compliant (Bit 3)": false,
        "200 MBps compliant (Bit 2)": true,
        "Extended (Bit 1)": false,
        "100 MBps compliant (Bit 0)": true
    },
    "Encoding": "64B/66B",
    "Nominal Signaling Rate [MBaud] (Divisible by 100)": 14000,
    "Extended Rate Select Compliance": {
        "Reserved (Bit 7)": false,
        "Reserved (Bit 6)": false,
        "Reserved (Bit 5)": false,
        "Reserved (Bit 4)": false,
        "Reserved (Bit 3)": false,
        "Reserved (Bit 2)": false,
        "Rate Select Version (Bit 1-0)": "Reserved / Not Implemented? (00b)"
    },
    "Length (Standard SM Fiber) [km]": "N/A",
    "Length (OM3 50 um) [m] (Divisible by 2)": 60,
    "Length (OM2 50 um) [m]": "N/A",
    "Copper or Fibre Properties": {
        "Type": "Fibre",
        "Length (OM1 62.5 um) [m]": "N/A",
        "Length (OM4 50 um) [m] (Divisible by 2)": 100,
        "Wavelength [nm] (Divisible by 0.05)": 850.0,
        "Wavelength Tolerance [nm] (Divisible by 0.005)": 10.0
    },
    "Device & Transmitter Properties": {
        "Transmitter Technology (Bit 7-4)": "850 nm VCSEL",
        "Active Wavelength Control (Bit 3)": false,
        "Cooled Transmitter (Bit 2)": false,
        "Detector Type (Bit 1)": "PIN",
        "Tunable Transmitter (Bit 0)": false
    },
    "Vendor Name": "FINISAR CORP",
    "Extended Module Codes (Infiniband)": {
        "Reserved (Bit 7)": false,
        "Reserved (Bit 6)": false,
        "HDR (200G) Infiniband supported (Bit 5)": false,
        "EDR (100G) Infiniband supported (Bit 4)": false,
        "FDR (56G) Infiniband supported (Bit 3)": true,
        "QDR (40G) Infiniband supported (Bit 2)": true,
        "DDR (20G) Infiniband supported (Bit 1)": true,
        "SDR (10G) Infiniband supported (Bit 0)": true
    },
    "Vendor OUI": {
        "Vendor Name": "FINISAR CORPORATION"
    },
    "Vendor Part Number": "FTL414QB2N-E5",
    "Vendor Rev": "B",
    "Maximum Case Temperature [degC]": 85,
    "CC_BASE": "auto",
    "Extended Specification Compliance Codes": "Unspecified",
    "Options": {
        "Reserved (Byte 193, Bit 7)": false,
        "LPMode/TxDis input signal configurable (Byte 193, Bit 6)": false,
        "IntL/RxLOSL output signal configurable (Byte 193, Bit 5)": false,
        "Tx input adaptive equalizers freeze capable (Byte 193, Bit 4)": false,
        "Tx input equalizers auto-adaptive capable (Byte 193, Bit 3)": false,
        "Tx input equalizers fixed-programmable settings implemented (Byte 193, Bit 2)": false,
        "Rx output emphasis fixed-programmable settings implemented (Byte 193, Bit 1)": false,
        "Rx output amplitude fixed-programmable settings implemented (Byte 193, Bit 0)": true,
        "Tx CDR On/Off Control implemented (Byte 194, Bit 7)": false,
        "Rx CDR On/Off Control implemented (Byte 194, Bit 6)": false,
        "Tx CDR Loss of Lock (LOL) flag implemented (Byte 194, Bit 5)": false,
        "Rx CDR Loss of Lock (LOL) flag implemented. (Byte 194, Bit 4)": false,
        "Rx Squelch Disable implemented (Byte 194, Bit 3)": false,
        "Rx Output Disable implemented (Byte 194, Bit 2)": true,
        "Tx Squelch Disable implemented (Byte 194, Bit 1)": false,
        "Tx Squelch implemented (Byte 194, Bit 0)": false,
        "Memory Page 02 provided (Byte 195, Bit 7)": true,
        "Memory Page 01h provided. (Byte 195, Bit 6)": true,
        "Rate select is implemented as defined in Section 6.2.7 (Byte 195, Bit 5)": false,
        "Tx_Disable is implemented (Byte 195, Bit 4)": true,
        "Tx_Fault signal implemented (Byte 195, Bit 3)": false,
        "Tx Squelch implemented to reduce Pave (Byte 195, Bit 2)": false,
        "Tx Loss of Signal (Byte 195, Bit 1)": false,
        "Pages 20-21h implemented (Byte 195, Bit 0)": false
    },
    "Vendor Serial Number": "X0HA6RM",
    "Date Code": {
        "Year low order digits (Byte 212-213)": 18,
        "Month digits (Byte 214-215)": 11,
        "Day (Byte 216-217)": 8,
        "Vendor Lot Code (Byte 218-219)": ""
    },
    "Diagnostic Monitoring Type": {
        "Reserved (Bit 7)": false,
        "Reserved (Bit 6)": false,
        "Temperature monitoring implemented (Bit 5)": false,
        "Supply voltage monitoring implemented (Bit 4)": false,
        "Received power measurements type (Bit 3)": "Average",
        "Transmitter power measurement supported (Bit 2)": false,
        "Reserved (Bit 1)": false,
        "Reserved (Bit 0)": false
    },
    "Enhanced Options": {
        "Reserved (Bit 7)": false,
        "Reserved (Bit 6)": false,
        "Reserved (Bit 5)": false,
        "Initialization Complete Flag implemented (Bit 4)": false,
        "Extended Rate Selection implemented (Bit 3)": false,
        "Reserved (Bit 2)": false,
        "TC readiness flag implemented (Bit 1)": false,
        "Software reset implemented (Bit 0)": false
    },
    "Extended Baud Rate [MBaud] (Divisible by 250)": "Unspecified",
    "CC_EXT": "auto",
    "Vendor Specific": {
        "Type": "Base64",
        "Value": "IDAxOS0wNzgtMDQ4IEEwMSAwMDAwMDAwMDAwMDAwMDA="
    }
}
```

</details>


## Usage
```
TransceiverTool v0.0.1 - Encode / Assemble from JSON, Decode / Parse to JSON and Pretty Print SFF-8472 SFP/+/28 and SFF-8636 QSFP/+/28 programmings / EEPROMs!
Usage: ./build/TransceiverTool [OPTIONS] [SUBCOMMAND]

Options:
  --version                   Display program version information and exit
  --help                      Print this help message and exit

Subcommands:
decode
  Decode / parse binary transceiver programming to JSON and/or pretty print
  Options:
    --binary-type ENUM:value in {8472-128b->0,8636-128b->1,8636-256b->2} OR {0,1,2} REQUIRED
                                Type of binary input
                                8472-128b: SFF-8472 (SFP/+/28) and 128 byte, 
                                8636-128b: SFF-8636 (QSFP/+/28) and 128 byte, aka upper page 00h or the last 128 bytes of page 00h or
                                8636-256b: SFF-8636 (QSFP/+/28) and 256 byte, aka full page 00h and first 128 bytes ignored / zeroed
    --fiber-mode,--copper-mode{false}
                                Specify meaning of bytes that can have multiple meanings, default fiber-mode
    --from-file TEXT            File path from which the binary shall be read instead of stdin
    --pretty-print              Pretty-prints the decoded / parsed binary programming to stdout
    --json-to-stdout            Print the decoded / parsed binary programming as JSON to stdout
    --pretty-print-to-file TEXT Write the pretty-print of the decoded / parsed binary programming to a file
    --json-to-file TEXT         Write the JSON of the decoded / parsed binary programming

encode
  Encode / assembly binary transceiver programming from JSON
  Options:
    --binary-type ENUM:value in {8472-128b->0,8636-128b->1,8636-256b->2} OR {0,1,2} REQUIRED
                                Type of binary output
                                8472-128b: SFF-8472 (SFP/+/28) and 128 byte, 
                                8636-128b: SFF-8636 (QSFP/+/28) and 128 byte, aka upper page 00h or the last 128 bytes of page 00h or
                                8636-256b: SFF-8636 (QSFP/+/28) and 256 byte, aka full page 00h and first 128 bytes ignored / zeroed
    --from-file TEXT            File path from which the JSON shall be read instead of stdin
    --to-file TEXT              File path to which the binary shall be written instead of stdout
    --validate                  Try to validate programming, don't write file if any errors occur

pretty-print-json
  Pretty print a JSON file of a decoded programming
  Options:
    --fiber-mode,--copper-mode{false}
                                Specify meaning of bytes that can have multiple meanings, default fiber-mode
    --from-file TEXT            File path from which the JSON shall be read instead of stdin
    --to-file TEXT              File path to which the pretty print shall be written instead of stdout

validate-json
  Validate a JSON file of a decoded programming
  Options:
    --from-file TEXT            File path from which the JSON shall be read instead of stdin

roundtrip-test
  Perform a roundtrip test (Binary -> JSON -> Binary)
  Options:
    --binary-type ENUM:value in {8472-128b->0,8636-128b->1,8636-256b->2} OR {0,1,2} REQUIRED
                                Type of binary input
                                8472-128b: SFF-8472 (SFP/+/28) and 128 byte, 
                                8636-128b: SFF-8636 (QSFP/+/28) and 128 byte, aka upper page 00h or the last 128 bytes of page 00h or
                                8636-256b: SFF-8636 (QSFP/+/28) and 256 byte, aka full page 00h and first 128 bytes ignored / zeroed
    --from-file TEXT            File path from which the binary shall be read instead of stdin
```


## Building

REQUIRED TOOLS:
- Conan v2 (e.g. v2.1.0)
- CMake >= 3.27 (e.g. v3.27.4)


We use Conan in order to manage (some of) our external dependencies

In order to build TransceiverTool, use the following commands

ONLY IF THIS MACHINE IS FRESH AND YOU HAVE NEVER USED CONAN BEFORE (QUICK START):
```
conan profile detect --force
```

Note that you might want to modify this profile, e.g. to make sure everything is built with Clang


```
conan config install conan-profiles
```
This will install the settings from the conan-profiles subfolder (last argument - we could also have named it "ILikeTrees") to your conan default directory in order that you can use the required profiles and settings


To install the external libraries:
```
conan install . --output-folder build --build=missing -pr:a default -pr:a TransceiverTool-<build_type>
```
The last two arguments are important (and their order!!)
`-pr:a default` instructs conan to use your default profile which specifies base stuff like compiler, OS, etc and is required
`-pr:a TransceiverTool-<build_type>` sets out build type

This will also automatically generate some files like `CMakeUserPresets.json` (which in turn includes `CMakePresets.json`)

We include `-DCMAKE_EXPORT_COMPILE_COMMANDS=1` by default. This will generate the `compile_commands.json` file needed by the Clangd language server to provide autocompletion in VSCode

Release Build:
```
conan install . --output-folder build --build=missing -pr:a default -pr:a TransceiverTool-release
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 --preset conan-release
cmake --build --preset conan-release
```

RelWithDebInfo Build:
```
conan install . --output-folder build --build=missing -pr:a default -pr:a TransceiverTool-relwithdebinfo
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 --preset conan-relwithdebinfo
cmake --build --preset conan-relwithdebinfo
```

Debug Build:
```
conan install . --output-folder build --build=missing -pr:a default -pr:a TransceiverTool-debug
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 --preset conan-debug
cmake --build --preset conan-debug
```
