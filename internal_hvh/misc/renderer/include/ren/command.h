#ifndef EV0LVE_RENDERER_COMMAND_H
#define EV0LVE_RENDERER_COMMAND_H

#include <memory>
#include <optional>

#include <ren/texture.h>
#include <ren/types.h>
#include <ren/vertex.h>

namespace evo::ren
{
	class layer;

	/**
	 * Adapter command
	 */
	class command
	{
	public:
		/**
		 * Command constructor
		 * @param l Layer pointer
		 */
		command(layer* l, bool set_texture = true);

		/**
		 * Adds vertex to layer
		 * @param vtx Vertex info
		 */
		void add_vertex(const vertex& vtx);

		template<class... V>
		void add_vertex(const vertex& vtx, const V... vtxs)
		{
			add_vertex(vtx);
			add_vertex(vtxs...);
		}

		/**
		 * Adds index to layer
		 * @param idx Index number
		 */
		void add_index(const uint16_t idx);

		template<typename... N>
		void add_index(uint16_t idx, const N... idxs)
		{
			add_index(idx);
			add_index(idxs...);
		}

		bool 					anti_alias{};	// Should enforce anti-aliasing
		uint32_t 				vertices{};		// Vertex count
		uint32_t 				indices{};		// Index count
		std::shared_ptr<texture> texture{};		// Texture to be applied
		rect 					clip{};			// Clip rect
		bool 					no_dpi{};		// Should prevent auto-scaling
		std::optional<float> 	alpha_mod{};	// Alpha modification

	private:
		layer* buffer{};
	};
}

#endif //EV0LVE_RENDERER_COMMAND_H
