#include "PointLight.h"

namespace e186
{

	PointLight::PointLight(glm::vec3 color, const glm::vec3& position)
		//: m_transform( position ),
		: m_position(position),
		m_light_color(std::move(color)),
		m_attenuation(1.0f, 0.1f, 0.01f, 0.001f),
		m_enabled{ true }
	{
	}

	PointLight::PointLight(glm::vec3 color, const glm::vec3& position, float const_atten, float lin_atten, float quad_atten, float cub_atten)
		//: m_transform( position ),
		: m_position(position),
		m_light_color(std::move(color)),
		m_attenuation(const_atten, lin_atten, quad_atten, cub_atten),
		m_enabled{ true }
	{
	}

	PointLight::PointLight(glm::vec3 color, Transform transform, float const_atten, float lin_atten, float quad_atten, float cub_atten)
		//: m_transform( position ),
		: m_position(transform.GetPosition()),
		m_light_color(std::move(color)),
		m_attenuation(const_atten, lin_atten, quad_atten, cub_atten),
		m_enabled{ true }
	{
	}

	PointLight::~PointLight()
	{
	}

	void PointLight::set_position(glm::vec3 position)
	{
		m_position = std::move(position);
	}

	void PointLight::set_light_color(glm::vec3 color)
	{
		m_light_color = std::move(color);
	}

	void PointLight::set_attenuation(glm::vec4 attenuation)
	{
		m_attenuation = std::move(attenuation);
	}

	void PointLight::set_const_attenuation(float attenuation)
	{
		m_attenuation = glm::vec4(attenuation, m_attenuation[1], m_attenuation[2], m_attenuation[3]);
	}

	void PointLight::set_linear_attenuation(float attenuation)
	{
		m_attenuation = glm::vec4(m_attenuation[0], attenuation, m_attenuation[2], m_attenuation[3]);
	}

	void PointLight::set_quadratic_attenuation(float attenuation)
	{
		m_attenuation = glm::vec4(m_attenuation[0], m_attenuation[1], attenuation, m_attenuation[3]);
	}

	void PointLight::set_cubic_attenuation(float attenuation)
	{
		m_attenuation = glm::vec4(m_attenuation[0], m_attenuation[1], m_attenuation[4], attenuation);
	}

	void PointLight::set_enabled(bool is_enabled)
	{
		m_enabled = is_enabled;
	}
}
