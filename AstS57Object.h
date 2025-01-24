#ifndef CASTS57OBJECT_H
#define CASTS57OBJECT_H

#include <QObject>

class CNode
{

private:

    long                          m_lXposition;
    long                          m_lYposition;
    long                          m_lVposition;

    unsigned char                 m_ucType;

public:

    CNode(long lXposition, long lYposition, long lVposition, unsigned char ucType);

    long GetXPosition(void){return m_lXposition;}
    long GetYPosition(void){return m_lYposition;}
    long GetVPosition(void){return m_lVposition;}

    unsigned char GetType(void){return m_ucType;}
};

class CAstS57Vector
{

private:

    unsigned long                 m_ulID;

    unsigned long                 m_ulBegVectorID;
    unsigned long                 m_ulEndVectorID;

    QList<CNode>                  m_NodesList;

public:
    CAstS57Vector();

    void SetID(unsigned long ulID){m_ulID = ulID;}
    void SetBegVectorID(unsigned long ulID){m_ulBegVectorID = ulID;}
    void SetEndVectorID(unsigned long ulID){m_ulEndVectorID = ulID;}

    unsigned long GetID(void){return m_ulID;}
    unsigned long GetBegVectorID(void){return m_ulBegVectorID;}
    unsigned long GetEndVectorID(void){return m_ulEndVectorID;}

    int GetNodesNum(void){return m_NodesList.size();}
    QList<CNode> GetNodesList(void){return m_NodesList;}

    void AddNode(long lXposition, long lYposition, long lVposition, unsigned char ucType);
};

class CAstS57Object
{

private:

    unsigned long                 m_ulID;

    unsigned short                m_usObjectClass;
    unsigned short                m_usObjectPrimitiveType;
    QList<unsigned long>          m_RelatedVectorIDsList;

public:
    CAstS57Object();

    void SetID(unsigned long ulID){m_ulID = ulID;}
    void SetObjectClass(unsigned short usOblectClass){m_usObjectClass = usOblectClass;}
    void SetObjectPrimitiveType(unsigned short usOblectPrimitiveType){m_usObjectPrimitiveType = usOblectPrimitiveType;}
    void SetRelatedObjectID(unsigned long ulRelatedVectorID){m_RelatedVectorIDsList.append(ulRelatedVectorID);}

    unsigned long GetID(void){return m_ulID;}
    unsigned short GetObjectClass(void){return m_usObjectClass;}
    unsigned short GetObjectPrimitiveType(void){return m_usObjectPrimitiveType;}
    QList<unsigned long> *GetRelatedObjectIDList(void){return &m_RelatedVectorIDsList;}
};

#endif // CASTS57OBJECT_H
