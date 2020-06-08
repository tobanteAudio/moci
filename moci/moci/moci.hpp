#pragma once

#include "moci_core/moci_core.hpp"
#include "moci_math/moci_math.hpp"
#include "moci_network/moci_network.hpp"
#include "moci_system/moci_system.hpp"

#include "moci_graphics_api_base/moci_graphics_api_base.hpp"

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

#include "moci/render/camera.hpp"
#include "moci/render/color.hpp"
#include "moci/render/font.hpp"
#include "moci/render/image.hpp"
#include "moci/render/light.hpp"
#include "moci/render/mesh.hpp"
#include "moci/render/render_command.hpp"
#include "moci/render/render_queue.hpp"
#include "moci/render/renderer.hpp"
#include "moci/render/renderer_api.hpp"

#include "moci/render/obj/file.hpp"

#include "moci/ui/component.hpp"
#include "moci/ui/component_layer.hpp"
#include "moci/ui/style.hpp"
#include "moci/ui/widgets.hpp"

// main should only be included once.
#if defined(MOCI_ENTRYPOINT)
#include "moci/app/entry_point.hpp"
#endif
