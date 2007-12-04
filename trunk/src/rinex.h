/**
\file    rinex.h
\brief   GNSS core 'c' function library: RINEX related functions.
\author  Glenn D. MacGougan (GDM)
\date    2007-12-02
\since   2007-12-02

\b REFERENCES \n
- 

\b "LICENSE INFORMATION" \n
Copyright (c) 2007, refer to 'author' doxygen tags \n
All rights reserved. \n

Redistribution and use in source and binary forms, with or without
modification, are permitted provided the following conditions are met: \n

- Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer. \n
- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution. \n
- The name(s) of the contributor(s) may not be used to endorse or promote 
  products derived from this software without specific prior written 
  permission. \n

THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS 
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
SUCH DAMAGE.
*/

#ifndef _RINEX_H_
#define _RINEX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "basictypes.h"
#include "novatel.h"


/**
\brief  A container for RINEX observation file header data. Each parameter is the string (multiline is some cases) of columns 1-60 that corresponds to a record descriptor from columns 61-80.
*/
typedef struct
{
  char             version_type[64];   //!< RINEX VERSION / TYPE - File type: O for Observation Data - Satellite System: G: GPS, R: GLONASS, E: Galileo, S: SBAS payload, M: Mixed.
  char          pgm_run_by_date[64];   //!< Name of program creating current file - Name of agency creating current file - Date and time of file creation, Format: yyyymmdd hhmmss zone, zone: 3-4 char. code for time zone. UTC recommended! LCL if local time with unknown local time system code.
  char                 comments[1024]; //!< (Optional-multiline) Comment line(s). Enough space for 16 lines of comments.
  char              marker_name[64];   //!< Name of antenna marker.
  char                marker_nr[64];   //!< (Optional) Number of antenna marker . 
  char              marker_type[64];   //!< Type of the marker: GEODETIC : Earth-fixed, high-precision monumentation, NON_GEODETIC : Earth-fixed, low-precision monumentation, NON_PHYSICAL : Generated from network processing, SPACEBORNE : Orbiting space vehicle, AIRBORNE : Aircraft, balloon, etc. WATER_CRAFT : Mobile water craft, GROUND_CRAFT : Mobile terrestrial vehicle, FIXED_BUOY : "Fixed" on water surface, FLOATING_BUOY: Floating on water surface, FLOATING_ICE : Floating ice sheet, etc., GLACIER : "Fixed" on a glacier, BALLISTIC : Rockets, shells, etc, ANIMAL : Animal carrying a receiver, HUMAN : Human being, Record required except for GEODETIC and NON_GEODETIC marker types. Users may define other project-dependent keywords.
  char          observer_agency[64];   //!< Name of observer / agency.
  char receiver_nr_type_version[64];   //!< Receiver number, type, and version (Version: e.g. Internal Software Version).
  char          antenna_nr_type[64];   //!< Antenna number and type.
  char      approx_position_xyz[64];   //!< Geocentric approximate marker position (Units: Meters, System: ITRS recommended) Optional for moving platforms.
  char      antenna_delta_h_e_n[64];   //!< Antenna height: Height of the antenna reference point (ARP) above the marker - Horizontal eccentricity of ARP relative to the marker (east/north). All units in meters.
  char        antenna_delta_xyz[64];   //!< (Optional) Position of antenna reference point for antenna on vehicle (m). XYZ vector in body-fixed coord. system.
  char      antenna_phasecenter[64];   //!< (Optional) Average phase center position w/r to antenna reference point (m) - satellite system (g/r/e/s), - observation code - north/east/up (fixed station) or x/y/z in body-fixed system (vehicle).
  char      antenna_b_sight_xyz[64];   //!< (Optional) Direction of the �vertical� antenna axis towards the gnss satellites. Antenna on vehicle: unit vector in body-fixed coord. system tilted antenna on fixed station: unit vector in n/e/up left-handed system.
  char      antenna_zerodir_azi[64];   //!< (Optional) Azimuth of the zero-direction of a fixed antenna (degrees, from north).
  char      antenna_zerodir_xyz[64];   //!< (Optional) Zero-direction of antenna antenna on vehicle: Unit vector in body-fixed coord. system tilted antenna on fixed station: Unit vector in n/e/up left-handed system.
  char       center_of_mass_xyz[64];   //!< (Optional) Current center of mass (x,y,z, meters) of vehicle in body-fixed coordinate system. Same system as used for attitude.
  char    system_nrobs_obstypes[64];   //!< Satellite system - Number of observation types - List of observation types. Varies with RINEX version.
  char     signal_strength_unit[64];   //!< (Optional) The Unit of the signal strengt observables Snn (if present)  DBHZ : s/n given in dB-Hz.
  char                 interval[64];   //!< (Optional) Observation interval in seconds.
  char        time_of_first_obs[64];   //!< The time of first observation record (4-digit-year, month,day,hour,min,sec)- Time system: GPS=GPS time system, GLO=UTC time system, GAL=Galileo System Time. Compulsory in mixed GPS/GLONASS files, Defaults: GPS for pure GPS files, GLO for pure GLONASS files, GAL for pure Galileo files.
  char         time_of_last_obs[64];   //!< (Optional) The time of last observation record (4-digit-year, month,day,hour,min,sec)- time system: same value as in time_of_first_obs record.
  char      rcv_clock_offs_appl[64];   //!< (Optional) Epoch, code, and phase are corrected by applying the realtime-derived receiver clock offset: 1=yes, 0=no; default: 0=no. Record required if clock offsets are reported in the EPOCH/SAT records.
  char         sys_dcbs_applied[64];   //!< (Optional) Satellite system (G/R/E/S) - Program name used to apply differential code bias corrections - Source of corrections (URL). Repeat for each satellite system. No corrections applied: Blank fields or record not present. 
  char         sys_pcvs_applied[64];   //!< (Optional) Satellite system (G/R/E/S) - Program name used to apply phase center variation corrections - Source of corrections (URL). Repeat for each satellite system. No corrections applied: Blank fields or record not present.
  char         sys_scale_factor[512];  //!< (Optional-multiline) Satellite system (G/R/E/S) - Factor to divide stored observations with before use (1,10,100,1000) - Number of observation types involved. 0 or blank: All observation types - List of observation types. Use continuation line(s) for more than 12 observation types. Repeat record if different factors are applied to different observation types. A value of 1 is assumed if record is missing.
  char             leap_seconds[64];   //!< (Optional) Number of leap seconds since 6-Jan-1980. Recommended for mixed GPS/GLONASS files.
  char            nr_satellites[64];   //!< (Optional) Number of satellites, for which observations are stored in the file.
  char            prn_nr_of_obs[1024]; //!< (Optional-multiline) Satellite numbers, number of observations for each observation type indicated in the SYS / # / OBS TYPES record. If more than 9 observation types: Use continuation line(s). This record is (these records are) repeated for each satellite present in the data file.  
  
} RINEX_structRawHeader;



/// An enumeration for RINEX file types.
typedef enum
{
  RINEX_FILE_TYPE_OBS       = 'O', //!< 'O' - Observation file.
  RINEX_FILE_TYPE_GPS_NAV   = 'N', //!< 'N' - GPS navigation file.
  RINEX_FILE_TYPE_MET       = 'M', //!< 'M' - Meteorological data file.
  RINEX_FILE_TYPE_GLO_NAV   = 'G', //!< 'G' - GLONASS navigation file.
  RINEX_FILE_TYPE_GEO_NAV   = 'H', //!< 'H' - Geostationary Navigation nile.
  RINEX_FILE_TYPE_GAL_NAV   = 'L', //!< 'L' - Galileo navigation message file.
  RINEX_FILE_TYPE_MIXED_NAV = 'P', //!< 'P' - Mixed GNSS navigation message file.
  RINEX_FILE_TYPE_SBAS      = 'B', //!< 'B' - SBAS broadcast data file (separate documentation).
  RINEX_FILE_TYPE_CLK       = 'C', //!< 'C' - Clock file (separate documentation).
  RINEX_FILE_TYPE_SUMMARY   = 'S', //!< 'S' - Summary file (used e.g., by IGS, not a standard!).
  RINEX_FILE_TYPE_UNKNOWN

} RINEX_enumFileType;

typedef enum
{
  RINEX_OBS_TYPE_L1,
  RINEX_OBS_TYPE_L2,
  RINEX_OBS_TYPE_C1,
  RINEX_OBS_TYPE_P1,
  RINEX_OBS_TYPE_P2,
  RINEX_OBS_TYPE_D1,
  RINEX_OBS_TYPE_D2,
  RINEX_OBS_TYPE_T1,
  RINEX_OBS_TYPE_T2,
  RINEX_OBS_TYPE_S1,
  RINEX_OBS_TYPE_S2,
  RINEX_OBS_TYPE_UNKNOWN

} RINEX_enumObservationType;

typedef enum
{
  RINEX_WAVELENTH_FACTOR_SINGLE_FREQ = 0,
  RINEX_WAVELENTH_FACTOR_FULL_AMB    = 1,
  RINEX_WAVELENTH_FACTOR_HALF_CYCLE  = 2,
  RINEX_WAVELENTH_FACTOR_UNKOWN
  
} RINEX_enumWavelenthFactorL1_L2;

typedef enum
{
  RINEX_TIME_SYSTEM_GPS = 0, //!< GPS time system.
  RINEX_TIME_SYSTEM_GLO = 1, //!< GLONASS time system.
  RINEX_TIME_SYSTEM_UNKNOWN

} RINEX_enumTimeSystem;
  


typedef struct
{
  unsigned short year;
  unsigned char month;
  unsigned char day;
  unsigned char hour;
  unsigned char minute;
  float seconds;
  RINEX_enumTimeSystem time_system;

} RINEX_TIME;


typedef enum
{
  RINEX_EPOCH_FLAG_OK             = 0, //!< OK
  RINEX_EPOCH_FLAG_PWR_FAIL       = 1, //!< power failure between previous and current epoch.
  RINEX_EPOCH_FLAG_MOVING_ANT     = 2, //!< start moving antenna.
  RINEX_EPOCH_FLAG_NEW_SITE       = 3, //!< new site occupation (end of kinem. data) (at least MARKER NAME record follows)
  RINEX_EPOCH_FLAG_HEADER_FOLLOWS = 4, //!< header information follows
  RINEX_EPOCH_FLAG_EXTERNAL_EVENT = 5, //!< external event (epoch is significant, same time frame as observation time tags).
  RINEX_EPOCH_FLAG_CYCLE_SLIP     = 6, //!< cycle slip records follow to optionally report detected and repaired cycle slips (same format as OBSERVATIONS records; slip instead of observation; LLI and signal strength blank or zero)
  RINEX_EPOCH_FLAG_EVENT         

} RINEX_enumEpochFlag;


typedef struct
{
  double              version;          //!< RINEX version.
  RINEX_enumFileType  type;             //!< RINEX file type.
  char                marker_name[128]; //!< The site marker name.
  double              x;                //!< Geocentric approximate marker position (Units: Meters, System: ITRS recommended) Optional for moving platforms.
  double              y;                //!< Geocentric approximate marker position (Units: Meters, System: ITRS recommended) Optional for moving platforms.
  double              z;                //!< Geocentric approximate marker position (Units: Meters, System: ITRS recommended) Optional for moving platforms.
  double              antenna_delta_h;  //!< Antenna height: Height of the antenna reference point (ARP) above the marker 
  double              antenna_ecc_e;    //!< Horizontal eccentricity of ARP relative to the marker (east). All units in meters.
  double              antenna_ecc_n;    //!< Horizontal eccentricity of ARP relative to the marker (north). All units in meters.
  unsigned            nr_obs_types;     //!< The number of valid observation types.
  RINEX_enumObservationType  obs_types[12]; //!< The observation types.
  
  RINEX_enumWavelenthFactorL1_L2 default_wavefactor_L1; //!< The default wavelenth factor for L1.
  RINEX_enumWavelenthFactorL1_L2 default_wavefactor_L2; //!< The default wavelenth factor for L2.

  char wavelength_factors[1024]; //!< A string containing any satellite specific wavelength factors.

  RINEX_TIME time_of_first_obs;

} RINEX_structDecodedHeader;





/**
\brief  Get the RINEX header, as a buffer, from the specified file path. Determine the RINEX version and file type.

\author Glenn D. MacGougan
\date   2007-12-03
\since  2007-12-03

remarks
- The "RINEX VERSION / TYPE" record must be the first record in a file.

\return  TRUE(1) if successful, FALSE(0) otherwise.
*/
BOOL RINEX_GetHeader( 
  const char* filepath,           //!< Path to the RINEX file.
  char* buffer,                   //!< (input/output) A character buffer in which to place the RINEX header.
  const unsigned buffer_max_size, //!< (input)  The maximum size of the buffer [bytes]. This value should be large enough to hold the entire header, (8192 to 16384).
  unsigned *buffer_size,          //!< (output) The length of the header data placed in the buffer [bytes].
  double *version,                //!< (output) The RINEX version number. e.g. 1.0, 2.0, 2.2, 3.0, etc.
  RINEX_enumFileType *file_type   //!< (output) The RINEX file type. 
  );



/**
\brief  Decode the parts of the RINEX Observation file header that are essential to

\author Glenn D. MacGougan
\date   2007-12-03
\since  2007-12-03

\return  TRUE(1) if successful, FALSE(0) otherwise.
*/
BOOL RINEX_DecodeHeader_ObservationFile(
  const char* header_buffer,         //!< (input) The character buffer containing the RINEX header.
  const unsigned header_buffer_size, //!< (input) The size of the character buffer containing the RINEX header [bytes]. Not the maximum size, the size of the valid data in the buffer.
  RINEX_structDecodedHeader* header  //!< (output) The decoded header data.
  );



BOOL RINEX_GetNextObservationSet(
  FILE* fid,                               //!< (input) An open (not NULL) file pointer to the RINEX data.
  RINEX_structDecodedHeader* RINEX_header, //!< (input/output) The decoded RINEX header information. The wavelength markers can change as data is decoded.
  BOOL *wasEndOfFileReached,               //!< Has the end of the file been reached (output).
  BOOL *wasObservationFound,               //!< Was a valid observation found (output).
  unsigned *filePosition,                  //!< The file position for the start of the message found (output).  
  NOVATELOEM4_structBinaryHeader* header,  //!< A pointer to a NovAtel OEM4 header information struct (output).
  NOVATELOEM4_structObservation* obsArray, //!< A pointer to a user provided array of struct_NOVATELOEM4_RANGE (output).
  const unsigned char maxNrObs,            //!< The maximum number of elements in the array provided (input).
  unsigned *nrObs                          //!< The number of valid elements set in the array (output).
  );





#endif // _RINEX_H_

