#ifdef EVO_USE_GL2

#ifndef ADAPTER_GL3_4CA217FB1105496AB7731FEE2E02D2BC_H
#define ADAPTER_GL3_4CA217FB1105496AB7731FEE2E02D2BC_H

#include <ren/adapter.h>
#include <GL/glew.h>

namespace evo::ren
{
	class gl2_backup
	{
	public:
		gl2_backup()
		{
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &texture);
			glGetIntegerv(GL_POLYGON_MODE, polygon_mode);
			glGetIntegerv(GL_VIEWPORT, viewport);
			glGetIntegerv(GL_SCISSOR_BOX, scissor_box);
			glGetIntegerv(GL_SHADE_MODEL, &shade_model);

			glGetTexEnviv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &tex_env_mode);
		}

		~gl2_backup()
		{
			glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(texture));

			glPolygonMode(GL_FRONT, (GLenum)polygon_mode[0]);
			glPolygonMode(GL_BACK, (GLenum)polygon_mode[1]);

			glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
			glScissor(scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3]);
			glShadeModel(shade_model);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, tex_env_mode);
		}

	private:
		GLint texture{}, polygon_mode[2]{}, viewport[4]{}, scissor_box[4]{}, shade_model{}, tex_env_mode{};
	};

	class adapter_gl2 : public adapter_base
	{
	public:
		adapter_gl2(HWND wnd) : adapter_base(wnd)
		{
			name = "OpenGL 2";
		}

		void create_objects() override;
		void destroy_objects() override;
		void render() override;

		void * create_texture(void *arr, uint32_t w, uint32_t h, uint32_t p) override;
		void destroy_texture(void *tex) override;

	private:
		bool objects_created{};

		GLuint pixel_texture{};
	};
}

#endif //ADAPTER_GL3_4CA217FB1105496AB7731FEE2E02D2BC_H
#endif