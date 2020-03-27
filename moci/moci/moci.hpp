#pragma once

#include "moci/core/preprocessor.hpp"

#include "moci/benchmark/benchmark.hpp"

#include "moci/core/algorithm.hpp"
#include "moci/core/logging.hpp"
#include "moci/core/memory.hpp"
#include "moci/core/property_map.hpp"
#include "moci/core/scope_guard.hpp"
#include "moci/core/strings.hpp"
#include "moci/core/vector.hpp"

#include "moci/debug/instrumentor.hpp"

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

#include "moci/geometry/line.hpp"
#include "moci/geometry/point.hpp"
#include "moci/geometry/rectangle.hpp"

#include "moci/network/datagram.hpp"

#include "moci/render/buffer.hpp"
#include "moci/render/camera.hpp"
#include "moci/render/color.hpp"
#include "moci/render/font.hpp"
#include "moci/render/graphics_context.hpp"
#include "moci/render/image.hpp"
#include "moci/render/light.hpp"
#include "moci/render/mesh.hpp"
#include "moci/render/render_command.hpp"
#include "moci/render/render_queue.hpp"
#include "moci/render/renderer.hpp"
#include "moci/render/renderer_api.hpp"
#include "moci/render/shader.hpp"
#include "moci/render/texture.hpp"
#include "moci/render/vertex_array.hpp"

#include "moci/render/obj/file.hpp"

#include "moci/ui/component.hpp"
#include "moci/ui/component_layer.hpp"
#include "moci/ui/style.hpp"
#include "moci/ui/widgets.hpp"

#include "moci/system/info.hpp"

// main should only be included once.
#if defined(MOCI_ENTRYPOINT)
#include "moci/app/entry_point.hpp"
#endif
