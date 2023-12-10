#ifdef EVO_USE_GL3

#include <ren/adapter_gl3.h>
#include <ren/command.h>
#include <iostream>

using namespace evo::ren;

void adapter_gl3::create_objects()
{
	adapter_base::create_objects();
	if (objects_created)
		return;

	GLint last_tex{}, last_array_buf{};
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_tex);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buf);

	const GLchar* vs_120 = "\nuniform mat4 ProjMtx;attribute vec2 Position;attribute vec2 UV;attribute vec4 Color;varying vec2 Frag_UV;varying vec4 Frag_Color;void main(){Frag_UV=UV;Frag_Color=Color;gl_Position=ProjMtx*vec4(Position.xy,0,1);}";
	const GLchar* vs_130 = "\nuniform mat4 ProjMtx;in vec2 Position;in vec2 UV;in vec4 Color;out vec2 Frag_UV;out vec4 Frag_Color;void main(){Frag_UV = UV;Frag_Color = Color;gl_Position=ProjMtx*vec4(Position.xy,0,1);}";
	const GLchar* vs_300 = "\nprecision mediump float;layout(location=0) in vec2 Position;layout(location=1) in vec2 UV;layout(location=2) in vec4 Color;uniform mat4 ProjMtx;out vec2 Frag_UV;out vec4 Frag_Color;void main(){Frag_UV=UV;Frag_Color=Color;gl_Position=ProjMtx*vec4(Position.xy,0,1);}";
	const GLchar* vs_410 = "\nlayout(location=0) in vec2 Position;layout(location=1) in vec2 UV;layout(location=2) in vec4 Color;uniform mat4 ProjMtx;out vec2 Frag_UV;out vec4 Frag_Color;void main(){Frag_UV=UV;Frag_Color=Color;gl_Position=ProjMtx*vec4(Position.xy,0,1);}";

	const GLchar* fs_120 = "\n#ifdef GL_ES\nprecision mediump float;\n#endif\nuniform sampler2D Texture;varying vec2 Frag_UV;varying vec4 Frag_Color;void main(){gl_FragColor=Frag_Color*texture2D(Texture,Frag_UV.st);}";
	const GLchar* fs_130 = "\nuniform sampler2D Texture;in vec2 Frag_UV;in vec4 Frag_Color;out vec4 Out_Color;void main(){Out_Color=Frag_Color*texture(Texture,Frag_UV.st);}";
	const GLchar* fs_300 = "\nprecision mediump float;uniform sampler2D Texture;in vec2 Frag_UV;in vec4 Frag_Color;layout(location=0) out vec4 Out_Color;void main(){Out_Color=Frag_Color*texture(Texture,Frag_UV.st);}";
	const GLchar* fs_410 = "\nin vec2 Frag_UV;in vec4 Frag_Color;uniform sampler2D Texture;layout(location=0) out vec4 Out_Color;void main(){Out_Color=Frag_Color*texture(Texture,Frag_UV.st);}";

	const GLchar* vs{};
	const GLchar* fs{};

	switch (glsl)
	{
		case glsl_120:
			vs = vs_120;
			fs = fs_120;
			break;
		case glsl_130:
			vs = vs_130;
			fs = fs_130;
			break;
		case glsl_300:
			vs = vs_300;
			fs = fs_300;
			break;
		case glsl_410:
			vs = vs_410;
			fs = fs_410;
			break;
	}

	const std::string vs_f = glsl_vs + vs;
	const auto vs_fs = vs_f.c_str();

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vs_fs, nullptr);
	glCompileShader(vertex_shader);

	if (!verify_shader(vertex_shader))
		throw std::runtime_error("Failed to compile vertex shader");

	const std::string fs_f = glsl_vs + fs;
	const auto fs_fs = fs_f.c_str();

	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &fs_fs, nullptr);
	glCompileShader(frag_shader);

	if (!verify_shader(frag_shader))
		throw std::runtime_error("Failed to compile fragment shader");

	pixel_shader = glCreateProgram();
	glAttachShader(pixel_shader, vertex_shader);
	glAttachShader(pixel_shader, frag_shader);
	glLinkProgram(pixel_shader);

	if (!verify_program(pixel_shader))
		throw std::runtime_error("Failed to link pixel shader");

	loc_tex = glGetUniformLocation(pixel_shader, "Texture");
	loc_mtx = glGetUniformLocation(pixel_shader, "ProjMtx");
	loc_vtx = static_cast<GLuint>(glGetAttribLocation(pixel_shader, "Position"));
	loc_uv = static_cast<GLuint>(glGetAttribLocation(pixel_shader, "UV"));
	loc_color = static_cast<GLuint>(glGetAttribLocation(pixel_shader, "Color"));

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &elements);

	glGenTextures(1, &pixel_texture);
	glBindTexture(GL_TEXTURE_2D, pixel_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#ifdef GL_UNPACK_ROW_LENGTH
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif

	uint32_t pixel{0xFFFFFFFF};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);

	glBindTexture(GL_TEXTURE_2D, last_tex);
	glBindBuffer(GL_ARRAY_BUFFER, last_array_buf);

	objects_created = true;
}

bool adapter_gl3::verify_shader(GLuint h)
{
	GLint status{}, log_length{};
	glGetShaderiv(h, GL_COMPILE_STATUS, &status);
	glGetShaderiv(h, GL_INFO_LOG_LENGTH, &log_length);

	if (log_length > 1)
	{
		std::vector<char> buf{};
		buf.resize(log_length + 1);

		glGetShaderInfoLog(h, log_length, nullptr, buf.data());
		std::cout << buf.data() << std::endl;
	}

	return static_cast<GLboolean>(status) != GL_FALSE;
}

bool adapter_gl3::verify_program(GLuint h)
{
	GLint status{}, log_length{};
	glGetProgramiv(h, GL_LINK_STATUS, &status);
	glGetProgramiv(h, GL_INFO_LOG_LENGTH, &log_length);

	if (log_length > 1)
	{
		std::vector<char> buf{};
		buf.resize(log_length + 1);

		glGetProgramInfoLog(h, log_length, nullptr, buf.data());
		std::cout << buf.data() << std::endl;
	}

	return static_cast<GLboolean>(status) != GL_FALSE;
}

void adapter_gl3::destroy_objects()
{
	adapter_base::destroy_objects();
	if (!objects_created)
		return;

	if (vbo)
		glDeleteBuffers(1, &vbo);
	if (elements)
		glDeleteBuffers(1, &elements);
	if (pixel_shader && vertex_shader)
		glDetachShader(pixel_shader, vertex_shader);
	if (pixel_shader && frag_shader)
		glDetachShader(pixel_shader, frag_shader);
	if (vertex_shader)
		glDeleteShader(vertex_shader);
	if (frag_shader)
		glDeleteShader(frag_shader);
	if (pixel_shader)
		glDeleteProgram(pixel_shader);
	if (pixel_texture)
		glDeleteTextures(1, &pixel_texture);
}

void adapter_gl3::render()
{
	adapter_base::render();
	if (!objects_created)
		create_objects();

	gl3_backup _backup{}; (void)_backup;

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_SCISSOR_TEST);

#ifdef GL_POLYGON_MODE
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

#ifdef GL_CLIP_ORIGIN
	auto is_ll_origin{true};

	GLenum current_origin{};
	glGetIntegerv(GL_CLIP_ORIGIN, reinterpret_cast<GLint*>(&current_origin));
	if (current_origin == GL_UPPER_LEFT)
		is_ll_origin = false;
#endif

	glViewport(0, 0, display.x, display.y);
	glScissor(0, 0, display.x, display.y);

	auto m_t = 0.f;
	auto m_b = display.y;

#ifdef GL_CLIP_ORIGIN
	if (!is_ll_origin)
		std::swap(m_t, m_b);
#endif

	const float mtx[4][4] = {
					{ 2.0f / display.x, 0.0f, 0.0f, 0.0f },
					{ 0.0f, 2.0f / (m_t - m_b), 0.0f, 0.0f },
					{ 0.0f, 0.0f, -1.0f, 0.0f },
					{ display.x / -display.x, (m_t + m_b) / (m_b - m_t), 0.0f, 1.0f },
	};

	glUseProgram(pixel_shader);
	glUniform1i(loc_tex, 0);
	glUniformMatrix4fv(loc_mtx, 1, GL_FALSE, &mtx[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);

	glVertexAttribPointer(loc_vtx, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(0x00));
	glVertexAttribPointer(loc_uv, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),  reinterpret_cast<void*>(0x08));
	glVertexAttribPointer(loc_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertex), reinterpret_cast<void*>(0x10));
	glEnableVertexAttribArray(loc_vtx);
	glEnableVertexAttribArray(loc_uv);
	glEnableVertexAttribArray(loc_color);

	draw.for_each_layer([&](layer& l) {
		glBufferData(GL_ARRAY_BUFFER, l.vertices.size() * sizeof(vertex), l.vertices.data(), GL_STREAM_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, l.indices.size() * sizeof(uint16_t), l.indices.data(), GL_STREAM_DRAW);

		uint32_t v_offset{}, i_offset{};
		for (const auto& c : l.commands)
		{
			glScissor(c.clip.mins.x, c.clip.mins.y, c.clip.width(), c.clip.height());
			glBindTexture(GL_TEXTURE_2D, c.texture ? reinterpret_cast<GLuint>(c.texture) : pixel_texture);
			glDrawElementsBaseVertex(GL_TRIANGLES, c.indices, GL_UNSIGNED_SHORT, reinterpret_cast<void*>(i_offset * sizeof(uint16_t)), v_offset);

			i_offset += c.indices;
			v_offset += c.vertices;
		}
	});
}

void* adapter_gl3::create_texture(void* arr, uint32_t w, uint32_t h, uint32_t p)
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

void adapter_gl3::destroy_texture(void* tex)
{
	auto t = reinterpret_cast<GLuint>(tex);
	glDeleteTextures(1, &t);
}

#endif