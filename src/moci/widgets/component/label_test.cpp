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
    REQUIRE(c1.red() == 0.0F);
    REQUIRE(c1.green() == 0.0F);
    REQUIRE(c1.blue() == 0.0F);
    REQUIRE(c1.alpha() == 0.0F);

    label.setTextColor(moci::Colors::black);

    auto c2 = label.getTextColor();
    REQUIRE(c2.red() == 0.0F);
    REQUIRE(c2.green() == 0.0F);
    REQUIRE(c2.blue() == 0.0F);
    REQUIRE(c2.alpha() == 1.0F);
}
