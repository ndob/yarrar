#include "Scene.h"

namespace yarrar {

void Scene::addModel(const Model& model)
{
    m_models.push_back(model);
}

const std::vector<Model>& Scene::getModels() const
{
    return m_models;
}

}