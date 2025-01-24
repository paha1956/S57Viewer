#ifndef CASTS57PARSER_H
#define CASTS57PARSER_H

#include <QObject>
#include <QMap>

#include "AstS57Object.h"

struct RECORD_DATA
{
    unsigned long                 ulID;
    unsigned char                 ucType;
};

class CDRfield
{

private:

    QString                       m_sTag;
    int                           m_iLen;
    int                           m_iOffset;
    QByteArray                    m_RAWdata;

public:
    CDRfield();

    void SetTag(QString sTag){m_sTag = sTag;}
    void SetLen(int iLen){m_iLen = iLen;}
    void SetOffset(int iOffset){m_iOffset = iOffset;}
    void SetRAWdata(QByteArray RAWdata){m_RAWdata = RAWdata;}

    QString GetTag(void){return m_sTag;}
    int GetLen(void){return m_iLen;}
    int GetOffset(void){return m_iOffset;}
    QByteArray GetRAWdata(void){return m_RAWdata;}
};

class CDRrecord
{

private:

    QObject                      *m_pParent;

    unsigned long                 m_ulID;

    int                           m_iLen;
    int                           m_iDataOffset;
    int                           m_iLenFieldSize;
    int                           m_iOffsetFieldSize;
    int                           m_iTagFieldSize;

    QList <CDRfield>              m_FieldsList;

    int tag0001(QByteArray data);
    int tagDSPM(QByteArray data, struct RECORD_DATA *p_RecordData);
    int tagFRID(QByteArray data, struct RECORD_DATA *p_RecordData);
    int tagVRID(QByteArray data, struct RECORD_DATA *p_RecordData);
    int tagFSPT(QByteArray data, struct RECORD_DATA RecordData);
    int tagVRPT(QByteArray data, struct RECORD_DATA RecordData);
    int tagSG2D(QByteArray data, struct RECORD_DATA RecordData);
    int tagSG3D(QByteArray data, struct RECORD_DATA RecordData);

public:
    CDRrecord(QObject *pParent);

    void SetID(unsigned long ulID){m_ulID = ulID;}
    void SetLen(int iLen){m_iLen = iLen;}
    void SetDataOffset(int iDataOffset){m_iDataOffset = iDataOffset;}
    void SetLenFieldSize(int iLenFieldSize){m_iLenFieldSize = iLenFieldSize;}
    void SetOffsetFieldSize(int iOffsetFieldSize){m_iOffsetFieldSize = iOffsetFieldSize;}
    void SetTagFieldSize(int iTagFieldSize){m_iTagFieldSize = iTagFieldSize;}

    unsigned long GetID(void){return m_ulID;}
    int GetLen(void){return m_iLen;}
    int GetDataOffset(void){return m_iDataOffset;}
    int GetLenFieldSize(void){return m_iLenFieldSize;}
    int GetOffsetFieldSize(void){return m_iOffsetFieldSize;}
    int GetTagFieldSize(void){return m_iTagFieldSize;}

    QList <CDRfield> GetFieldsList(void){return m_FieldsList;}

    bool SetData(QByteArray RAWdata);
};

class CAstS57Parser : public QObject
{

    Q_OBJECT

private:

    QMap<unsigned long, CDRrecord>     m_DRrecords;

    QMap<unsigned long, CAstS57Vector> m_MapVectors;
    QMap<unsigned long, CAstS57Object> m_MapObjects;

    bool                          m_bMinMaxPositionDetect;
    long                          m_lMinXPosition;
    long                          m_lMaxXPosition;
    long                          m_lMinYPosition;
    long                          m_lMaxYPosition;

    // Данные DSPM
    unsigned long                 m_ulCOMF;
    unsigned long                 m_ulSOMF;
    QString                       m_Comment;

public:
    CAstS57Parser();
    CAstS57Parser(QByteArray RAWdata);

    void clear();

    void SetDataRecordsList(QByteArray RAWdata);

    void DefineMinMaxPosition(long ulXPosition, long ulYPosition);

    void SetRecord(CDRrecord record){m_DRrecords.insert(record.GetID(), record);}
    void SetVector(CAstS57Vector vector){m_MapVectors.insert(vector.GetID(), vector);}
    void SetObject(CAstS57Object object){m_MapObjects.insert(object.GetID(), object);}

    void SetCOMF(unsigned long ulCOMF){m_ulCOMF = ulCOMF;}
    void SetSOMF(unsigned long ulSOMF){m_ulSOMF = ulSOMF;}
    void SetComment(QByteArray data){m_Comment.clear(); m_Comment.append(QString::fromLocal8Bit(data));}

    void SetMinXPosition(long lXPosition){m_lMinXPosition = lXPosition;}
    void SetMaxXPosition(long lXPosition){m_lMaxXPosition = lXPosition;}
    void SetMinYPosition(long lYPosition){m_lMinYPosition = lYPosition;}
    void SetMaxYPosition(long lYPosition){m_lMaxYPosition = lYPosition;}

    QMap<unsigned long, CDRrecord> GetRecordsList(void){return m_DRrecords;}

    QMap<unsigned long, CAstS57Vector> *GetVectorsList(void){return &m_MapVectors;}
    QMap<unsigned long, CAstS57Object> *GetObjectsList(void){return &m_MapObjects;}
    CAstS57Vector *GetVector(int iIndex){return &(m_MapVectors[iIndex]);}
    CAstS57Object *GetObject(int iIndex){return &(m_MapObjects[iIndex]);}

    unsigned long GetCOMF(void){return m_ulCOMF;}
    unsigned long GetSOMF(void){return m_ulSOMF;}
    QString GetComment(void){return m_Comment;};

    long GetMinXPosition(void){return m_lMinXPosition;}
    long GetMaxXPosition(void){return m_lMaxXPosition;}
    long GetMinYPosition(void){return m_lMinYPosition;}
    long GetMaxYPosition(void){return m_lMaxYPosition;}

};


#endif // CASTS57PARSER_H
