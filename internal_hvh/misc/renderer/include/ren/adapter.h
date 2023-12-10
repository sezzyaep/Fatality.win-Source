#ifndef EV0LVE_RENDERER_ADAPTER_H
#define EV0LVE_RENDERER_ADAPTER_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ren/types.h>
#include <ren/renderer.h>
#include <ren/texture.h>

namespace evo::ren
{
	/**
	 * Adapter base class
	 */
	class adapter_base
	{
	public:
		adapter_base( HWND wnd = nullptr ) : window( wnd ), run( true )
		{}

		/**
		 * Creates GPU objects if they are not created yet
		 */
		virtual void create_objects()
		{
			draw.for_each_font( [] ( std::shared_ptr<font_base>& f )
			{
				f->create();
			} );

			draw.for_each_texture( [] ( std::shared_ptr<texture>& t )
			{
				t->create();
			} );
		}

		/**
		 * Destroys previously created GPU objects
		 */
		virtual void destroy_objects()
		{
			draw.for_each_font( [] ( std::shared_ptr<font_base>& f )
			{
				f->destroy();
			} );

			draw.for_each_texture( [] ( std::shared_ptr<texture>& t )
			{
				t->destroy();
			} );
		}

		/**
		 * Renders layers onto framebuffer
		 */
		virtual void render()
		{
			if ( !run )
				return;

			draw.for_each_anim( [] ( std::shared_ptr<anim_base>& a )
			{
				a->animate( draw.get_frame_time() );
			} );
		}

		/**
		 * Creates a GPU texture
		 * @param arr RGBA (or similar) buffer
		 * @param w Width
		 * @param h Height
		 * @param p Stride
		 * @return GPU object pointer or nullptr on failure
		 */
		virtual void* create_texture( void* arr, uint32_t w, uint32_t h, uint32_t p ) { return nullptr; }

		/**
		 * Destroys a GPU texture
		 * @param tex GPU object pointer
		 */
		virtual void destroy_texture( void* tex ) {}

		/**
		 * Refreshes draw and rendering informations for frame
		 */
		void refresh()
		{
			if ( !window || !run )
				return;

			RECT client_rect{};
			GetClientRect( window, &client_rect );

			display.x = ( float ) client_rect.right - client_rect.left;
			display.y = ( float ) client_rect.bottom - client_rect.top;
		}

		/**
		 * Flushes layers
		 */
		void flush() const
		{
			draw.for_each_layer( [] ( layer& l )
			{
				if ( l.get_draw_mode() == layer::dm_normal )
					l.clear();
			} );
		}

		bool  		run{};				// Should process layers
		HWND  		window{};			// Window handle
		vec2  		display{};			// Display area (aka #0 clip area)
		float 		dpi{ 1 };				// Scaling
		std::string name{ "None" };	// Adapter name (for debug purposes)
	};

	inline std::shared_ptr<adapter_base> adapter{};
}

#endif //EV0LVE_RENDERER_ADAPTER_H
