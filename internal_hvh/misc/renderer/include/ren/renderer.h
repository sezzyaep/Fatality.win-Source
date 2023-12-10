//
// Created by panzerfaust on 9/16/2020.
//

#ifndef EV0LVE_RENDERER_RENDERER_H
#define EV0LVE_RENDERER_RENDERER_H

#include <memory>
#include <map>
#include <functional>
#include <mutex>

#include <ren/layer.h>
#include <ren/font.h>
#include <ren/anim.h>

namespace evo::ren
{
	class renderer
	{
	public:
		renderer()
		{
			set_layer( layer::background );
			set_layer( layer::foreground );
		}

		template<typename T>
		std::shared_ptr<T> get_anim( const uint32_t i )
		{
			return anims[ i ]->as<T>();
		}

		std::shared_ptr<anim_base>& get_anim( const uint32_t i )
		{
			return anims[ i ];
		}

		void manage( const uint32_t i, const std::shared_ptr<anim_base>& a )
		{
			anims[ i ] = a;
		}

		void forget_anim( const uint32_t i )
		{
			if ( const auto p = anims.find( i ); p != anims.end() )
				anims.erase( p );
		}

		void forget_all_anims()
		{
			while ( !anims.empty() )
				forget_anim( anims.begin()->first );
		}

		std::shared_ptr<font_base>& get_font( const uint32_t i )
		{
			return fonts[ i ];
		}

		void manage( const uint32_t i, const std::shared_ptr<font_base>& f )
		{
			fonts[ i ] = f;
		}

		void forget_font( const uint32_t i, bool d = false )
		{
			if ( const auto p = fonts.find( i ); p != fonts.end() )
			{
				if ( d )
					fonts[ i ]->destroy();

				fonts.erase( p );
			}
		}

		void forget_all_fonts( bool d = false )
		{
			while ( !fonts.empty() )
				forget_font( fonts.begin()->first, d );
		}

		std::shared_ptr<texture>& get_texture( const uint32_t i )
		{
			return textures[ i ];
		}

		void manage( const uint32_t i, const std::shared_ptr<texture>& f )
		{
			textures[ i ] = f;
		}

		void forget_texture( const uint32_t i, bool d = false )
		{
			if ( const auto p = textures.find( i ); p != textures.end() )
			{
				if ( d )
					textures[ i ]->destroy();

				textures.erase( p );
			}
		}

		void forget_all_textures( bool d = false )
		{
			while ( !textures.empty() )
				forget_texture( textures.begin()->first, d );
		}

		layer& set_layer( const uint8_t idx, const layer& l = {} )
		{
			layers[ idx ] = l;
			return layers[ idx ];
		}

		layer& get_layer( const uint8_t idx = layer::background )
		{
			return layers[ idx ];
		}

		void forget_everything( bool d = false )
		{
			forget_all_anims();
			forget_all_fonts( d );
			forget_all_textures( d );
		}

		size_t get_total_vertices();
		size_t get_total_indices();

		void for_each_layer( const std::function<void( layer& )>& fn );
		void for_each_font( const std::function<void( std::shared_ptr<font_base>& )>& fn );
		void for_each_texture( const std::function<void( std::shared_ptr<texture>& )>& fn );
		void for_each_anim( const std::function<void( std::shared_ptr<anim_base>& )>& fn );

		void swap_buffers( std::map<uint8_t, layer>& buffer )
		{
			std::lock_guard _lock( mtx );
			std::swap( layers, buffer );
		}

		float get_frame_time() const
		{
			return frame_time;
		}

		void set_frame_time( float ft )
		{
			frame_time = ft;
		}

		uint32_t get_free_font_slot()
		{
			return get_free_slot( fonts );
		}

		uint32_t get_free_texture_slot()
		{
			return get_free_slot( textures );
		}

		uint32_t get_free_anim_slot()
		{
			return get_free_slot( anims );
		}

		bool has_font( uint32_t id )
		{
			return fonts.find( id ) != fonts.end();
		}

		bool has_texture( uint32_t id )
		{
			return textures.find( id ) != textures.end();
		}

		bool has_anim( uint32_t id )
		{
			return anims.find( id ) != anims.end();
		}

	private:
		std::map<uint8_t, layer> 								layers{};
		std::unordered_map<uint32_t, std::shared_ptr<font_base>> 	fonts{};
		std::unordered_map<uint32_t, std::shared_ptr<texture>> 	textures{};
		std::unordered_map<uint32_t, std::shared_ptr<anim_base>>	anims{};

		std::mutex mtx{};
		float frame_time{};

		template<typename T>
		uint32_t get_free_slot( const std::unordered_map<uint32_t, std::shared_ptr<T>>& a )
		{
			for ( auto i = 0u; i < UINT32_MAX - 1; i++ )
			{
				if ( a.find( i ) == a.end() || !a.at( i ) )
					return i;
			}

			return -1;
		}
	};

	inline renderer draw{};
}

#endif //EV0LVE_RENDERER_RENDERER_H
