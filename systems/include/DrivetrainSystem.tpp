#include "DrivetrainSystem.h"

template <typename InverterType>
bool DrivetrainSystem<InverterType>::inverter_init_timeout(unsigned long curr_time)
{
    return ((int)(curr_time - drivetrain_initialization_phase_start_time_) > init_time_limit_ms_);
}

template <typename InverterType>
bool DrivetrainSystem<InverterType>::handle_inverter_startup(unsigned long curr_time)
{
    if (drivetrain_ready_() && !hv_en_requested_)
    {
        enable_drivetrain_hv_(curr_time);
        hv_en_requested_ = true;
    }
    else if (drivetrain_ready_() && check_drivetrain_quit_dc_on_() && !enable_requested_)
    {
        request_enable_();
        enable_requested_ = true;
    }
    bool all_ready = ( drivetrain_ready_() && check_drivetrain_quit_dc_on_() && drivetrain_enabled_() );
    return all_ready;
}

// command functions
template <typename InverterType>
void DrivetrainSystem<InverterType>::enable_drivetrain_hv_(unsigned long curr_time)
{

    for (auto inv_pointer : inverters_)
    {
        inv_pointer->request_enable_hv();
    }
    drivetrain_initialization_phase_start_time_ = curr_time;
}

template <typename InverterType>
void DrivetrainSystem<InverterType>::request_enable_()
{
    for (auto inv_pointer : inverters_)
    {
        inv_pointer->request_enable_inverter();
    }
}

template <typename InverterType>
void DrivetrainSystem<InverterType>::command_drivetrain_no_torque()
{
    for (auto inv_pointer : inverters_)
    {
        inv_pointer->command_no_torque();
    }
}

template <typename InverterType>
bool DrivetrainSystem<InverterType>::drivetrain_error_occured()
{
    for (auto inv_pointer : inverters_)
    {
        if (inv_pointer->error())
        {
            return true;
        }
    }
    return false;
}

template <typename InverterType>
void DrivetrainSystem<InverterType>::reset_drivetrain()
{
    for (auto inv_pointer : inverters_)
    {
        inv_pointer->command_reset();
    }
}

template <typename InverterType>
void DrivetrainSystem<InverterType>::command_drivetrain(const DrivetrainCommand_s &data)
{

    // inverters_[0]->handle_command(data.left_front_inverter_cmd);
    // inverters_[1]->handle_command(data.right_front_inverter_cmd);
    // inverters_[2]->handle_command(data.left_rear_inverter_cmd);
    // inverters_[3]->handle_command(data.right_rear_inverter_cmd);
}

// feedback functions
template <typename InverterType>
bool DrivetrainSystem<InverterType>::hv_over_threshold_on_drivetrain()
{
    for (auto inv_pointer : inverters_)
    {
        if (!(inv_pointer->dc_bus_voltage() > min_hv_voltage_))
        {
            return false;
        }
    }
    return true;
}
template <typename InverterType>
bool DrivetrainSystem<InverterType>::drivetrain_ready_()
{
    for (auto inv_pointer : inverters_)
    {
        if (!inv_pointer->inverter_system_ready())
        {
            return false;
        }
    }
    return true;
}

template <typename InverterType>
bool DrivetrainSystem<InverterType>::check_drivetrain_quit_dc_on_()
{
    for (auto inv_pointer : inverters_)
    {
        if (!inv_pointer->dc_quit_on())
        {
            return false;
        }
    }
    return true;
}

template <typename InverterType>
bool DrivetrainSystem<InverterType>::drivetrain_enabled_()
{
    for (auto inv_pointer : inverters_)
    {
        if (!inv_pointer->quit_inverter_on())
        {
            return false;
        }
    }
    return true;
}
