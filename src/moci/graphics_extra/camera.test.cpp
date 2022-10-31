/**
 * @file camera.test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

TEST_CASE("graphics_extra: OrthographicCamera", "[render]")
{
    moci::OrthographicCamera camera {0.0F, 100.0F, 100.0F, 0.0F};
    REQUIRE(camera.getPosition() == glm::vec3 {});
    REQUIRE(camera.getRotation() == 0.0F);
    REQUIRE(camera.getViewMatrix() == glm::mat4 {1.0F});
    REQUIRE(camera.getViewProjectionMatrix() == glm::ortho(0.0F, 100.0F, 100.0F, 0.0F, -1.0F, 1.0F));
}