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
    std::cout << "world deconstructor" << std::endl;
    // does nothing for now
}

void WorldNode::renderAllScenes()
{
    std::vector<Scene*> scenes = collectScenes();

    for (int i = 0; i < scenes.size(); ++i)
    {
        std::cout << "scene: " << i << std::endl;
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

        std::cout << "rendering film: " << camera->getFilmSize()[0] << ", " << camera->getFilmSize()[1] << std::endl;

        // TODO: this will need to be changed for parallelization
        Imagef image = Imagef(camera->getFilmSize()[0],
                              camera->getFilmSize()[1],
                              3);

        integrator->render(scene,
                           camera,
                           sampler,
                           image);

        std::cout << "writing scene: " << scene->getSceneName() << std::endl;

        image.write(scene->getSceneName() + ".hdr");
        image.write(scene->getSceneName() + ".png");
        image.write(scene->getSceneName() + ".exr");
    }

    std::cout << "Rendering Complete" << std::endl;
}

void WorldNode::preProcess()
{
    preProcessChildren();
}

std::vector<Scene*> WorldNode::collectScenes() const
{
    std::vector<Scene*> scenes = std::vector<Scene*>();

    for (int i = 0; i < children.size(); ++i)
    {
        if (children[i]->getNodeType() == NT_Scene)
        {
            scenes.push_back((Scene*)children[i]);
        }
    }

    return scenes;
}

std::string WorldNode::getName() const
{
    return "world";
}

NodeType WorldNode::getNodeType() const
{
    return NT_World;
}
