#ifdef EVO_USE_GL3

#ifndef ADAPTER_GL3_4CA217FB1105496AB7731FEE2E02D2BC_H
#define ADAPTER_GL3_4CA217FB1105496AB7731FEE2E02D2BC_H

#include <ren/adapter.h>
#include <GL/glew.h>

namespace evo::ren
{
	enum glsl_version
	{
		glsl_120,
		glsl_130,
		glsl_300,
		glsl_410
	};

	class gl3_backup
	{
	public:
		gl3_backup()
		{
			glGetIntegerv(GL_ACTIVE_TEXTURE, reinterpret_cast<GLint*>(&active_tex));
			glActiveTexture(GL_TEXTURE0);

			glGetIntegerv(GL_CURRENT_PROGRAM, reinterpret_cast<GLint*>(&program));
			glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&texture));
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, reinterpret_cast<GLint*>(&array_buf));

#ifdef GL_POLYGON_MODE
			glGetIntegerv(GL_POLYGON_MODE, polygon_mode);
#endif

			glGetIntegerv(GL_VIEWPORT, viewport);
			glGetIntegerv(GL_SCISSOR_BOX, scissor_box);

			glGetIntegerv(GL_BLEND_SRC_RGB, reinterpret_cast<GLint*>(&blend_src_rgb));
			glGetIntegerv(GL_BLEND_DST_RGB, reinterpret_cast<GLint*>(&blend_dst_rgb));
			glGetIntegerv(GL_BLEND_SRC_ALPHA, reinterpret_cast<GLint*>(&blend_src_a));
			glGetIntegerv(GL_BLEND_DST_ALPHA, reinterpret_cast<GLint*>(&blend_dst_a));
			glGetIntegerv(GL_BLEND_EQUATION_RGB, reinterpret_cast<GLint*>(&blend_eq_rgb));
			glGetIntegerv(GL_BLEND_EQUATION_ALPHA, reinterpret_cast<GLint*>(&blend_eq_a));

			blend = glIsEnabled(GL_BLEND);
			cull_face = glIsEnabled(GL_CULL_FACE);
			depth_test = glIsEnabled(GL_DEPTH_TEST);
			stencil_test = glIsEnabled(GL_STENCIL_TEST);
			scissor_test = glIsEnabled(GL_SCISSOR_TEST);
		}

		~gl3_backup()
		{
			glUseProgram(program);
			glBindTexture(GL_TEXTURE_2D, texture);
			glActiveTexture(active_tex);

			glBindBuffer(GL_ARRAY_BUFFER, array_buf);
			glBlendEquationSeparate(blend_eq_rgb, blend_eq_a);
			glBlendFuncSeparate(blend_src_rgb, blend_dst_rgb, blend_src_a, blend_dst_a);

			blend ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
			cull_face ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
			depth_test ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
			stencil_test ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
			scissor_test ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);

#ifdef GL_POLYGON_MODE
			glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(polygon_mode[0]));
#endif

			glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
			glScissor(scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3]);
		}

	private:
		GLenum active_tex{};
		GLuint program{}, texture{}, array_buf{};

#ifdef GL_POLYGON_MODE
		GLint polygon_mode[2]{};
#endif

		GLint viewport[4]{}, scissor_box[4]{};
		GLenum blend_src_rgb{}, blend_dst_rgb{}, blend_src_a{}, blend_dst_a{}, blend_eq_rgb{}, blend_eq_a{};
		GLboolean blend{}, cull_face{}, depth_test{}, stencil_test{}, scissor_test{};
	};

	class adapter_gl3 : public adapter_base
	{
	public:
		adapter_gl3(const std::string& glsl_v, HWND wnd) : adapter_base(wnd), glsl_vs(glsl_v)
		{
			int v{130};
			sscanf_s(glsl_v.c_str(), "#version %d", &v);

			if (v < 130)
				glsl = glsl_120;
			else if (v >= 410)
				glsl = glsl_410;
			else if (v == 300)
				glsl = glsl_300;
			else
				glsl = glsl_130;

			name = "OpenGL 3";
		}

		void create_objects() override;
		void destroy_objects() override;
		void render() override;

		void * create_texture(void *arr, uint32_t w, uint32_t h, uint32_t p) override;
		void destroy_texture(void *tex) override;

	private:
		bool objects_created{};
		glsl_version glsl{};
		std::string glsl_vs{};

		GLuint pixel_shader{};
		GLuint vertex_shader{};
		GLuint frag_shader{};

		GLint loc_tex{};
		GLint loc_mtx{};
		GLuint loc_vtx{};
		GLuint loc_uv{};
		GLuint loc_color{};

		uint32_t vbo{};
		uint32_t elements{};

		GLuint pixel_texture{};

		bool verify_shader(GLuint h);
		bool verify_program(GLuint h);
	};
}

#endif //ADAPTER_GL3_4CA217FB1105496AB7731FEE2E02D2BC_H
#endif