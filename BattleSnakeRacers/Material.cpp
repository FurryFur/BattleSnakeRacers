#include "Material.h"

GLuint Material::getUniformLocation(const std::string& uniformName) const
{
	auto searchResult = m_uniformLocationCache.find(uniformName);
	if (searchResult != m_uniformLocationCache.end())
		return searchResult->second;

	auto location = glGetUniformLocation(shader, uniformName.c_str());
	m_uniformLocationCache[uniformName] = location;
	return location;
}

GLuint Material::getUniformBlockIndex(const std::string& uniformBlockName) const
{
	auto searchResult = m_uniformBlockIndexCache.find(uniformBlockName);
	if (searchResult != m_uniformBlockIndexCache.end())
		return searchResult->second;


	auto location = glGetUniformLocation(shader, uniformBlockName.c_str());
	m_uniformBlockIndexCache[uniformBlockName] = location;
	return location;
}
