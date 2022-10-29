/**
 * @file camera_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

TEST_CASE("moci_graphics_extra: OrthographicCamera", "[render]")
{
    moci::OrthographicCamera camera {0.0F, 100.0F, 100.0F, 0.0F};
    REQUIRE(camera.GetPosition() == glm::vec3 {});
    REQUIRE(camera.GetRotation() == 0.0F);
    REQUIRE(camera.GetViewMatrix() == glm::mat4 {1.0F});
    REQUIRE(camera.GetViewProjectionMatrix() == glm::ortho(0.0F, 100.0F, 100.0F, 0.0F, -1.0F, 1.0F));
}