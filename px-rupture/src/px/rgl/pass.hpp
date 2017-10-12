// name: pass.hpp
// type: c++ header
// auth: is0urce
// desc: renderpass state mashine abstraction

#pragma once

#include <GL/GL.h>

#include <initializer_list>
#include <vector>

namespace px {

	class pass
	{
	public:
		struct attachment
		{
			GLuint binding;
			GLuint element;
		};

	public:
		void swap(pass & that) noexcept
		{
			std::swap(m_init, that.m_init);
			std::swap(m_framebuffer, that.m_framebuffer);
			std::swap(m_vao, that.m_vao);
			std::swap(m_width, that.m_width);
			std::swap(m_height, that.m_height);
			std::swap(m_textures, that.m_textures);
			std::swap(m_uniforms, that.m_uniforms);
		}
		void output(GLuint framebuffer, GLsizei width, GLsizei height)
		{
			m_framebuffer = framebuffer;
			viewport(width, height);
		}
		void viewport(GLsizei width, GLsizei height) noexcept
		{
			m_width = width;
			m_height = height;
		}
		void bind_texture(GLuint texture, GLuint index)
		{
			if (texture == 0) throw std::runtime_error("px::gl_pass::bind_texture(..) - texture is 0");
			m_textures.push_back({ index, texture });
		}
		void bind_texture(GLuint texture)
		{
			bind_texture(texture, 0);
		}
		void bind_uniform(GLuint ubo, GLuint index)
		{
			m_uniforms.push_back({ index, ubo });
		}
		void bind_uniform(GLuint ubo)
		{
			m_uniforms.push_back({ 0, ubo });
		}
		void bind_textures(std::initializer_list<GLuint> && container)
		{
			GLuint index = 0;
			for (GLuint texture : container) {
				bind_texture(texture, index);
				++index;
			}
		}

	public:
		void bind()
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer);
			glViewport(0, 0, m_width, m_height);
			glScissor(0, 0, m_width, m_height);
			if (m_vao != 0) {
				glBindVertexArray(m_vao);
			}
			for (auto const& uniform : m_uniforms) {
				glBindBufferBase(GL_UNIFORM_BUFFER, uniform.binding, uniform.element);
			}
			for (auto const& texture : m_textures) {
				glActiveTexture(GL_TEXTURE0 + texture.binding);
				glBindTexture(GL_TEXTURE_2D, texture.element);
			}
		}
		void draw_arrays(GLenum mode, GLsizei count, GLint first)
		{
			bind();
			glDrawArrays(mode, first, count);
		}
		void draw_arrays(GLenum mode, GLsizei count)
		{
			draw_arrays(mode, count, 0);
		}

	public:
		~pass()
		{
		}
		pass() noexcept
			: m_framebuffer(0)
			, m_vao(0)
			, m_init(false)
			, m_width(0)
			, m_height(0)
		{
		}
		pass(GLuint framebuffer, GLuint vao, GLsizei width, GLsizei height) noexcept
			: pass()
		{
			m_framebuffer = framebuffer;
			m_vao = vao;
			m_init = true;
			viewport(width, height);
		}
		pass(pass const&) = delete;
		pass & operator=(pass const&) = delete;
		pass(pass && that) noexcept
			: pass()
		{
			swap(that);
		}
		pass & operator=(pass && that) noexcept
		{
			swap(that);
			return *this;
		}

	private:
		bool					m_init;
		GLuint					m_framebuffer;
		GLuint					m_vao;
		GLsizei					m_width;
		GLsizei					m_height;
		std::vector<attachment>	m_textures;
		std::vector<attachment>	m_uniforms;
	};
}