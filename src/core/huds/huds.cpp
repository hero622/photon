#include "huds.h"

#include "photon-sdk/photon.h"

#include <algorithm>
#include <cmath>

static sdk::vec2_t get_abs_pos( photon_api::hud_t *hud ) {
	const auto pos = photon->render->to_screen( hud->pos );
	const auto anchor = hud->anchor * hud->bounds;

	return pos - anchor;
}

static void set_abs_pos( photon_api::hud_t *hud, sdk::vec2_t pos ) {
	const auto screen_size = photon->render->get_screen_size( );

	const auto anchor = hud->anchor * hud->bounds;

	auto new_pos = pos + anchor;

	new_pos.x = std::clamp( new_pos.x, ( float ) huds::safezone_x, screen_size.x - huds::safezone_x );
	new_pos.y = std::clamp( new_pos.y, ( float ) huds::safezone_y, screen_size.y - huds::safezone_y );

	hud->pos = photon->render->normalize( new_pos );
}

static void set_hud_anchor( photon_api::hud_t *hud ) {
	const auto screen_size = photon->render->get_screen_size( );

	const auto center = get_abs_pos( hud ) + sdk::vec2_t( hud->bounds.x / 2, hud->bounds.y / 2 );

	if ( ( int ) center.x < screen_size.x / 2 )
		hud->anchor.x = 0.0f;
	else if ( ( int ) center.x > screen_size.x / 2 )
		hud->anchor.x = 1.0f;
	else
		hud->anchor.x = 0.5f;

	if ( ( int ) center.y < screen_size.y / 2 )
		hud->anchor.y = 0.0f;
	else if ( ( int ) center.y > screen_size.y / 2 )
		hud->anchor.y = 1.0f;
	else
		hud->anchor.y = 0.5f;
}

struct hud_bounds_t {
	sdk::vec2_t mins;
	sdk::vec2_t maxs;
	sdk::vec2_t center;
};

static hud_bounds_t get_hud_bounds( photon_api::hud_t *hud ) {
	const auto hud_pos = get_abs_pos( hud );

	hud_bounds_t bounds;
	bounds.mins = sdk::vec2_t( hud_pos.x, hud_pos.y );
	bounds.maxs = sdk::vec2_t( hud_pos.x + hud->bounds.x, hud_pos.y + hud->bounds.y );
	bounds.center = sdk::vec2_t( hud_pos.x + hud->bounds.x / 2, hud_pos.y + hud->bounds.y / 2 );

	return bounds;
};

/*
 * get all distances to all hud element's edges, sort them, align to closest
 * this whole thing might be overcomplicated, but this was my best idea
 */
static void align_hud_element( photon_api::hud_t *hud ) {
	const auto clr = sdk::color_t( 255, 0, 255, 255 );

	const auto screen_size = photon->render->get_screen_size( );
	const auto hud_rect = get_hud_bounds( hud );

	struct point_distance_t {
		sdk::vec2_t pt1;  // one of the current hud's points
		sdk::vec2_t pt2;  // one of the other hud's points
		float dist;       // squared distance
	};
	std::vector<point_distance_t> distances;

	for ( const auto &other_hud : huds::huds ) {
		if ( hud == other_hud )
			continue;

		const auto other_hud_rect = get_hud_bounds( other_hud );

		// loop over all edges
		for ( int i = 0; i < 3; ++i ) {
			for ( int j = 0; j < 3; ++j ) {
				for ( int axis = 0; axis < 2; ++axis ) {
					/*
					 * NOTE: this whole section is using pointer indexing to access struct members
					 */

					auto pt1 = ( ( sdk::vec2_t * ) &hud_rect )[ i ];
					auto pt2 = ( ( sdk::vec2_t * ) &other_hud_rect )[ j ];

					// since we align huds on axis, not actually by points, set -1 on the axis we dont use
					( ( float * ) &pt1 )[ 1 - axis /* other axis */ ] = -1;
					( ( float * ) &pt2 )[ 1 - axis /* other axis */ ] = -1;

					// delta pos, again only on one axis
					float delta_pos = ( ( float * ) &pt1 )[ axis ] - ( ( float * ) &pt2 )[ axis ];

					point_distance_t dist;
					dist.pt1 = pt1;
					dist.pt2 = pt2;
					dist.dist = delta_pos * delta_pos;  // squared distance

					distances.push_back( dist );
				}
			}
		}
	}

	// sort points by distance
	std::sort( distances.begin( ), distances.end( ), []( const point_distance_t &lhs, const point_distance_t &rhs ) {
		return lhs.dist < rhs.dist;
	} );

	// align hud element x, y but only once per axis
	bool was_vertical = false;
	for ( int i = 0; i < 2; ++i ) {
		const auto &pt_dist = distances[ i ];
		const auto vertical = pt_dist.pt2.x == -1;  // axis check
		const auto hud_pos = get_abs_pos( hud );

		// only align once per axis
		if ( i == 1 && vertical == was_vertical )
			return;

		was_vertical = vertical;

		if ( pt_dist.dist < 100 ) {
			if ( !vertical ) {
				hud->pos.x = ( pt_dist.pt2.x - ( pt_dist.pt1.x - hud_pos.x ) ) / screen_size.x;
				photon->render->draw_line( pt_dist.pt2.x, 0, 0, screen_size.y, clr );
			} else {
				hud->pos.y = ( pt_dist.pt2.y - ( pt_dist.pt1.y - hud_pos.y ) ) / screen_size.y;
				photon->render->draw_line( 0, pt_dist.pt2.y, screen_size.x, 0, clr );
			}
		}
	}
}

void huds::paint( ) {
	for ( const auto &hud : huds ) {
		if ( hud->type == photon_api::hudtype_hud ) {
			( ( photon_api::i_hud * ) hud )->paint( );
		} else {
			const auto thud = ( photon_api::i_thud * ) hud;

			// TODO: improve on this system or maybe even rework the thud system completely
			auto text = std::string( thud->format );

			utils::string::replace( text, "{name}", std::string( thud->get_name( ) ) );
			utils::string::replace( text, "{value}", std::string( thud->get_text( ) ) );

			const auto font = photon->render->get_font( thud->font );

			thud->bounds = photon->render->get_text_size( font, text.c_str( ) );

			const auto pos = get_abs_pos( thud );

			photon->render->draw_text( pos.x, pos.y, font, { 255, 255, 255, 255 }, false, text.c_str( ) );
		}
	}
}

void huds::paint_ui( ) {
	const auto clr = sdk::color_t( 0, 255, 255, 255 );

	const auto screen_size = photon->render->get_screen_size( );

	static photon_api::hud_t *cur_hud;
	static sdk::vec2_t grab_pos;

	for ( const auto &hud : huds ) {
		sdk::vec2_t orig_cur_pos;

		const auto pos = get_abs_pos( hud );

		if ( photon->input->is_cursor_in_area( pos.x, pos.y, pos.x + hud->bounds.x, pos.y + hud->bounds.y ) ) {
			photon->render->draw_outlined_rect( pos.x - 1, pos.y - 1, hud->bounds.x + 2, hud->bounds.y + 2, clr );
			photon->render->draw_filled_rect( pos.x + hud->anchor.x * hud->bounds.x - 3, pos.y + hud->anchor.y * hud->bounds.y - 3, 6, 6, clr );

			if ( photon->input->get_key_press( sdk::mouse_left ) ) {
				cur_hud = hud;
				grab_pos = photon->input->get_cursor_position( ) - pos;
			}
		}
	}

	if ( cur_hud ) {
		if ( photon->input->get_key_held( sdk::mouse_left ) ) {
			const auto hud = cur_hud;

			set_hud_anchor( hud );

			set_abs_pos( hud, photon->input->get_cursor_position( ) - grab_pos );

			align_hud_element( hud );
		}

		if ( photon->input->get_key_release( sdk::mouse_left ) ) {
			cur_hud = nullptr;
		}
	}
}
