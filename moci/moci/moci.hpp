#pragma once

#include "moci_core/moci_core.hpp"
#include "moci_math/moci_math.hpp"
#include "moci_network/moci_network.hpp"
#include "moci_system/moci_system.hpp"

#include "moci_render_base/moci_render_base.hpp"

#include "moci_graphics/moci_graphics.hpp"

#include "moci/app/application.hpp"
#include "moci/app/input.hpp"
#include "moci/app/key_codes.hpp"
#include "moci/app/layer.hpp"
#include "moci/app/layer_stack.hpp"
#include "moci/app/mouse_button_codes.hpp"
#include "moci/app/timestep.hpp"
#include "moci/app/window.hpp"

#include "moci/app/events/application_event.hpp"
#include "moci/app/events/key_event.hpp"
#include "moci/app/events/mouse_event.hpp"

#include "moci/ui/component.hpp"
#include "moci/ui/component_layer.hpp"
#include "moci/ui/style.hpp"
#include "moci/ui/widgets.hpp"

// main should only be included once.
#if defined(MOCI_ENTRYPOINT)
#include "moci/app/entry_point.hpp"
#endif
