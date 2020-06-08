#pragma once

#include "moci_application/moci_application.hpp"
#include "moci_core/moci_core.hpp"
#include "moci_events/moci_events.hpp"
#include "moci_graphics/moci_graphics.hpp"
#include "moci_math/moci_math.hpp"
#include "moci_network/moci_network.hpp"
#include "moci_render_base/moci_render_base.hpp"
#include "moci_system/moci_system.hpp"

#include "moci/ui/component.hpp"
#include "moci/ui/component_layer.hpp"
#include "moci/ui/style.hpp"
#include "moci/ui/widgets.hpp"

// main should only be included once.
#if defined(MOCI_ENTRYPOINT)
#include "moci_application/entry_point.hpp"
#endif
