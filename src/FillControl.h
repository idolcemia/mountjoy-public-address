#ifndef FILL_CONTROL_H
#define FILL_CONTROL_H

#include <Arduino.h>
#include <lvgl.h>

enum FillControlState
{
    FILL_RESET = 0,
    FILL_RUN = 1,
    FILL_PAUSE = 2,
    FILL_DONE = 3
};

/**
 * Controls liquid filling using a fluid level sensor and solenoid valve.
 *
 * CIRCUIT CONFIGURATION:
 * HOW IT WORKS:
 */

class FillControl
{
private:
    static const int FILL_AVG_WINDOW = 10; // Number of recent readings to average for smoothing

    int _sensorPin;
    int _valvePin;


    float _recentVolumes[FILL_AVG_WINDOW] = {0.0f}; // Circular buffer to hold recent volume readings for smoothing
    int _recentVolumeIndex = 0;
    int _recentVolumeCount = 0;
    float _recentVolumeSum = 0.0f;

  
 
   
    #define SERIES_RESISTOR 560
    #define ZERO_VOLUME_RESISTANCE 540 // Resistance reading when the tube is empty.  This will depend on your specific sensor and setup, so you should measure it with an empty tube to get an accurate value.
    #define CALIBRATION_RESISTANCE 1800 // Resistance at 27.5 cm of rise.
    #define CALIBRATION_VOLUME 19.23   // Volume in liters at 27.5 cm of rise.  Assuing a 30cm diameter tube,

                                        // 699 cubic centimeters per cm of rise, so 27.5 cm * 699 cc/cm = 19227.5 cc = 19.23 liters.  Adjust the calibration volume if your tube diameter is different.

public:
    /**
     * Constructor
     *
     */

    FillControlState _state = FILL_DONE;
    unsigned long _startTimeMillis = 0;
    float _fillAmount;

    // Actual fill time is 56 seconds.
    unsigned long _fillTimeMillis = 0; // Will be calculated in begin()

    void begin();

    FillControl(
        int sensorPin,
        int valvePin,
        float fillAmount = 0) : _sensorPin(sensorPin), _valvePin(valvePin), _fillAmount(fillAmount)
    {
    }

    float getFillVolume() ;
    float readResistance(int pin, int seriesResistance);
    float resistanceToVolume(float resistance, float zeroResistance, float calResistance, float calVolume);
    
    /**
     * Start filling
     */
    void start();

    /**
     * Stop filling
     */
    void stop();

    /**
     * Reset the fill control state and stop filling
     */
    void reset();

    /**
     * Interrupt Service Routine to count pulses from the flow sensor
     */
    void isr();

    // Interrupt service routine
    static void isr_wrapper(void);

    void updateUI();
};

#endif // FILL_CONTROL_H
