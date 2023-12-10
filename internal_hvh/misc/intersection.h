#pragma once
#include <stack>

class intersection
{
public:

	struct hitbox_info
	{
		bool valid;
		int index;
		int hitgroup;
		float radius;
		Vector vmin;
		Vector vmax;
	};


	C_CSPlayer* trace_player{};
	bool roll = false;
	std::array< hitbox_info, HITBOX_MAX> player_hitbox_info{};

	intersection( C_CSPlayer* const player, const studiohdr_t* studio_hdr, const matrix3x4_t* matrix, const bool roll = false )
	{
		this->trace_player = player;
		this->roll = roll;

		for ( auto i = 0; i < HITBOX_MAX; i++ )
		{
			player_hitbox_info[ i ].valid = false;

			const auto hitbox = studio_hdr->pHitbox( i, 0 );
			if ( !hitbox )
				continue;

			auto& out = matrix[ hitbox->bone ];

			/*if ( !hitbox->rotation.IsZero() ) // crashes in concat_transforms reading 0xFFFFFFFF with clang dont know why
			{
				matrix3x4_t temp{};
				math::angle_matrix( hitbox->rotation, temp );
				out = math::concat_transforms( out, temp );
			}*/

			Vector vmin{}, vmax{};
			math::vector_transform( hitbox->bbmin, out, vmin );
			math::vector_transform( hitbox->bbmax, out, vmax );

			player_hitbox_info[ i ] = { true, i, hitbox->group, hitbox->radius, vmin, vmax };
		}
	}

	__forceinline void trace_hitboxes( const Ray_t& ray, trace_t& tr )
	{
		tr.m_pEnt = nullptr;

		for ( const auto& hitbox : player_hitbox_info )
		{
			if ( !hitbox.valid )
				continue;

			if ( hitbox.radius == -1.f )
			{
				continue;

				if ( does_intersect_obb( ray, hitbox.vmin, hitbox.vmax ) )
				{
					tr.m_pEnt = trace_player;
					tr.hitgroup = hitbox.hitgroup;
					tr.hitbox = hitbox.index;
					break;
				}
				continue;
			}

			if ( does_intersect_capsule( ray, hitbox.vmin, hitbox.vmax, hitbox.radius ) )
			{
				tr.m_pEnt = trace_player;
				tr.hitgroup = hitbox.hitgroup;
				tr.hitbox = hitbox.index;
				break;
			}
		}
	}

	static __forceinline float segment_to_segment( const Vector s1, const Vector s2, const Vector k1, const Vector k2 )
	{
		static auto constexpr epsilon = 0.00000001;

		auto u = s2 - s1;
		auto v = k2 - k1;
		const auto w = s1 - k1;

		const auto a = u.Dot( u );
		const auto b = u.Dot( v );
		const auto c = v.Dot( v );
		const auto d = u.Dot( w );
		const auto e = v.Dot( w );
		const auto D = a * c - b * b;
		float sN, sD = D;
		float tN, tD = D;

		if ( D < epsilon )
		{
			sN = 0.0;
			sD = 1.0;
			tN = e;
			tD = c;
		}
		else
		{
			sN = b * e - c * d;
			tN = a * e - b * d;

			if ( sN < 0.0 )
			{
				sN = 0.0;
				tN = e;
				tD = c;
			}
			else if ( sN > sD )
			{
				sN = sD;
				tN = e + b;
				tD = c;
			}
		}

		if ( tN < 0.0 )
		{
			tN = 0.0;

			if ( -d < 0.0 )
				sN = 0.0;
			else if ( -d > a )
				sN = sD;
			else
			{
				sN = -d;
				sD = a;
			}
		}
		else if ( tN > tD )
		{
			tN = tD;

			if ( -d + b < 0.0 )
				sN = 0;
			else if ( -d + b > a )
				sN = sD;
			else
			{
				sN = -d + b;
				sD = a;
			}
		}

		const float sc = abs( sN ) < epsilon ? 0.0 : sN / sD;
		const float tc = abs( tN ) < epsilon ? 0.0 : tN / tD;

		auto dP = w + u * sc - v * tc;
		return sqrt( dP.Dot( dP ) );
	}

	static __forceinline bool intersect_line_with_bb( const Ray_t& ray, Vector& min, Vector& max )
	{
		auto tmin = ( min.x - ray.m_Start.x ) / ray.m_Delta.x;
		auto tmax = ( max.x - ray.m_Start.x ) / ray.m_Delta.x;

		if ( tmin > tmax ) std::swap( tmin, tmax );

		auto tymin = ( min.y - ray.m_Start.y ) / ray.m_Delta.y;
		auto tymax = ( max.y - ray.m_Start.y ) / ray.m_Delta.y;

		if ( tymin > tymax ) std::swap( tymin, tymax );

		if ( ( tmin > tymax ) || ( tymin > tmax ) )
			return false;

		if ( tymin > tmin )
			tmin = tymin;

		if ( tymax < tmax )
			tmax = tymax;

		auto tzmin = ( min.z - ray.m_Start.z ) / ray.m_Delta.z;
		auto tzmax = ( max.z - ray.m_Start.z ) / ray.m_Delta.z;

		if ( tzmin > tzmax ) std::swap( tzmin, tzmax );

		if ( ( tmin > tzmax ) || ( tzmin > tmax ) )
			return false;

		if ( tzmin > tmin )
			tmin = tzmin;

		if ( tzmax < tmax )
			tmax = tzmax;

		return true;
	}

	static __forceinline bool does_intersect_capsule( const Ray_t ray, const Vector vmin, const Vector vmax, const float radius )
	{
		const auto dist = segment_to_segment( Vector( ray.m_Start ), ray.m_Start + ray.m_Delta, vmin, vmax );
		return dist < radius;
	}

	static __forceinline bool does_intersect_obb( const Ray_t ray, Vector vmin, Vector vmax )
	{
		return intersect_line_with_bb( ray, vmin, vmax );
	}

	struct BoxTraceInfo_t
	{
		float t1;
		float t2;
		int	hitside;
		bool startsolid;
	};

	static __forceinline bool intersect_with_bbox( Ray_t& ray, const Vector& origin, const Vector& mins, const Vector& maxs, trace_t* tr )
	{
		const auto abs_mins = origin + mins;
		const auto abs_maxs = origin + maxs;
		intersect_ray_with_box( ray.m_Start + ray.m_StartOffset, ray.m_Delta, abs_mins, abs_maxs, 0.0f, tr );
		return tr->DidHit();
	}

	static __forceinline bool intersect_ray_with_box( const Vector& vecRayStart, const Vector& vecRayDelta, const Vector& boxMins, const Vector& boxMaxs, float flTolerance, BoxTraceInfo_t* pTrace )
	{
		int			i;
		float		d1, d2;
		float		f;

		pTrace->t1 = -1.0f;
		pTrace->t2 = 1.0f;
		pTrace->hitside = -1;

		// UNDONE: This makes this code a little messy
		pTrace->startsolid = true;

		for ( i = 0; i < 6; ++i )
		{
			if ( i >= 3 )
			{
				d1 = vecRayStart[ i - 3 ] - boxMaxs[ i - 3 ];
				d2 = d1 + vecRayDelta[ i - 3 ];
			}
			else
			{
				d1 = -vecRayStart[ i ] + boxMins[ i ];
				d2 = d1 - vecRayDelta[ i ];
			}

			// if completely in front of face, no intersection
			if ( d1 > 0 && d2 > 0 )
			{
				// UNDONE: Have to revert this in case it's still set
				// UNDONE: Refactor to have only 2 return points (true/false) from this function
				pTrace->startsolid = false;
				return false;
			}

			// completely inside, check next face
			if ( d1 <= 0 && d2 <= 0 )
				continue;

			if ( d1 > 0 )
			{
				pTrace->startsolid = false;
			}

			// crosses face
			if ( d1 > d2 )
			{
				f = d1 - flTolerance;
				if ( f < 0 )
				{
					f = 0;
				}
				f = f / ( d1 - d2 );
				if ( f > pTrace->t1 )
				{
					pTrace->t1 = f;
					pTrace->hitside = i;
				}
			}
			else
			{
				// leave
				f = ( d1 + flTolerance ) / ( d1 - d2 );
				if ( f < pTrace->t2 )
				{
					pTrace->t2 = f;
				}
			}
		}

		return pTrace->startsolid || ( pTrace->t1 < pTrace->t2&& pTrace->t1 >= 0.0f );
	}

	static __forceinline bool intersect_ray_with_box( const Vector& vecRayStart, const Vector& vecRayDelta, const Vector& boxMins, const Vector& boxMaxs, float flTolerance, CBaseTrace* pTrace, float* pFractionLeftSolid = nullptr )
	{
		BoxTraceInfo_t trace;

		if ( intersect_ray_with_box( vecRayStart, vecRayDelta, boxMins, boxMaxs, flTolerance, &trace ) )
		{
			pTrace->startsolid = trace.startsolid;
			if ( trace.t1 < trace.t2 && trace.t1 >= 0.0f )
			{
				pTrace->fraction = trace.t1;
				math::vector_ma( pTrace->startpos, trace.t1, vecRayDelta, pTrace->endpos );
				pTrace->contents = CONTENTS_SOLID;
				pTrace->plane.normal = {};
				if ( trace.hitside >= 3 )
				{
					trace.hitside -= 3;
					pTrace->plane.dist = boxMaxs[ trace.hitside ];
					pTrace->plane.normal[ trace.hitside ] = 1.0f;
					pTrace->plane.type = trace.hitside;
				}
				else
				{
					pTrace->plane.dist = -boxMins[ trace.hitside ];
					pTrace->plane.normal[ trace.hitside ] = -1.0f;
					pTrace->plane.type = trace.hitside;
				}
				return true;
			}

			if ( pTrace->startsolid )
			{
				pTrace->allsolid = ( trace.t2 <= 0.0f ) || ( trace.t2 >= 1.0f );
				pTrace->fraction = 0;
				if ( pFractionLeftSolid )
				{
					*pFractionLeftSolid = trace.t2;
				}
				pTrace->endpos = pTrace->startpos;
				pTrace->contents = CONTENTS_SOLID;
				pTrace->plane.dist = pTrace->startpos[ 0 ];
				pTrace->plane.normal.Init( 1.0f, 0.0f, 0.0f );
				pTrace->plane.type = 0;
				pTrace->startpos = vecRayStart + ( vecRayDelta * trace.t2 );
				return true;
			}
		}

		return false;
	}
};

namespace poly_intersect
{
	typedef circular_buffer<Vector> convex_polygon;

	inline bool is_equal( const double d1, const double d2 )
	{
		constexpr double equity_tolerance = 0.001;
		return abs( d1 - d2 ) <= equity_tolerance;
	}

	inline bool inside_poly( const Vector& test, const convex_polygon& poly )
	{
		auto res = false;
		for ( auto i = 0, j = poly.size() - 1; i < poly.size(); j = i++ )
		{
			if ( ( poly[ i ].y > test.y ) != ( poly[ j ].y > test.y ) &&
				( test.x < ( poly[ j ].x - poly[ i ].x ) * ( test.y - poly[ i ].y ) / ( poly[ j ].y - poly[ i ].y ) + poly[ i ].x ) )
			{
				res = !res;
			}
		}
		return res;
	}

	inline std::optional<Vector> get_intersection( const Vector& l1p1, const Vector& l1p2, const Vector& l2p1, const Vector& l2p2 )
	{
		const double a1 = l1p2.y - l1p1.y;
		const double b1 = l1p1.x - l1p2.x;
		const double c1 = a1 * l1p1.x + b1 * l1p1.y;
		const double a2 = l2p2.y - l2p1.y;
		const double b2 = l2p1.x - l2p2.x;
		const double c2 = a2 * l2p1.x + b2 * l2p1.y;

		const auto det = a1 * b2 - a2 * b1;
		if ( is_equal( det, 0.0 ) )
			return {};

		else
		{
			const auto x = ( b2 * c1 - b1 * c2 ) / det;
			const auto y = ( a1 * c2 - a2 * c1 ) / det;
			const auto online1 = ( ( std::min( l1p1.x, l1p2.x ) < x || is_equal( std::min( l1p1.x, l1p2.x ), x ) )
				&& ( std::max( l1p1.x, l1p2.x ) > x || is_equal( std::max( l1p1.x, l1p2.x ), x ) )
				&& ( std::min( l1p1.y, l1p2.y ) < y || is_equal( std::min( l1p1.y, l1p2.y ), y ) )
				&& ( std::max( l1p1.y, l1p2.y ) > y || is_equal( std::max( l1p1.y, l1p2.y ), y ) ) );
			const auto online2 = ( ( std::min( l2p1.x, l2p2.x ) < x || is_equal( std::min( l2p1.x, l2p2.x ), x ) )
				&& ( std::max( l2p1.x, l2p2.x ) > x || is_equal( std::max( l2p1.x, l2p2.x ), x ) )
				&& ( std::min( l2p1.y, l2p2.y ) < y || is_equal( std::min( l2p1.y, l2p2.y ), y ) )
				&& ( std::max( l2p1.y, l2p2.y ) > y || is_equal( std::max( l2p1.y, l2p2.y ), y ) ) );

			if ( online1 && online2 )
				return Vector( x, y, 0.f );
		}
		return {};
	}

	inline convex_polygon get_intersections( const Vector& l1p1, const Vector& l1p2, const convex_polygon& poly )
	{
		convex_polygon intersection_vectors( poly.size() );
		for ( auto i = 0u; i < poly.size(); i++ )
		{
			const int next = i + 1 == poly.size() ? 0 : i + 1;
			if ( auto intersection = get_intersection( l1p1, l1p2, poly[ i ], poly[ next ] ); intersection.has_value() )
				intersection_vectors.emplace_back( *intersection );
		}
		return intersection_vectors;
	}

	inline void add_points( convex_polygon& points, const Vector& np )
	{
		auto found = false;
		for ( auto j = 0u; j < points.size(); j++ )
		{
			auto& p = points[ j ];
			if ( is_equal( p.x, np.x ) && is_equal( p.y, np.y ) )
			{
				found = true;
				break;
			}
		}

		if ( !found )
			points.emplace_back( np );
	}

	inline void add_points( convex_polygon& points, const convex_polygon& newpoints )
	{
		for ( auto i = 0u; i < newpoints.size(); i++ )
		{
			auto& np = newpoints[ i ];
			auto found = false;
			for ( auto j = 0u; j < points.size(); j++ )
			{
				auto& p = points[ j ];
				if ( is_equal( p.x, np.x ) && is_equal( p.y, np.y ) )
				{
					found = true;
					break;
				}
			}

			if ( !found )
				points.emplace_back( np );
		}
	}

	inline void order_clockwise( convex_polygon& points )
	{
		auto mx = 0.0;
		auto my = 0.0;
		for ( auto i = 0u; i < points.size(); i++ )
		{
			auto& p = points[ i ];
			mx += p.x;
			my += p.y;
		}
		mx /= points.size();
		my /= points.size();

		points.sort( [&] ( const Vector& p1, const Vector& p2 )
		{
			return atan2( p1.y - my, p1.x - mx ) > atan2( p2.y - my, p2.x - mx );
		} );
	}

	inline float area( const convex_polygon& poly )
	{
		auto area = 0.f;

		auto j = poly.size() - 1;
		for ( int i = 0; i < poly.size(); i++ )
		{
			area += ( poly[ j ].x + poly[ i ].x ) * ( poly[ j ].y - poly[ i ].y );
			j = i;
		}

		return fabsf( area * 0.5f );
	}

	inline convex_polygon get_intersection_poly( const convex_polygon& poly1, const convex_polygon& poly2 )
	{
		if ( poly1.size() < 3 )
			return {};

		convex_polygon clipped_corners( poly1.size() + poly2.size() + poly2.size() * poly1.size() );

		// p1 points inside p2
		for ( auto i = 0u; i < poly1.size(); i++ )
		{
			auto& p = poly1[ i ];
			if ( inside_poly( p, poly2 ) )
				add_points( clipped_corners, { p } );
		}

		// p2 points inside p1
		for ( auto i = 0u; i < poly2.size(); i++ )
		{
			auto& p = poly2[ i ];
			if ( inside_poly( p, poly1 ) )
				add_points( clipped_corners, { p } );
		}

		// intersections
		for ( auto i = 0u, next = 1u; i < poly1.size(); i++, next = ( i + 1 == poly1.size() ) ? 0 : i + 1 )
		{
			add_points( clipped_corners, get_intersections( poly1[ i ], poly1[ next ], poly2 ) );
		}

		order_clockwise( clipped_corners );
		return clipped_corners;
	}

	// ----------------- graham scan -------------------

	// returns -1 if a -> b -> c forms a counter-clockwise turn,
	// +1 for a clockwise turn, 0 if they are collinear
	inline int ccw( Vector a, Vector b, Vector c )
	{
		const int area = ( b.x - a.x ) * ( c.y - a.y ) - ( b.y - a.y ) * ( c.x - a.x );
		if ( area > 0 )
			return -1;

		if ( area < 0 )
			return 1;

		return 0;
	}

	inline void graham_scan( circular_buffer<Vector>& points )
	{
		std::stack<Vector> hull;

		if ( points.size() < 3 )
			return;

		// find the point having the least y coordinate (pivot),
		// ties are broken in favor of lower x coordinate
		auto operator_less = [] ( const Vector a, const Vector b ) -> bool
		{
			if ( a.y != b.y )
				return a.y < b.y;
			return a.x < b.x;
		};

		auto least_y = 0;
		for ( int i = 1; i < points.size(); i++ )
			if ( operator_less( points[ i ], points[ least_y ] ) )
				least_y = i;

		// swap the pivot with the first point
		const auto temp = points[ 0 ];
		points[ 0 ] = points[ least_y ];
		points[ least_y ] = temp;

		// Vector having the least y coordinate, used for sorting other points
		// according to polar angle about this point
		const auto pivot = points[ 0 ];
		points.pop_front();

		// sort the remaining point according to polar order about the pivot
		// used for sorting points according to polar order w.r.t the pivot
		points.sort( [&] ( const Vector& a, const Vector& b )
		{
			const auto order = ccw( pivot, a, b );
			if ( order == 0 )
				return pivot.DistToSqr( a ) < pivot.DistToSqr( b );
			return ( order == -1 );
		} );

		hull.push( pivot );
		hull.push( points[ 0 ] );
		hull.push( points[ 1 ] );

		for ( auto i = 2u; i < points.size(); i++ )
		{
			auto top = hull.top();
			hull.pop();
			while ( !hull.empty() && ccw( hull.top(), top, points[ i ] ) != -1 )
			{
				top = hull.top();
				hull.pop();
			}
			hull.push( top );
			hull.push( points[ i ] );
		}

		points.clear();

		while ( !hull.empty() )
		{
			auto p = hull.top();
			hull.pop();

			points.emplace_back( p );
		}
	}
}