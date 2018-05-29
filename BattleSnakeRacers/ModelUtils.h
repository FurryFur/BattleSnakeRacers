#pragma once

#include <string>

struct ModelComponent;

namespace ModelUtils {

	// Load a model into a model component from file.
	// The noCache parameter can be used to reload a model from scratch instead
	// or using a model that has already been cached.
	ModelComponent loadModel(const std::string& path);
}