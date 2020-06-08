/**
 * @file camera_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci_graphics/camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

TEST_CASE("moci_graphics: OrthographicCamera", "[render]")
{
    moci::OrthographicCamera camera {0.0f, 100.0f, 100.0f, 0.0f};
    REQUIRE(camera.GetPosition() == glm::vec3 {});
    REQUIRE(camera.GetRotation() == 0.0f);
    REQUIRE(camera.GetViewMatrix() == glm::mat4 {1.0f});
    REQUIRE(camera.GetViewProjectionMatrix() == glm::ortho(0.0f, 100.0f, 100.0f, 0.0f, -1.0f, 1.0f));
}