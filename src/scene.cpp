#include <feign/scene.h>

Scene::Scene() {
    sceneObjects = vector<SceneNode*>();
    root = nullptr;
}

Scene::~Scene() {
    delete root;
    sceneObjects.clear();
}

void Scene::addNode(SceneNode* node) {
    sceneObjects.push_back(node);
}

SceneNode* Scene::getRoot() const { return root; }
vector<SceneNode*> Scene::getSceneObjects() const { return sceneObjects; }
vector<Emitter*> Scene::getEmitters() const { return emitters; }
Texture* Scene::getEnvMap() const { return envMap; }
Integrator* Scene::getIntegrator() const { return integrator; }
Media* Scene::getEnvMedium() const { return envMedium; }
Sampler* Scene::getSampler() const { return sampler; }
