#ifndef PEDALSSYSTEM
#define PEDALSSYSTEM
#include <math.h>
#include <tuple>

struct PedalsDriverInterface
{
    int accelPedalPosition1;
    int accelPedalPosition2;
    int brakePedalPosition1;
    int brakePedalPosition2;
};

struct PedalsSystemInterface
{
    bool accelImplausible;
    bool brakeImplausible;
    bool brakeAndAccelPressedImplausibility;
    bool isBraking;
    int requestedTorque;
};

/// @brief Pedals params struct that will hold min / max that will be used for evaluateion. 
//          NOTE: min and max may be need to be flipped depending on the sensor. (looking at you brake pedal sensor 2)
struct PedalsParams
{
    int min_sense_1;
    int min_sense_2;
    int max_sense_1;
    int max_sense_2;
    int start_sense_1;
    int start_sense_2;
    int end_sense_1;
    int end_sense_2;
}; 

class PedalsSystem
{
public:
    PedalsSystem(){
        implausibilityStartTime_ = 0;
        // Setting of min and maxes for pedals via config file
    };
    PedalsSystemInterface evaluate_pedals(
        const PedalsDriverInterface &pedal_data, unsigned long curr_time);
    bool max_duration_of_implausibility_exceeded(unsigned long curr_time);
    bool mech_brake_active();

private:
    std::tuple<int, int> linearize_accel_pedal_values_(int accel1, int accel2);

    bool evaluate_pedal_implausibilities_(int sense_1, int sense_2, const PedalsParams &params, float max_percent_differnce);
    
    bool evaluate_brake_and_accel_pressed_(const PedalsDriverInterface &data);
    bool pedal_is_active_(int sense1, int sense_2, const PedalsParams& pedalParams, float percent_threshold); 
    PedalsParams accelParams_;
    PedalsParams brakeParams_;
    unsigned long implausibilityStartTime_;
};


#endif /* PEDALSSYSTEM */

