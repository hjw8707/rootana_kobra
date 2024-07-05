#ifndef TKoBRAADCData_hxx_seen
#define TKoBRAADCData_hxx_seen

#include <vector>

#include "TGenericData.hxx"

/// Class for each ADC measurement
/// For the definition of obscure variables see the CAEN V1190 manual.
class KoBRAADCMeasurement {

  friend class TKoBRAADCData;

public:

  /// Get the channel number
  uint32_t GetChannel() const {return adc_channel_word;}
  /// Get the ADC measurement
  uint32_t GetMeasurement() const {return adc_measurement_word;}


private:

  /// Found fields to hold the header, measurement, trailer and error words.
  uint32_t adc_channel_word;
  uint32_t adc_measurement_word;

  
  /// Constructor; need to pass in header and measurement.
  KoBRAADCMeasurement(uint32_t channel, uint32_t measurement):
    adc_channel_word(channel),
    adc_measurement_word(measurement){};



  KoBRAADCMeasurement();    
};


/// Class to store data from CAEN V1190.
/// We store the information as a vector of ADCMeasurement's
/// Question: do we need a way of retrieving information about ADCs that
/// have no measurements?  Currently this information is not exposed.
/// For the definition of obscure variables see the CAEN V1190 manual.
class TKoBRAADCData: public TGenericData {

public:

  /// Constructor
  TKoBRAADCData(int bklen, int bktype, const char* name, void *pdata);


  /// Get Event Counter
  uint32_t GetLastWord() const {return fLastWord;};
  uint32_t GetLastWord_1() const {return fLastWord_1;}; 
  uint32_t GetLastWord_2() const {return fLastWord_2;}; 

  void Print();

  /// Get the Vector of ADC Measurements.
  std::vector<KoBRAADCMeasurement>& GetMeasurements() {return fMeasurements;}


private:
  
  // We have vectors of the headers/trailers/etc, since there can be 
  // multiple events in a bank.

  /// The overall global header
  uint32_t fLastWord, fLastWord_1, fLastWord_2;

  /// Vector of ADC Measurements.
  std::vector<KoBRAADCMeasurement> fMeasurements;

};

#endif
