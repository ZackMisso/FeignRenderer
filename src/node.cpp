#include <feign/node.h>
#include <feign/exceptions.h>

Node::Node()
{
    children = std::vector<Node*>();
    parent = nullptr;
}

Node::Node(Node* parent) : parent(parent)
{
    children = std::vector<Node*>();
}

Node::~Node()
{
    std::cout << "clearing " << std::endl;
    for (int i = 0; i < children.size(); ++i)
    {
        delete children[i];
    }
    children.clear();
    std::cout << "cleared" << std::endl;
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

void Node::preProcessChildren()
{
    for (int i = 0; i < children.size(); ++i)
    {
        children[i]->preProcess();
    }
}

void Node::printGraph(std::string indent) const
{
    std::cout << indent << getName() << std::endl;
    std::string newIndent = indent + "  ";

    for (int i = 0; i < children.size(); ++i)
    {
        children[i]->printGraph(newIndent);
    }
}

int Node::numChildren() const
{
    return children.size();
}

int Node::getTag() const
{
    return tag;
}
