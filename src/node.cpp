#include <feign/node.h>

SceneNode::SceneNode()
{
    children = vector<SceneNode*>();
    parent = nullptr;
}

SceneNode::SceneNode(SceneNode* parent) : parent(parent)
{
    children = vector<SceneNode*>();
}

SceneNode::~SceneNode()
{
    for (int i = 0; i < children.size(); ++i) {
        delete children[i];
    }
    children.clear();
    parent = nullptr;
}

void SceneNode::addChild(SceneNode* node)
{
    children.push_back(node);
}

void SceneNode::setParent(SceneNode* node)
{
    if (parent) parent->removeChild(this);
    parent = node;

    if (!node->hasChild(this)) node->addChild(this);
}

bool SceneNode::hasChild(SceneNode* node)
{
    for (int i = 0; i < children.size(); i++)
    {
        if (children[i] == node) return true;
    }

    return false;
}

void SceneNode::removeChild(SceneNode* node)
{
    for (int i = 0; i < children.size(); i++)
    {
        if (children[i] == node)
        {
            int lastIndex = children.size() - 1;

            // swap for O(1) remove
            SceneNode* tmp = children[lastIndex];
            children[lastIndex] = node;
            children[i] = tmp;

            children.pop_back();

            if (node->parent) node->parent = nullptr;
        }
    }
}

int SceneNode::getTag() const
{
    return tag;
}
