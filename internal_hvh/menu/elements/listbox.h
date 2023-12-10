#pragma once

namespace listbox_style
{
	enum flags
	{
		none = 1 << 0,
		search_bar = 1 << 1,
	};
}

struct listbox_item_t
{
	listbox_item_t( std::string name, const int index, const color col = gui_color::text_solid() ) : m_index( index ), m_color( col )
	{
		m_name = std::move( name );
		_u( u_name, m_name );
		hash = fnv1a_rt( u_name );
		return;
	}

	std::string m_name;
	int m_index;
	color m_color;
	fnv_t hash;
};

class value;
class text_box;

class listbox : public control
{
public:
	listbox( value* var, std::vector<std::string>* items, int items_to_display = 7, uint8_t flags = listbox_style::flags::none );
	listbox( const std::string& name, value* var, std::vector<std::string>& items, int items_to_display = 7, uint8_t flags = listbox_style::flags::none );
	listbox( const std::string& name, value* var, std::vector<listbox_item_t>* items, int items_to_display = 7, uint8_t flags = listbox_style::flags::none );

	void run_render( evo::ren::vec2& pos ) override;
	void run_input() override;
	void reset_itemcache() { m_last_itemsize = -1; }
	std::wstring& get_search() { return m_search; }
	bool is_listbox() override
	{
		return true;
	}
	std::vector<std::string>& get_items() const
	{
		return *m_items;
	}

	void set_items( const std::vector<std::string>& new_items )
	{
		( m_direct ? *m_items : m_items_alt ) = new_items;
		reset_itemcache();
	}

	void reset_scroll()
	{
		m_scroll_amount = 0;
	}
	void adjust_scroll()
	{
		const auto items_to_display = std::max( 1u, m_items_to_display == -1 ? m_display_items->size() : m_items_to_display );
		if ( m_var->get<int>() < m_scroll_amount - 1 || m_var->get<int>() > m_scroll_amount + items_to_display - 1 )
		{
			m_scroll_amount = m_var->get<int>() - 4;
			if ( static_cast< int >( m_display_items->size() - items_to_display ) > 0 )
				m_scroll_amount = clamp( m_scroll_amount, 0, static_cast< int >( m_display_items->size() - items_to_display ) );
			else
				m_scroll_amount = 0;
		}
	}

	uint8_t m_flags;
private:
	void handle_items() const;

	void render_scroll() const;
	void handle_scroll();

	void cache_items();

	int m_items_to_display;
	int m_scroll_amount;

	bool m_cached_items;

	bool m_direct = false;
	std::vector<std::string>* m_items;
	std::vector<std::string> m_items_alt;
	std::vector<listbox_item_t>* m_display_items;
	std::vector<listbox_item_t> m_display_items_alt;
	int m_last_itemsize;

	std::shared_ptr<text_box> m_search_bar;
	std::wstring m_search;
	int m_old_search;
};
