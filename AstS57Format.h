#ifndef ASTS57FORMAT_H
#define ASTS57FORMAT_H

#include <QObject>

#pragma pack(push, 1)


// DR leader формат
#define ENTRY_RP_RECORD_LENGTH           (-5)
#define ENTRY_RP_BASE_ADDR_FIELD_AREA    12
#define ENTRY_RP_SIZE_OF_LENGTH_FIELD    20
#define ENTRY_RP_SIZE_OF_POSITION_FIELD  21
#define ENTRY_RP_SIZE_OF_TAG_FIELD       23
#define ENTRY_RP_DATA                    24

#define ENTRY_Len_RECORD_LENGTH          5
#define ENTRY_Len_BASE_ADDR_FIELD_AREA   5
#define ENTRY_Len_SIZE_OF_LENGTH_FIELD   1
#define ENTRY_Len_SIZE_OF_POSITION_FIELD 1
#define ENTRY_Len_SIZE_OF_TAG_FIELD      1

#define TERMINAL_BYTE             0x1e

struct SG2D_COORDINATES
{
    uint32_t Y;
    uint32_t X;
};

struct SG3D_COORDINATES
{
    uint32_t Y;
    uint32_t X;
    uint32_t V;
};

#define DSPM_RECORD_NAME          20

enum RECORD_NAMES
{
    FRID_RECORD    = 100,
    ISOLATED_NODE  = 110,
    CONNECTED_NODE = 120,
    EDGE_NODE      = 130,
    FACE_NODE      = 140
};

enum GEOMETRIC_PRIMITIVE_TYPES
{
    POINT_PTYPE = 1,
    LINE_PTYPE  = 2,
    AREA_PTYPE  = 3,
    NONE_PTYPE  = 255
};

struct DSPM
{
    unsigned char                 RCNM;                               // Record name RCNM A(2) b11 an "DP" {20}
    unsigned long                 RCID;                               // Record identification number RCID I(10) b14 int Range: 1 to 2^32-2
    unsigned char                 HDAT;                               // Horizontal geodetic datum HDAT I(3) b11 int Value taken from the attribute HORDAT (see Appendix A - Object Catalogue)
    unsigned char                 VDAT;                               // Vertical datum VDAT I(2) b11 int Value taken from the attribute VERDAT (see Appendix A - Object Catalogue)
    unsigned char                 SDAT;                               // Sounding datum SDAT I(2) b11 int Value taken from the attribute VERDAT (see Appendix A - Object Catalogue)
    unsigned long                 CSCL;                               // Compilation scale of data CSCL I( ) b14 int The modulus of the compilation scale. For example, a scale of 1:25000 is encoded as 25000
    unsigned char                 DUNI;                               // Units of depth measurement DUNI I(2) b11 int Value taken from the attribute DUNITS (see Appendix A - Object Catalogue)
    unsigned char                 HUNI;                               // Units of height measurement HUNI I(2) b11 int Value taken from the attribute HUNITS (see Appendix A - Object Catalogue)
    unsigned char                 PUNI;                               // Units of positional accuracy PUNI I(2) b11 int Value taken from the attribute PUNITS (see Appendix A - Object Catalogue)
    unsigned char                 COUN;                               // Coordinate units COUN A(2) b11 an Unit of measurement for coordinates:
                                                                      //             "LL" {1} Latitude/Longitude
                                                                      //             "EN" {2} Easting/Northing
                                                                      //             "UC" {3} Units on the chart/map
    unsigned long                 COMF;                               // Coordinate multiplication factor COMF I( ) b14 int Floating-point to integer multiplication factor for coordinate values
    unsigned long                 SOMF;                               // 3-D (sounding) multiplication factor SOMF I( ) b14 int Floating point to integer multiplication factor for 3-D (sounding) values
    char                          COMT[1];                            // Comment COMT A( ) bt A string of characters
};

struct FRID
{
    unsigned char                 RCNM;                               // Record name RCNM A(2) b11 an "VI" {110} Isolated node
                                                                      //                              "VC" {120} Connected node
                                                                      //                              "VE" {130} Edge
                                                                      //                              "VF" {140} Face
    unsigned long                 RCID;                               // Record identification number RCID I(10 ) b14 int Range: 1 to 2^32-2
    unsigned char                 PRIM;                               // Object geometric primitive PRIM A(1) b11 an "P" {1} Point
                                                                      //                                             "L" {2} Line
                                                                      //                                             "A" {3} Area
                                                                      //                                             "N" {255} Object does not directly reference any spatial objects
    unsigned char                 GRUP;                               // Group  GRUP I(3) b11 int Range: 1 to 254, 255 - No group (binary)
    unsigned short                OBJL;                               // Object label/code OBJL I(5) b12 int A valid object label/code
    unsigned short                RVER;                               // Record version RVER I(3) b12 int RVER contains the serial number of the record edition
    unsigned char                 RUIN;                               // Record update instruction RUIN A(1) b11 an "I" {1} Insert
                                                                      //                                            "D" {2} Delete
                                                                      //                                            "M" {3} Modify
    unsigned char                 TerminalByte;                       // Терминальный байт. Должен быть равен 0x1E
};

struct VRID
{
    unsigned char                 RCNM;                               // Record name RCNM A(2) b11 an "VI" {110} Isolated node
                                                                      //                              "VC" {120} Connected node
                                                                      //                              "VE" {130} Edge
                                                                      //                              "VF" {140} Face
    unsigned long                 RCID;                               // Record identification number RCID I(10 ) b14 int Range: 1 to 2^32-2
    unsigned short                RVER;                               // Record version RVER I(3) b12 int RVER contains the serial number of the record edition
    unsigned char                 RUIN;                               // Record update instruction RUIN A(1) b11 an "I" {1} Insert
                                                                      //                                            "D" {2} Delete
                                                                      //                                            "M" {3} Modify
    unsigned char                 TerminalByte;                       // Терминальный байт. Должен быть равен 0x1E
};

struct FSPT
{
    unsigned char                 RCNM;                               // Record name RCNM A(2) b11
    unsigned long                 RCID;                               // Record identification number RCID I(10 ) b14 int Range: 1 to 2^32-2
    unsigned char                 ORNT;                               // Orientation ORNT A(1) b11 an "F" {1} Forward
                                                                      //                              "R" {2} Reverse
                                                                      //                              "N" {255} NULL
    unsigned char                 USAG;                               // Usage indicator USAG A(1) b11 an "E" {1} Exterior
                                                                      //                                  "I" {2} Interior
                                                                      //                                  "C" {3} Exterior boundary truncated by the data limit
                                                                      //                                  "N" {255} NULL
    unsigned char                 MASK;                               // Masking indicator MASK A(1) b11 an "M" {1} Mask
                                                                      //                                    "S" {2} Show
                                                                      //                                    "N" {255} NULL
};

struct VRPT
{
    unsigned char                 RCNM;                               // Record name RCNM A(2) b11
    unsigned long                 RCID;                               // Record identification number RCID I(10 ) b14 int Range: 1 to 2^32-2
    unsigned char                 ORNT;                               // Orientation ORNT A(1) b11 an "F" {1} Forward
                                                                      //                              "R" {2} Reverse
                                                                      //                              "N" {255} NULL
    unsigned char                 USAG;                               // Usage indicator USAG A(1) b11 an "E" {1} Exterior
                                                                      //                                  "I" {2} Interior
                                                                      //                                  "C" {3} Exterior boundary truncated by the data limit
                                                                      //                                  "N" {255} NULL
    unsigned char                 TOPI;                               // Topology indicator TOPI A(1) b11 an "B" {1} Beginning node
                                                                      //                                     "E" {2} End node
                                                                      //                                     "S" {3} Left face
                                                                      //                                     "D" {4} Right face
                                                                      //                                     "F" {5} Containing face
                                                                      //                                     "N" {255} NUL
    unsigned char                 MASK;                               // Masking indicator MASK A(1) b11 an "M" {1} Mask
                                                                      //                                    "S" {2} Show
                                                                      //                                    "N" {255} NULL
};

#pragma pack(pop)

#endif // ASTS57FORMAT_H
