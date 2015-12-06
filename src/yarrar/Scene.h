#pragma once

#include "Util.h"

#include <string>
#include <vector>

namespace yarrar {

struct Model
{
    std::string name;
    std::vector<float> vertices;
    Vector3 translation;
};

class Scene
{
public:

    void addModel(const Model& model);
    const std::vector<Model>& getModels() const;

private:
    std::vector<Model> m_models;

};

}