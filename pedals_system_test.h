/*
basic test cases for the pedals component class
version 1.0
rough draft
author: Lucas Plant
*/

// TODO @ben

#ifndef PEDALS_SYSTEM_TEST
#define PEDALS_SYSTEM_TEST
#include <gtest/gtest.h>
#include <string>
#include "PedalsSystem.h"
#include <array>


struct PedalIsActiveTestCase
{
    // defines input and output params

    // input params
    PedalsDriverInterface pedalsinput;

    // expected output
    bool expect_active;
};



TEST(PedalsSystemTesting, test_pedal_is_active)
{
    PedalsParams params_for_test = {1, 1, 10, 10, 1, 1, 9, 9};
    PedalsSystem pedals_system(params_for_test, params_for_test);
    std::array<PedalIsActiveTestCase, 1> test_cases{{{{0, 0, 3, 3}, true}}};

    for (const auto &test_case : test_cases)
    {
        auto out = pedals_system.mech_brake_active(test_case.pedalsinput);
        EXPECT_EQ(test_case.expect_active, out);
    }
}




#endif /* PEDALS_SYSTEM_TEST */
