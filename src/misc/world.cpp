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
    cout << "Starting Rendering Process" << endl;

    vector<Scene*> scenes = collectScenes();

    for (int i = 0; i < scenes.size(); ++i)
    {
        cout << "scene: " << i << endl;
        Scene* scene = scenes[i];

        Integrator* integrator = scene->getIntegrator();
        Camera* camera = scene->getCamera();
        Sampler* sampler = scene->getSampler();

        if (!integrator)
        {
            throw new MissingPrimitiveException("no specified integrator");
        }
        if (!camera)
        {
            throw new MissingPrimitiveException("no specified camera");
        }
        if (!sampler)
        {
            throw new MissingPrimitiveException("no specified sampler");
        }

        // TODO: this will need to be changed for parallelization
        Imagef image = Imagef(camera->getFilmSize()[0],
                              camera->getFilmSize()[1],
                              3);

        cout << "pre Render" << endl;
        integrator->render(scene,
                           camera,
                           sampler,
                           image);
        cout << "post Render" << endl;

        image.write(scene->getSceneName());
    }

    cout << "Rendering Complete" << endl;
}

void WorldNode::preProcess()
{
    preProcessChildren();
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
