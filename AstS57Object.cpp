#include "AstS57Object.h"

CNode::CNode(long lXposition, long lYposition, long lVposition, unsigned char ucType)
{
    m_lXposition = lXposition;
    m_lYposition = lYposition;
    m_lVposition = lVposition;

    m_ucType     = ucType;
}

CAstS57Vector::CAstS57Vector()
{
    m_NodesList.clear();
}

void CAstS57Vector::AddNode(long lXposition, long lYposition, long lVposition, unsigned char ucType)
{
    CNode Node(lXposition, lYposition, lVposition, ucType);
    m_NodesList.append(Node);
}

CAstS57Object::CAstS57Object()
{
    m_RelatedVectorIDsList.clear();
}
