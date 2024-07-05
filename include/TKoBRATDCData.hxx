#ifndef TKoBRATDCData_hxx_seen
#define TKoBRATDCData_hxx_seen

#include <vector>

#include "TGenericData.hxx"

/// Class for each TDC measurement
/// For the definition of obscure variables see the CAEN V1190 manual.
class KoBRATDCMeasurement {

  friend class TKoBRATDCData;

public:

  /// Get the TDC measurement
  uint32_t GetMeasurement() const {return tdc_measurement_word;}

  /// Get the channel number
  uint32_t GetChannel() const {return tdc_channel_word;}


private:

  /// Found fields to hold the header, measurement, trailer and error words.
  uint32_t tdc_channel_word;
  uint32_t tdc_measurement_word;

  /// Constructor; need to pass in header and measurement.
  KoBRATDCMeasurement(uint32_t channel, uint32_t measurement):
    tdc_channel_word(channel),
    tdc_measurement_word(measurement){};

  KoBRATDCMeasurement();    
};


/// Class to store data from CAEN V1190.
/// We store the information as a vector of TDCMeasurement's
/// Question: do we need a way of retrieving information about TDCs that
/// have no measurements?  Currently this information is not exposed.
/// For the definition of obscure variables see the CAEN V1190 manual.
class TKoBRATDCData: public TGenericData {

public:

  /// Constructor
  TKoBRATDCData(int bklen, int bktype, const char* name, void *pdata);


  /// Get Event Counter
  uint32_t GetEventCounter() const {return fGlobalHeader;};
  uint32_t GetGlobalTimeStamp() const {return fGlobalTimeStamp;};
  uint32_t GetLastWord() const {return fLastWord;};
  uint32_t GetLastWord_1() const {return fLastWord_1;}; 

  void Print();

  /// Get the Vector of TDC Measurements.
  std::vector<KoBRATDCMeasurement>& GetMeasurements() {return fMeasurements;}


private:
  
  // We have vectors of the headers/trailers/etc, since there can be 
  // multiple events in a bank.

  /// The overall global header
  uint32_t fGlobalHeader;
  uint32_t fGlobalTimeStamp;
  uint32_t fLastWord, fLastWord_1;

  /// Vector of TDC Measurements.
  std::vector<KoBRATDCMeasurement> fMeasurements;

};

#endif
