#include <feign/node.h>
#include <feign/exceptions.h>

Node::Node()
{
    children = vector<Node*>();
    primitives = new PrimitiveList();
    parent = nullptr;
}

Node::Node(Node* parent) : parent(parent)
{
    children = vector<Node*>();
    primitives = new PrimitiveList();
}

Node::~Node()
{
    delete primitives;
    for (int i = 0; i < children.size(); ++i) {
        delete children[i];
    }
    children.clear();
    parent = nullptr;
}

void Node::addChild(Node* node)
{
    children.push_back(node);
}

void Node::setParent(Node* node)
{
    if (parent) parent->removeChild(this);
    parent = node;

    if (!node->hasChild(this)) node->addChild(this);
}

bool Node::hasChild(Node* node)
{
    for (int i = 0; i < children.size(); i++)
    {
        if (children[i] == node) return true;
    }

    return false;
}

void Node::removeChild(Node* node)
{
    for (int i = 0; i < children.size(); i++)
    {
        if (children[i] == node)
        {
            int lastIndex = children.size() - 1;

            // swap for O(1) remove
            Node* tmp = children[lastIndex];
            children[lastIndex] = node;
            children[i] = tmp;

            children.pop_back();

            if (node->parent) node->parent = nullptr;
        }
    }
}

void Node::printGraph() const
{
    throw new NotImplementedException("SceneNode::printGraph");

    // TODO
}

int Node::getTag() const
{
    return tag;
}

PrimitiveList* Node::getPrimList() const
{
    return primitives;
}
