#ifdef EVO_USE_GL2

#include <ren/adapter_gl2.h>
#include <ren/command.h>
#include <iostream>

using namespace evo::ren;

void adapter_gl2::create_objects()
{
	adapter_base::create_objects();
	if (objects_created)
		return;

	GLint last_tex{};
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_tex);

	glGenTextures(1, &pixel_texture);
	glBindTexture(GL_TEXTURE_2D, pixel_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

	uint32_t pixel{0xFFFFFFFF};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);

	glBindTexture(GL_TEXTURE_2D, last_tex);
	objects_created = true;
}

void adapter_gl2::destroy_objects()
{
	adapter_base::destroy_objects();
	if (!objects_created)
		return;

	if (pixel_texture)
		glDeleteTextures(1, &pixel_texture);
}

void adapter_gl2::render()
{
	adapter_base::render();
	if (!objects_created)
		create_objects();

	gl2_backup _backup{}; (void)_backup;

	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_SCISSOR_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glViewport(0, 0, display.x, display.y);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, display.x, display.y, 0, -1.f, 1.f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	draw.for_each_layer([&](layer& l) {
		auto buf = reinterpret_cast<uint8_t*>(l.vertices.data());

		uint32_t i_offset{};
		for (const auto& c : l.commands)
		{
			glVertexPointer(2, GL_FLOAT, sizeof(vertex), buf);
			glTexCoordPointer(2, GL_FLOAT, sizeof(vertex), buf + 0x08);
			glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vertex), buf + 0x10);

			glScissor(c.clip.mins.x, display.y - c.clip.maxs.y, c.clip.width(), c.clip.height());
			glBindTexture(GL_TEXTURE_2D, c.texture ? reinterpret_cast<GLuint>(c.texture) : pixel_texture);
			glDrawElements(GL_TRIANGLES, c.indices, GL_UNSIGNED_SHORT, l.indices.data() + i_offset);

			buf += c.vertices * sizeof(vertex);
			i_offset += c.indices;
		}
	});

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

void* adapter_gl2::create_texture(void* arr, uint32_t w, uint32_t h, uint32_t p)
{
	GLuint last_tex{}, tex{};

	glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&last_tex));

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, arr);

	glBindTexture(GL_TEXTURE_2D, last_tex);
	return reinterpret_cast<void*>(tex);
}

void adapter_gl2::destroy_texture(void* tex)
{
	auto t = reinterpret_cast<GLuint>(tex);
	glDeleteTextures(1, &t);
}

#endif