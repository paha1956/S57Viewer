#ifndef ASTS57OBJECTCLASSES_H
#define ASTS57OBJECTCLASSES_H

enum S57_OBJECT_CLASSES
{
    ADMARE = 1,                   // Administration area (Named)
    AIRARE = 2,                   // Airport / airfield
    ACHBRT = 3,                   // Anchor berth
    ACHARE = 4,                   // Anchorage area
    BCNCAR = 5,                   // Beacon, cardinal
    BCNISD = 6,                   // Beacon, isolated danger
    BCNLAT = 7,                   // Beacon, lateral
    BCNSAW = 8,                   // Beacon, safe water
    BCNSPP = 9,                   // Beacon, special purpose/general
    BERTHS = 10,                  // Berth
    BRIDGE = 11,                  // Bridge
    BUISGL = 12,                  // Building, single
    BUAARE = 13,                  // Built-up area
    BOYCAR = 14,                  // Buoy, cardinal
    BOYINB = 15,                  // Buoy, installation
    BOYISD = 16,                  // Buoy, isolated danger
    BOYLAT = 17,                  // Buoy, lateral
    BOYSAW = 18,                  // Buoy, safe water
    BOYSPP = 19,                  // Buoy, special purpose/general
    CBLARE = 20,                  // Cable area
    CBLOHD = 21,                  // Cable, overhead
    CBLSUB = 22,                  // Cable, submarine
    CANALS = 23,                  // Canal
    CANBNK = 24,                  // Canal bank
    CTSARE = 25,                  // Cargo transshipment area
    CAUSWY = 26,                  // Causeway
    CTNARE = 27,                  // Caution area
    CHKPNT = 28,                  // Checkpoint
    CGUSTA = 29,                  // Coastguard station
    COALNE = 30,                  // Coastline
    CONZNE = 31,                  // Contiguous zone
    COSARE = 32,                  // Continental shelf area
    CTRPNT = 33,                  // Control point
    CONVYR = 34,                  // Conveyor
    CRANES = 35,                  // Crane
    CURENT = 36,                  // Current - non - gravitational
    CUSZNE = 37,                  // Custom zone
    DAMCON = 38,                  // Dam
    DAYMAR = 39,                  // Daymark
    DWRTCL = 40,                  // Deep water route centerline
    DWRTPT = 41,                  // Deep water route part
    DEPARE = 42,                  // Depth area
    DEPCNT = 43,                  // Depth contour
    DISMAR = 44,                  // Distance mark
    DOCARE = 45,                  // Dock area
    DRGARE = 46,                  // Dredged area
    DRYDOC = 47,                  // Dry dock
    DMPGRD = 48,                  // Dumping ground
    DYKCON = 49,                  // Dyke
    EXEZNE = 50,                  // Exclusive Economic Zone
    FAIRWY = 51,                  // Fairway
    FNCLNE = 52,                  // Fence/wall
    FERYRT = 53,                  // Ferry route
    FSHZNE = 54,                  // Fishery zone
    FSHFAC = 55,                  // Fishing facility
    FSHGRD = 56,                  // Fishing ground
    FLODOC = 57,                  // Floating dock
    FOGSIG = 58,                  // Fog signal
    FORSTC = 59,                  // Fortified structure
    FRPARE = 60,                  // Free port area
    GATCON = 61,                  // Gate
    GRIDRN = 62,                  // Gridiron
    HRBARE = 63,                  // Harbour area (administrative)
    HRBFAC = 64,                  // Harbour facility
    HULKES = 65,                  // Hulk
    ICEARE = 66,                  // Ice area
    ICNARE = 67,                  // Incineration area
    ISTZNE = 68,                  // Inshore traffic zone
    LAKARE = 69,                  // Lake
    LAKSHR = 70,                  // Lake shore
    LNDARE = 71,                  // Land area
    LNDELV = 72,                  // Land elevation
    LNDRGN = 73,                  // Land region
    LNDMRK = 74,                  // Landmark
    LIGHTS = 75,                  // Light
    LITFLT = 76,                  // Light float
    LITVES = 77,                  // Light vessel
    LOCMAG = 78,                  // Local magnetic anomaly
    LOKBSN = 79,                  // Lock basin
    LOGPON = 80,                  // Log pond
    MAGVAR = 81,                  // Magnetic variation
    MARCUL = 82,                  // Marine farm/culture
    MIPARE = 83,                  // Military practice area
    MORFAC = 84,                  // Mooring/warping facility
    NAVLNE = 85,                  // Navigation line
    OBSTRN = 86,                  // Obstruction
    OFSPLF = 87,                  // Offshore platform
    OSPARE = 88,                  // Offshore production area
    OILBAR = 89,                  // Oil barrier
    PILPNT = 90,                  // Pile
    PILBOP = 91,                  // Pilot boarding place
    PIPARE = 92,                  // Pipeline area
    PIPOHD = 93,                  // Pipeline, overhead
    PIPSOL = 94,                  // Pipeline, submarine/on land
    PONTON = 95,                  // Pontoon
    PRCARE = 96,                  // Precautionary area
    PRDARE = 97,                  // Production / storage area
    PYLONS = 98,                  // Pylon/bridge support
    RADLNE = 99,                  // Radar line
    RADRNG = 100,                 // Radar range
    RADRFL = 101,                 // Radar reflector
    RADSTA = 102,                 // Radar station
    RTPBCN = 103,                 // Radar transponder beacon
    RDOCAL = 104,                 // Radio calling-in point
    RDOSTA = 105,                 // Radio station
    RAILWY = 106,                 // Railway
    RAPIDS = 107,                 // Rapids
    RCRTCL = 108,                 // Recommended route centerline
    RECTRC = 109,                 // Recommended track
    RCTLPT = 110,                 // Recommended Traffic Lane Part
    RSCSTA = 111,                 // Rescue station
    RESARE = 112,                 // Restricted area
    RETRFL = 113,                 // Retro-reflector
    RIVERS = 114,                 // River
    RIVBNK = 115,                 // River bank
    ROADWY = 116,                 // Road
    RUNWAY = 117,                 // Runway
    SNDWAV = 118,                 // Sand waves
    SEAARE = 119,                 // Sea area / named water area
    SPLARE = 120,                 // Sea-plane landing area
    SBDARE = 121,                 // Seabed area
    SLCONS = 122,                 // Shoreline Construction
    SISTAT = 123,                 // Signal station, traffic
    SISTAW = 124,                 // Signal station, warning
    SILTNK = 125,                 // Silo / tank
    SLOTOP = 126,                 // Slope topline
    SLOGRD = 127,                 // Sloping ground
    SMCFAC = 128,                 // Small craft facility
    SOUNDG = 129,                 // Sounding
    SPRING = 130,                 // Spring
    SQUARE = 131,                 // Square
    STSLNE = 132,                 // Straight territorial sea baseline
    SUBTLN = 133,                 // Submarine transit lane
    SWPARE = 134,                 // Swept Area
    TESARE = 135,                 // Territorial sea area
    TS_PRH = 136,                 // Tidal stream - harmonic prediction
    TS_PNH = 137,                 // Tidal stream - non-harmonic prediction
    TS_PAD = 138,                 // Tidal stream panel data
    TS_TIS = 139,                 // Tidal stream - time series
    T_HMON = 140,                 // Tide - harmonic prediction
    T_NHMN = 141,                 // Tide - non-harmonic prediction
    T_TIMS = 142,                 // Tidal stream - time series
    TIDEWY = 143,                 // Tideway
    TOPMAR = 144,                 // Top mark
    TSELNE = 145,                 // Traffic Separation Line
    TSSBND = 146,                 // Traffic Separation Scheme  Boundary
    TSSCRS = 147,                 // Traffic Separation Scheme Crossing
    TSSLPT = 148,                 // Traffic Separation Scheme  Lane part
    TSSRON = 149,                 // Traffic Separation Scheme  Roundabout
    TSEZNE = 150,                 // Traffic Separation Zone
    TUNNEL = 151,                 // Tunnel
    TWRTPT = 152,                 // Two-way route  part
    UWTROC = 153,                 // Underwater rock / awash rock
    UNSARE = 154,                 // Unsurveyed area
    VEGATN = 155,                 // Vegetation
    WATTUR = 156,                 // Water turbulence
    WATFAL = 157,                 // Waterfall
    WEDKLP = 158,                 // Weed/Kelp
    WRECKS = 159,                 // Wreck
    TS_FEB = 160,                 // Tidal stream - flood/ebb
    M_ACCY = 300,                 // Accuracy of data
    M_CSCL = 301,                 // Compilation scale of data
    M_COVR = 302,                 // Coverage
    M_HDAT = 303,                 // Horizontal datum of data
    M_HOPA = 304,                 // Horizontal datum shift parameters
    M_NPUB = 305,                 // Nautical publication information
    M_NSYS = 306,                 // Navigational system of marks
    M_PROD = 307,                 // Production information
    M_QUAL = 308,                 // Quality of data
    M_SDAT = 309,                 // Sounding datum
    M_SREL = 310,                 // Survey reliability
    M_UNIT = 311,                 // Units of measurement of data
    M_VDAT = 312,                 // Vertical datum of data
    C_AGGR = 400,                 // Aggregation
    C_ASSO = 401,                 // Association
    C_STAC = 402,                 // Stacked on/stacked under
    sAREAS = 500,                 // Cartographic area
    sLINES = 501,                 // Cartographic line
    sCSYMB = 502,                 // Cartographic symbol
    sCOMPS = 503,                 // Compass
    sTEXTS = 504,                 // Text

    //........
    DMYOBJ = 65000                // Dummy object
};

#endif // ASTS57OBJECTCLASSES_H
