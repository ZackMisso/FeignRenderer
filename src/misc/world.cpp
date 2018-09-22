#include <feign/misc/world.h>

WorldNode::WorldNode() : Node()
{
    // does nothing for now
}

WorldNode::WorldNode(Node* parent) : Node(parent)
{
    // does nothing for now
}

WorldNode::~WorldNode()
{
    // does nothing for now
}

void WorldNode::renderAllScenes()
{
    cout << "Starting the render" << endl;

    vector<Scene*> scenes = collectScenes();

    for (int i = 0; i < scenes.size(); ++i)
    {
        Scene* scene = scenes[i];
        scene->preProcess();
        cout << "Scene Children: " << scene->numChildren() << endl;

        Integrator* integrator = scene->getIntegrator();
        Camera* camera = scene->getCamera();
        Sampler* sampler = scene->getSampler();

        if (!integrator) cout << "NO INTEGRATOR" << endl;
        if (!camera) cout << "NO CAMERA" << endl;
        if (!sampler) cout << "NO SAMPLER" << endl;

        // TODO
    }

    cout << "Rendering Complete" << endl;
}

vector<Scene*> WorldNode::collectScenes() const
{
    vector<Scene*> scenes = vector<Scene*>();

    for (int i = 0; i < children.size(); ++i)
    {
        if (children[i]->getNodeType() == NT_Scene)
        {
            scenes.push_back((Scene*)children[i]);
        }
    }

    return scenes;
}

string WorldNode::getName() const
{
    return "world";
}

NodeType WorldNode::getNodeType() const
{
    return NT_World;
}
