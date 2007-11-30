/**
\file    GNSS_RxData.h
\brief   The header file for the GNSS_RxData class.

\author  Glenn D. MacGougan (GDM)
\date    2007-11-29
\since   2006-11-13

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

#ifndef _GNSS_RXDATA_H_
#define _GNSS_RXDATA_H_

#include <stdio.h>
#include "GNSS_Types.h"
#include "gps.h"


/// This is the fixed number of channels contained in the array 
/// for one GNSS_RxData object.
#define GNSS_RXDATA_NR_CHANNELS (48)

/// This is the buffer length (in bytes) for a data message buffer used
/// by the receiver object in decoding data.
#define GNSS_RXDATA_MSG_LENGTH  (16384)


namespace GNSS
{
  //============================================================================
  /// \class   GPS_BroadcastEphemerisAndAlmanacArray
  /// \brief   An array class for storing broadcast GPS ephemeris and almanac 
  ///          information.
  ///
  /// Each satellite is uniquely identified in GPS by it's PRN. Each PRN has
  /// an associated almanac structure, a most recent ephemeris structure,
  /// and a previous ephemeris structure (to faciliate ephemeris matching)
  /// 
  /// GPS         are  1-32    \n
  /// Pseudolites are  33-37   \n
  /// SBAS        are  120-138 \n
  /// WAAS, EGNOS, MSAS        \n
  ///
  /// WAAS: \n
  /// AOR-W       122 \n
  /// Anik        138 \n
  /// POR         134 \n
  /// PanAm       135 \n
  ///
  /// EGNOS: \n
  /// AOR-E       120 \n
  /// Artemis     124 \n
  /// IOR-W       126 \n
  /// IOR-E       131 \n
  ///
  /// MSAS: \n
  /// MTSAT-1     129 \n
  /// MTSAT-2     137 \n
  ///
  /// The index mapping is as follows:                      \n
  /// PRN 1-37    maps to indidex 0-36                      \n
  /// PRN 38-40   maps to indices 37-39 (reserved mappings) \n
  /// PRN 120-138 maps to indicex 40-58                     \n
  ///
  /// \author  Glenn D. MacGougan (GDM)
  /// \date    2007-10-29
  /// \since   2006-11-15
  ///
  class GPS_BroadcastEphemerisAndAlmanacArray
  {
  public:

    /// \brief    The default constructor (no data allocated yet).
    GPS_BroadcastEphemerisAndAlmanacArray();

    /// \brief    The destructor.
    virtual ~GPS_BroadcastEphemerisAndAlmanacArray();

    /// \brief    Add an ephemeris structure.
    /// \return   true if successful, false if error.
    bool AddEphemeris( const unsigned short prn, const GPS_structEphemeris &eph );

    /// \brief    Add an almanac structure.
    /// \return   true if successful, false if error.
    bool AddAlmanac( const unsigned short prn, const GPS_structAlmanac &alm );


    /// \brief    Check if ephemeris information is available for a PRN.
    /// \return   true if successful, false if error.    
    bool IsEphemerisAvailable( 
      const unsigned short prn, //!< The desired GPS PRN. (1-32 GPS, 120-138 SBAS).
      bool &isAvailable,        //!< This boolean indicates if ephemeris data is available or not.
      char iode = -1            //!< The issue of data for the ephemeris, -1 means get the most current.
      );

    /// \brief    Try to get the most current ephemeris or the ephemeris 
    ///           with the issue of data (ephemeris), iode, specified. 
    /// \remarks  (1) iode == -1, means retrieve the most current ephemeris. \n
    ///           (2) 
    /// \return   true if successful, false if error.
    bool GetEphemeris( 
      const unsigned short prn, //!< The desired GPS PRN. (1-32 GPS, 120-138 SBAS).
      GPS_structEphemeris &eph, //!< A reference to an ephemeris struct in which to store the data.
      bool &isAvailable,        //!< This boolean indicates if ephemeris data is available or not.
      char iode = -1            //!< The issue of data for the ephemeris, -1 means get the most current.
      );

  private:
    /// \brief   The copy constructor. Disabled!
    GPS_BroadcastEphemerisAndAlmanacArray( const GPS_BroadcastEphemerisAndAlmanacArray& rhs );

    /// \brief   The assignment operator. Disabled!
    void operator=(const GPS_BroadcastEphemerisAndAlmanacArray& rhs)
    {}

  protected:

    /// \brief   Allocate the array.
    /// \return  true if successful, false if error.
    bool AllocateArray();

    /// \brief   Get the index of the PRN in the array.
    /// \return  true if successful, false if error (e.g. unsupported PRN).
    bool GetIndexGivenPRN( const unsigned short prn, unsigned short &index );

  protected:

    struct GPS_structOrbitParameters
    {
      unsigned short prn;
      GPS_structEphemeris currentEph;
      GPS_structEphemeris previousEph;
      GPS_structAlmanac   almanac;
    };

    /// A pointer to the array of GPS satellite orbit information structs.
    GPS_structOrbitParameters* m_array; 

    /// The maximum number of elements in m_array.
    unsigned m_arrayLength;
  };


  //============================================================================
  /// \class   GNSS_RxData
  /// \brief   A class for handling GNSS information for ONE EPOCH for ONE
  ///          RECEIVER such as pseudorange, ADR, and Doppler measurements, 
  ///          user position, user velocity, and other associated information.
  ///          The previous epoch of data is also retained if available.
  /// 
  /// The 'measurement' data is public and can be accessed directly by the user! 
  /// This is struct style encapsulation and to allow easy access to the data but
  /// the user must be careful (especially with the observation array in to 
  /// avoiding out of bound array access).
  ///
  /// \author  Glenn D. MacGougan (GDM)
  /// \date    2007-10-29  
  /// \since   2006-11-13
  /// 
  class GNSS_RxData
  { 
  public: // types

    /// \brief    These are the supported receiver data types.
    enum GNSS_enumRxDataType
    {
      GNSS_RXDATA_NOVATELOEM4 = 0,
      GNSS_RXDATA_UNKNOWN
    };
    
  public: 

    /// \brief    The default constructor (no data allocated yet).
    GNSS_RxData();                                             

    /// \brief    The destructor.
    virtual ~GNSS_RxData();

  private:

    /// \brief   The copy constructor. Disabled!
    GNSS_RxData( const GNSS_RxData& rhs );

    /// \brief   The assignment operator. Disabled!
    GNSS_RxData& operator=(const GNSS_RxData& rhs)
    { return *this; }

  public: 

    /// \brief   Set all data in the measurement array to zero.
    /// \return  true if successful, false if error.    
    bool ZeroAllMeasurements();


    /// \brief   Set all PVT data to zero.
    /// \return  true if successful, false if error.        
    bool ZeroPVT();


    /// \brief   Set the initial receiver position, velocity, and time.
    /// \return  true if successful, false if error.        
    bool SetInitialPVT( 
      const double latitudeRads,   //!< The latitude [rad].
      const double longitudeRads,  //!< The longitude [rad].
      const double height,         //!< The orthometric height [m].
      const double vn,             //!< The northing velocity [m/s].
      const double ve,             //!< The easting velocity [m/s].
      const double vup,            //!< The up velocity [m/s].
      const double clk,            //!< The clock offset [m].
      const double clkdrift,       //!< The clock drift [m/s].
      const double std_lat,        //!< The standard deviation uncertainty in the latitude [m].
      const double std_lon,        //!< The standard deviation uncertainty in the longitude [m].
      const double std_hgt,        //!< The standard deviation uncertainty in the height [m].
      const double std_vn,         //!< The standard deviation uncertainty in the northing velocity [m/s].
      const double std_ve,         //!< The standard deviation uncertainty in the easting velocity [m/s].
      const double std_vup,        //!< The standard deviation uncertainty in the up velocity [m/s].
      const double std_clk,        //!< The standard deviation uncertainty in the clock offset [m].
      const double std_clkdrift,   //!< The standard deviation uncertainty in the clock drift [m/s].
      const double undulation = 0  //!< The undulation if known [m].
      );

    /// \brief   Update the receiver position and clock offset.
    /// \return  true if successful, false if error.            
    bool UpdatePositionAndRxClock( 
      const double latitudeRads,   //!< The latitude [rad].
      const double longitudeRads,  //!< The longitude [rad].
      const double height,         //!< The orthometric height [m].
      const double clk,            //!< The clock offset [m].
      const double std_lat,        //!< The standard deviation uncertainty in the latitude [m].
      const double std_lon,        //!< The standard deviation uncertainty in the longitude [m].
      const double std_hgt,        //!< The standard deviation uncertainty in the height [m].
      const double std_clk         //!< The standard deviation uncertainty in the clock offset [m].
      );

    /// \brief   Update the receiver velocity and clock drift.
    /// \return  true if successful, false if error.                
    bool UpdateVelocityAndClockDrift( 
      const double vn,             //!< The northing velocity [m/s].
      const double ve,             //!< The easting velocity [m/s].
      const double vup,            //!< The up velocity [m/s].
      const double clkdrift,       //!< The clock drift [m/s].
      const double std_vn,         //!< The standard deviation uncertainty in the northing velocity [m/s].
      const double std_ve,         //!< The standard deviation uncertainty in the easting velocity [m/s].
      const double std_vup,        //!< The standard deviation uncertainty in the up velocity [m/s].
      const double std_clkdrift    //!< The standard deviation uncertainty in the clock drift [m/s].
      );


  
    /// \brief  Get degrees, minutes, seconds from a decimal degrees angle.
    /// \return  true if successful, false if error.        
    static bool GetDMS( 
      const double angleDegs,  //!< The angle [degrees].
      short &degrees,          //!< The degrees part.
      short &minutes,          //!< The minutes part.
      float &seconds,          //!< The seconds part.
      char *dms_str,           //!< A DMS string e.g. dms_str = "-180'59'59.9999\""
      const unsigned maxLength_dms_str //!< The maximum length of the dms_str string.
      );
  



    /// \brief   Initialize the source of measurements 
    /// \return  true if successful, false if error.
    bool Initialize( const char* path, bool &isValidPath, const GNSS_enumRxDataType rxType );


    /// \brief   Load the next epoch of data.
    /// \return  true if successful, false if error.
    /// \param   endOfStream - indicates if the end of the input source 
    ///          was reached and no further data is available.
    bool LoadNext( bool &endOfStream );


    /// \brief  Check for cycle slips using the phase rate prediction method.
    ///
    /// \post   m_ObsArray[i].flags.isNoCycleSlipDetected is set for each observation.
    ///
    /// \return true if successful, false if error.
    bool CheckForCycleSlips_UsingPhaseRatePrediction( 
      const double nrThresholdCycles //!< The maximum number of cycles to use as the threshold to detect a slip [cycles].
      );


    /// \brief  A debugging function for printing all of the observation array
    /// to a file.
    ///
    /// \return true if successful, false if error.
    bool DebugPrintObservationArray( const char *filepath );


    bool Debug_WriteSuperMsg80CharsWide(
      char* buffer,                    //!< A large character buffer (8KB min).
      const unsigned maxBufferLength,  //!< The maximum buffer length [bytes].
      const double referenceLatitude,  //!< Reference position latitude [rad].
      const double referenceLongitude, //!< Reference position longitude [rad].
      const double referenceHeight,    //!< Reference position height [m].
      unsigned& nrBytesInBuffer );      //!< The number of bytes set in the buffer.  
  
  public:

    /// The array of GNSS measurements.
    GNSS_structMeasurement m_ObsArray[GNSS_RXDATA_NR_CHANNELS];
    
    /// The number of usable items in m_ObsArray.
    unsigned char m_nrValidObs;


    /// The previous observation set.
    GNSS_structMeasurement m_prev_ObsArray[GNSS_RXDATA_NR_CHANNELS];

    /// The number of usable items in m_prevObsArray.
    unsigned char m_prev_nrValidObs;

    
    /// The number of GPS L1 observations in m_ObsArray.
    unsigned char m_nrGPSL1Obs;

    /// The receiver's position, velocity, and time information.
    GNSS_structPVT  m_pvt;

    /// The receiver's previous position, velocity, and time information.
    GNSS_structPVT  m_prev_pvt;

    /// The klobuchar ionospheric correction parameters for GPS.
    GNSS_structKlobuchar m_klobuchar;

    /// This is an object for storing and handling GPS ephemeris and almanac
    /// information for valid GPS system satellites.
    GPS_BroadcastEphemerisAndAlmanacArray  m_EphAlmArray;

    /// This is the elevation mask angle [rads]. 
    /// The default is 5 degrees.
    double m_elevationMask; 

    /// This is the carrier to noise density ratio mask value [dB-Hz].
    /// The default is 28.0 dB-Hz.
    double m_cnoMask;

    /// This is the minimum allowable locktime [s].
    /// The default is 0.0.
    double m_locktimeMask;

    /// The maximum usable age for an ephemeris [s].
    /// The default is 4 hours (3600*4).
    unsigned m_maxAgeEphemeris; 


    /// A boolean to indicate if the tropospheric correction is to be disabled for all satellites.
    bool m_DisableTropoCorrection;

    /// A boolean to indicate if the ionospheric correction is to be disabled for all satellites.
    bool m_DisableIonoCorrection;

  protected:

    /// A file pointer to the input.
    FILE* m_fid;

    /// A large message buffer.
    unsigned char m_message[GNSS_RXDATA_MSG_LENGTH];

    /// The length of the message in the message buffer.
    unsigned short m_messageLength;

    /// The receiver data type.
    GNSS_enumRxDataType m_rxDataType;
    
  };

} // end namespace GNSS

#endif // _GNSS_RXDATA_H_

