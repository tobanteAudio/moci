/**
 * @file label_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "moci/widgets/component/label.hpp"
#include "moci/widgets/style/style.hpp"

TEST_CASE("widget: LabelConstruct", "[ui]")
{
    moci::Label const label {"test", {}};
    REQUIRE(label.getText() == "test");
    REQUIRE(label.getTextView() == "test");
}

TEST_CASE("widget: LabelSetText", "[ui]")
{
    moci::Label label {"", {}};
    REQUIRE(label.getText().empty());
    REQUIRE(label.getTextView().empty());
    label.setText("test143");
    REQUIRE(label.getText() == "test143");
    REQUIRE(label.getTextView() == "test143");
}

TEST_CASE("widget: LabelSetTextColor", "[ui]")
{
    moci::Label label {"", {}};

    auto c1 = label.getTextColor();
    REQUIRE(c1.getRed() == 0.0F);
    REQUIRE(c1.getGreen() == 0.0F);
    REQUIRE(c1.getBlue() == 0.0F);
    REQUIRE(c1.getAlpha() == 0.0F);

    label.setTextColor(moci::Colors::black);

    auto c2 = label.getTextColor();
    REQUIRE(c2.getRed() == 0.0F);
    REQUIRE(c2.getGreen() == 0.0F);
    REQUIRE(c2.getBlue() == 0.0F);
    REQUIRE(c2.getAlpha() == 1.0F);
}
