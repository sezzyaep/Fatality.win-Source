#pragma once

class OverlayText_t;

class IVDebugOverlay
{
public:
	virtual void            AddEntityTextOverlay( int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char* format, ... ) = 0;
	virtual void            AddBoxOverlay( const Vector& origin, const Vector& mins, const Vector& max, QAngle const& orientation, int r, int g, int b, int a, float duration ) = 0;
	virtual void            AddSphereOverlay( const Vector& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration ) = 0;
	virtual void            AddTriangleOverlay( const Vector& p1, const Vector& p2, const Vector& p3, int r, int g, int b, int a, bool noDepthTest, float duration ) = 0;
	virtual void            AddLineOverlay( const Vector& origin, const Vector& dest, int r, int g, int b, bool noDepthTest, float duration ) = 0;
	virtual void            AddTextOverlay( const Vector& origin, float duration, const char* format, ... ) = 0;
	virtual void            AddTextOverlay( const Vector& origin, int line_offset, float duration, const char* format, ... ) = 0;
	virtual void            AddScreenTextOverlay( float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char* text ) = 0;
	virtual void            AddSweptBoxOverlay( const Vector& start, const Vector& end, const Vector& mins, const Vector& max, const QAngle& angles, int r, int g, int b, int a, float flDuration ) = 0;
	virtual void            AddGridOverlay( const Vector& origin ) = 0;
	virtual void            AddCoordFrameOverlay( const matrix3x4_t& frame, float flScale, int vColorTable[ 3 ][ 3 ] = NULL ) = 0;
	virtual int             ScreenPosition( const Vector& point, Vector& screen ) = 0;
	virtual int             ScreenPosition( float flXPos, float flYPos, Vector& screen ) = 0;
	virtual OverlayText_t* GetFirst( void ) = 0;
	virtual OverlayText_t* GetNext( OverlayText_t* current ) = 0;
	virtual void            ClearDeadOverlays( void ) = 0;
	virtual void            ClearAllOverlays( void ) = 0;
	virtual void            AddTextOverlayRGB( const Vector& origin, int line_offset, float duration, float r, float g, float b, float alpha, const char* format, ... ) = 0;
	virtual void            AddTextOverlayRGB( const Vector& origin, int line_offset, float duration, int r, int g, int b, int a, const char* format, ... ) = 0;
	virtual void            AddLineOverlayAlpha( const Vector& origin, const Vector& dest, int r, int g, int b, int a, bool noDepthTest, float duration ) = 0;
	virtual void            AddBoxOverlay2( const Vector& origin, const Vector& mins, const Vector& max, QAngle const& orientation, const uint8_t* faceColor, const uint8_t* edgeColor, float duration ) = 0;
	virtual void			AddLineOverlay( const Vector& origin, const Vector& dest, int r, int g, int b, int a, float, float ) = 0;
	virtual void            PurgeTextOverlays() = 0;
	virtual void			AddCapsuleOverlay( const Vector& mins, const Vector& maxs, const float& radius, int r, int g, int b, int a, float duration, bool a1 = false, bool skip_occlusion = true ) = 0;

	static void RenderBox( const Vector vOrigin, const Vector vMins, const Vector vMaxs, const QAngle angles, Color c, bool bZBuffer, IMaterial* override_material = nullptr )
	{
		reinterpret_cast< void( __stdcall* )( ) >( make_offset_multi( "engine.dll", sig_init_materials ) )( );
		reinterpret_cast< void( __cdecl* )( const Vector&, const QAngle&, const Vector&, const Vector&, Color, IMaterial*, bool ) >( make_offset( "engine.dll", sig_render_box ) )( vOrigin, angles, vMins, vMaxs, c, override_material ? override_material : *reinterpret_cast< IMaterial** >( make_offset_multi( "engine.dll", sig_vertex_color_ignorez ) ), false );
	}

	static void RenderTriangle( const Vector& p1, const Vector& p2, const Vector& p3, Color c, bool noDepthTest, IMaterial* override_material = nullptr )
	{
		reinterpret_cast< void( __stdcall* )( ) >( make_offset_multi( "engine.dll", sig_init_materials ) )( );
		reinterpret_cast< void( __cdecl* )( const Vector&, const Vector&, const Vector&, Color, IMaterial* ) >( make_offset( "engine.dll", sig_render_triangle ) )( p1, p2, p3, c, override_material ? override_material : *reinterpret_cast< IMaterial** >( make_offset_multi( "engine.dll", sig_vertex_color_ignorez ) ) );
	}

	static void Circle( const Vector& position, const Vector& xAxis, const Vector& yAxis, float radius, Color c, bool bNoDepthTest )
	{
		constexpr unsigned int nSegments = 64;
		constexpr float flRadStep = ( M_PI * 2.0f ) / static_cast< float >( nSegments );

		Vector vecLastPosition{};

		// Find our first position
		// Retained for triangle fanning
		Vector vecStart = position + xAxis * radius;
		Vector vecPosition = vecStart;

		// Draw out each segment (fanning triangles if we have an alpha amount)
		for ( int i = 1; i <= nSegments; i++ )
		{
			// Store off our last position
			vecLastPosition = vecPosition;

			// Calculate the new one
			float flSin, flCos;
			math::sin_cos( flRadStep * i, &flSin, &flCos );
			vecPosition = position + ( xAxis * flCos * radius ) + ( yAxis * flSin * radius );

			// If we have an alpha value, then draw the fan
			if ( c.a() && i > 1 )
			{
				RenderTriangle( vecStart, vecLastPosition, vecPosition, c, bNoDepthTest );
			}
		}
	}

	static void Circle( const Vector& position, const QAngle& angles, float radius, Color c, bool bNoDepthTest )
	{
		// Setup our transform matrix
		matrix3x4_t xform;
		math::angle_matrix( angles, position, xform );
		Vector xAxis{}, yAxis{};
		// default draws circle in the y/z plane
		math::matrix_get_column( xform, 2, xAxis );
		math::matrix_get_column( xform, 1, yAxis );
		Circle( position, xAxis, yAxis, radius, c, bNoDepthTest );
	}
};
