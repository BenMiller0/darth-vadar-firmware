#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// FSR voltage-divider output. GPIO 0 is ADC1 channel 0 on the ESP32-C3.
#define FSR_PIN                         0
#define FSR_SAMPLE_INTERVAL_MS          100

// Read the ADC with the full 12-bit range and a wide input attenuation.
#define ADC_RESOLUTION_BITS             12
#define ADC_ATTENUATION                 ADC_11db

#endif // CONSTANTS_HPP
