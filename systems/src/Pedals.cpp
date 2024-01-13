#include "Pedals.h"

PedalsComponentInterface PedalsComponent::evaluate_pedals(const PedalsDriverInterface &data)
{
    PedalsComponentInterface out;
    out.accelImplausible = evaluate_pedal_implausibilities_(data.accelPedalPosition1, data.accelPedalPosition2, accelParams_, 0.1);
    out.brakeImplausible = evaluate_pedal_implausibilities_(data.brakePedalPosition1, data.brakePedalPosition2, brakeParams_, 0.25);

    return out;
}

bool PedalsComponent::evaluate_pedal_implausibilities_(int sense_1, int sense_2, const PedalsParams &params, float max_percent_diff)
{
    // FSAE EV.5.5
    // FSAE T.4.2.10
    bool pedal_1_less_than_min = (sense_1 < params.min_sense_1);
    bool pedal_2_less_than_min = (sense_2 < params.min_sense_2);
    bool pedal_1_greater_than_max = (sense_1 > params.max_sense_1);
    bool pedal_2_greater_than_max = (sense_2 > params.max_sense_2);

    // check that the pedals are reading within 10% of each other
    // T.4.2.4
    float scaled_pedal_1 = (sense_1 - params.start_sense_1) / (params.end_sense_1 - params.start_sense_1);
    float scaled_pedal_2 = (sense_2 - params.start_sense_2) / (params.end_sense_2 - params.start_sense_2);
    bool sens_not_within_req_percent = (fabs(scaled_pedal_1 - scaled_pedal_2) > max_percent_diff);

    if (
        pedal_1_less_than_min ||
        pedal_2_less_than_min ||
        pedal_1_greater_than_max ||
        pedal_2_greater_than_max)
    {
        return true;
    }
    else if (sens_not_within_req_percent)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool PedalsComponent::evaluate_brake_and_accel_pressed_(const PedalsDriverInterface &data)
{

    bool accel_pressed = pedal_is_active_(data.accelPedalPosition1, data.accelPedalPosition2, accelParams_, 0.1);
    bool brake_pressed = pedal_is_active_(data.brakePedalPosition1, data.brakePedalPosition2, brakeParams_, 0.05);
    
    return (accel_pressed && brake_pressed);

}

bool PedalsComponent::pedal_is_active_(int sense_1, int sense_2, const PedalsParams &pedalParams, float percent_threshold)
{
    bool pedal_1_is_active = (sense_1 > (((pedalParams.end_sense_1 - pedalParams.start_sense_1) * percent_threshold) + pedalParams.start_sense_1));
    bool pedal_2_is_active = (sense_2 > (((pedalParams.end_sense_2 - pedalParams.start_sense_2) * percent_threshold) + pedalParams.start_sense_2));

    return (pedal_1_is_active || pedal_2_is_active);
}