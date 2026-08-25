/*
 * test_trig.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 16/08/2026
 */

#include <doctest/doctest.h>

#include "math/float.h"
#include "math/trig.h"

TEST_SUITE("math::trig")
{
    TEST_CASE("sin")
    {
        CHECK(st_float64_equal(st_float64_sin(1.0), 0.8414709848078965));
    }
}
