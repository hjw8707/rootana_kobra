#ifndef TKoBRADIGData_hxx_seen
#define TKoBRADIGData_hxx_seen

#include <vector>

#include "TGenericData.hxx"

/// Class for each DIG measurement
/// For the definition of obscure variables see the CAEN V1190 manual.
class KoBRADIGMeasurement {

  friend class TKoBRADIGData;

public:
  /// Get the channel number
  uint32_t GetChannel() const {return channel;}
/// Get the DIG energy
  uint32_t GetTime() const {return time;}    
  uint32_t GetTimeF() const {return timef;}   
  uint32_t GetTimeX() const {return timex;}       
  /// Get the DIG energy
  uint32_t GetEnergy() const {return energy;}
  /// Get the DIG energy
  uint32_t GetEnergyShort() const {return energys;}  


private:
  /// Found fields to hold the header, measurement, trailer and error words.
  uint32_t channel;
  uint32_t time;
  uint32_t timef;
  uint32_t timex;
  uint32_t energy;
  uint32_t energys;

  /// Constructor; need to pass in header and measurement.
  KoBRADIGMeasurement(uint32_t _ch, uint32_t _t, uint32_t _tf, uint32_t _tx, uint32_t _e, uint32_t _es):
    channel(_ch),
    time(_t), timef(_tf), timex(_tx),
    energy(_e), energys(_es) {};

  KoBRADIGMeasurement();    
};


/// Class to store data from CAEN V1190.
/// We store the information as a vector of DIGMeasurement's
/// Question: do we need a way of retrieving information about DIGs that
/// have no measurements?  Currently this information is not exposed.
/// For the definition of obscure variables see the CAEN V1190 manual.
class TKoBRADIGData: public TGenericData {

public:

  /// Constructor
  TKoBRADIGData(int bklen, int bktype, const char* name, void *pdata);

  void Print();

  /// Get the Vector of DIG Measurements.
  std::vector<KoBRADIGMeasurement>& GetMeasurements() {return fMeasurements;}


private:
  
  // We have vectors of the headers/trailers/etc, since there can be 
  // multiple events in a bank.

  /// Vector of DIG Measurements.
  std::vector<KoBRADIGMeasurement> fMeasurements;

};

#endif
