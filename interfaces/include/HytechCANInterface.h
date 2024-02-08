#ifndef HYTECHCANINTERFACE
#define HYTECHCANINTERFACE

#include <tuple>
#include "FlexCAN_T4.h"

#include "hytech.h"
#include "HyTech_CAN.h"

#include "InverterInterface.h"
#include "DashboardInterface.h"
#include "AMSInterface.h"

template <typename circular_buffer>
struct CANInterfaces
{
    InverterInterface<circular_buffer> *front_left_inv;
    InverterInterface<circular_buffer> *front_right_inv;
    InverterInterface<circular_buffer> *rear_left_inv;
    InverterInterface<circular_buffer> *rear_right_inv;
    DashboardInterface *dash_interface;
    AMSInterface *ams_interface;
};

// the goal with the can interface is that there exists a receive call that appends to a circular buffer
// the processing of the receive queue happens on every iteration of the loop
// in the processing of the receive call, all of the messages received get de-serialized and passed to their interfaces

extern Circular_Buffer<uint8_t, (uint32_t)16, sizeof(CAN_message_t)>
    CAN1_rxBuffer;
extern Circular_Buffer<uint8_t, (uint32_t)16, sizeof(CAN_message_t)>
    CAN2_rxBuffer;

extern Circular_Buffer<uint8_t, (uint32_t)16, sizeof(CAN_message_t)>
    CAN3_rxBuffer;

extern Circular_Buffer<uint8_t, (uint32_t)16, sizeof(CAN_message_t)>
    CAN1_txBuffer;
extern Circular_Buffer<uint8_t, (uint32_t)16, sizeof(CAN_message_t)>
    CAN2_txBuffer;

extern Circular_Buffer<uint8_t, (uint32_t)16, sizeof(CAN_message_t)>
    CAN3_txBuffer;

void on_can1_receive(const CAN_message_t &msg);
void on_can2_receive(const CAN_message_t &msg);
void on_can3_receive(const CAN_message_t &msg);

// reads from receive buffer updating the current message frame from a specific receive buffer
// TODO ensure that all of the repeated interfaces are at the correct IDs
// FL = MC1
// FR = MC2
// RL = MC3
// RR = MC4
template <typename BufferType, typename InterfaceType>
void process_ring_buffer(BufferType &rx_buffer, const InterfaceType &interfaces, unsigned long curr_millis)
{
    // TODO switch to using the global CAN receive function from the generated CAN library

    while (rx_buffer.available())
    {
        CAN_message_t recvd_msg;
        uint8_t buf[sizeof(CAN_message_t)];
        rx_buffer.pop_front(buf, sizeof(CAN_message_t));
        memmove(&recvd_msg, buf, sizeof(recvd_msg));
        switch (recvd_msg.id)
        {

        case DASHBOARD_STATE_CANID:
            interfaces.dash_interface->read(recvd_msg);
            break;
        
        // AMS msg receives
        case ID_BMS_STATUS:
            interfaces.ams_interface->retrieve_status_CAN(recvd_msg, curr_millis);
            break;
        case ID_BMS_TEMPERATURES:
            interfaces.ams_interface->retrieve_temp_CAN(recvd_msg);
            break;
        case ID_BMS_VOLTAGES:
            interfaces.ams_interface->retrieve_voltage_CAN(recvd_msg);
            break;
        
            // MC status msgs
        case ID_MC1_STATUS:
            interfaces.front_left_inv->receive_status_msg(recvd_msg);
            break;
        case ID_MC2_STATUS:
            interfaces.front_right_inv->receive_status_msg(recvd_msg);
            break;
        case ID_MC3_STATUS:
            interfaces.rear_left_inv->receive_status_msg(recvd_msg);
            break;
        case ID_MC4_STATUS:
            interfaces.rear_right_inv->receive_status_msg(recvd_msg);
            break;

            // MC temp msgs
        case ID_MC1_TEMPS:
            interfaces.front_left_inv->receive_temp_msg(recvd_msg);
            break;
        case ID_MC2_TEMPS:
            interfaces.front_right_inv->receive_temp_msg(recvd_msg);
            break;
        case ID_MC3_TEMPS:
            interfaces.rear_left_inv->receive_temp_msg(recvd_msg);
            break;
        case ID_MC4_TEMPS:
            interfaces.rear_right_inv->receive_temp_msg(recvd_msg);
            break;

            // MC energy msgs
        case ID_MC1_ENERGY:
            interfaces.front_left_inv->receive_energy_msg(recvd_msg);
            break;
        case ID_MC2_ENERGY:
            interfaces.front_right_inv->receive_energy_msg(recvd_msg);
            break;
        case ID_MC3_ENERGY:
            interfaces.rear_left_inv->receive_energy_msg(recvd_msg);
            break;
        case ID_MC4_ENERGY:
            interfaces.rear_right_inv->receive_energy_msg(recvd_msg);
            break;
        }
    }
}

template <typename bufferType>
void send_all_CAN_msgs(bufferType &buffer, FlexCAN_T4_Base *can_interface)
{
    CAN_message_t msg;
    while (buffer.available())
    {
        CAN_message_t msg;
        uint8_t buf[sizeof(CAN_message_t)];
        buffer.pop_front(buf, sizeof(CAN_message_t));
        memmove(&msg, buf, sizeof(msg));
        can_interface->write(msg);
    }
}

#endif /* HYTECHCANINTERFACE */
