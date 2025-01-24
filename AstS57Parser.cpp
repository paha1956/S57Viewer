#include "AstS57Parser.h"
#include "AstS57Format.h"

#include "AstS57ObjectClasses.h"

CDRfield::CDRfield(void)
{
}

CDRrecord::CDRrecord(QObject *pParent)
{
    m_pParent = pParent;
}

bool CDRrecord::SetData(QByteArray RAWdata)
{
    if (m_pParent == nullptr) return false;

    bool error;

    m_iLen             = RAWdata.size();
    m_iDataOffset      = RAWdata.mid(ENTRY_RP_BASE_ADDR_FIELD_AREA,   ENTRY_Len_BASE_ADDR_FIELD_AREA).toInt(&error);
    if (!error) return false;

    m_iLenFieldSize    = RAWdata.mid(ENTRY_RP_SIZE_OF_LENGTH_FIELD,   ENTRY_Len_SIZE_OF_LENGTH_FIELD).toInt(&error);
    if (!error) return false;

    m_iOffsetFieldSize = RAWdata.mid(ENTRY_RP_SIZE_OF_POSITION_FIELD, ENTRY_Len_SIZE_OF_POSITION_FIELD).toInt(&error);
    if (!error) return false;

    m_iTagFieldSize    = RAWdata.mid(ENTRY_RP_SIZE_OF_TAG_FIELD,      ENTRY_Len_SIZE_OF_TAG_FIELD).toInt(&error);
    if (!error) return false;

    int iIndex = ENTRY_RP_DATA;
    int iTagLevel = 0;
    struct RECORD_DATA RecordData;
    RecordData.ulID = 0;
    while (iIndex < m_iDataOffset - 1)  // Учитывается терминальный байт 0x1E в конце списка тэгов
    {
        CDRfield field;
        field.SetTag(QString::fromLocal8Bit(RAWdata.mid(iIndex, m_iTagFieldSize)));
        iIndex += m_iTagFieldSize;

        int iFieldLen = RAWdata.mid(iIndex, m_iLenFieldSize).toInt(&error);
        if (!error) return false;

        field.SetLen(iFieldLen);
        iIndex += m_iLenFieldSize;

        int iFieldOffset = RAWdata.mid(iIndex, m_iOffsetFieldSize).toInt(&error);
        if (!error) return false;

        field.SetOffset(iFieldOffset);
        iIndex += m_iOffsetFieldSize;

        field.SetRAWdata(RAWdata.mid(m_iDataOffset + iFieldOffset, iFieldLen));

        switch (iTagLevel)
        {
            case 0: if (field.GetTag().compare("0001") == 0) iTagLevel = tag0001(field.GetRAWdata());
                    RecordData.ulID = 0;
                    break;

            case 1: if (field.GetTag().compare("DSPM") == 0) iTagLevel = tagDSPM(field.GetRAWdata(), &RecordData);
                    if (field.GetTag().compare("FRID") == 0) iTagLevel = tagFRID(field.GetRAWdata(), &RecordData);
                    if (field.GetTag().compare("VRID") == 0) iTagLevel = tagVRID(field.GetRAWdata(), &RecordData);
                    break;

            case 2: if (field.GetTag().compare("FSPT") == 0) iTagLevel = tagFSPT(field.GetRAWdata(), RecordData);
                    if (field.GetTag().compare("VRPT") == 0) iTagLevel = tagVRPT(field.GetRAWdata(), RecordData);
                    if (field.GetTag().compare("SG2D") == 0) iTagLevel = tagSG2D(field.GetRAWdata(), RecordData);
                    if (field.GetTag().compare("SG3D") == 0) iTagLevel = tagSG3D(field.GetRAWdata(), RecordData);
                    break;
        }

        if (iTagLevel < 0) return false;

        m_FieldsList.append(field);
    }
    return true;
}

int CDRrecord::tag0001(QByteArray data)
{
    if (m_pParent == nullptr) return false;
    if (*(data.end() - 1) != TERMINAL_BYTE) return -1;

    switch (data.size())
    {
    case 2:  m_ulID = *((uint8_t*)data.constData()); break;
    case 3:  m_ulID = *((uint16_t*)data.constData()); break;
    case 5:  m_ulID = *((uint32_t*)data.constData()); break;
    default: return -1;
    }

    return 1;
}

int CDRrecord::tagDSPM(QByteArray data, struct RECORD_DATA *p_RecordData)
{
    if (m_pParent == nullptr) return false;
    if (*(data.end() - 1) != TERMINAL_BYTE) return -1;

    struct DSPM *dspm = (struct DSPM *)(data.constData());

    if (dspm->RCNM != DSPM_RECORD_NAME) return -1;

    unsigned long ulRCID = dspm->RCID;
    if (ulRCID >= 100000) ulRCID -= 100000;

    ((CAstS57Parser *)m_pParent)->SetCOMF(dspm->COMF);
    ((CAstS57Parser *)m_pParent)->SetSOMF(dspm->SOMF);
    ((CAstS57Parser *)m_pParent)->SetComment(QByteArray(dspm->COMT, data.size() - sizeof(struct DSPM) - 1));  // Длина массива COMT (1 байт) уже вычитается, поэтому необходимо добавить только вычитание одного байта из двух терминальных 0x1F и 0x1E

    p_RecordData->ulID = ulRCID;
    p_RecordData->ucType = dspm->RCNM;

    return 2;
}

int CDRrecord::tagFRID(QByteArray data, struct RECORD_DATA *p_RecordData)
{
    if (m_pParent == nullptr) return false;
    struct FRID *frid = (struct FRID *)(data.constData());

    if (data.size() != sizeof(struct FRID) || frid->TerminalByte != TERMINAL_BYTE) return -1;
    if (frid->RCNM != FRID_RECORD) return -1;

    unsigned long ulRCID = frid->RCID;

    if (!((CAstS57Parser *)m_pParent)->GetObjectsList()->contains(ulRCID))
    {
        CAstS57Object S57Object;
        S57Object.SetID(ulRCID);
        ((CAstS57Parser *)m_pParent)->SetObject(S57Object);
    }

    CAstS57Object *pObj = ((CAstS57Parser *)m_pParent)->GetObject(ulRCID);
    pObj->SetObjectClass(frid->OBJL);
    pObj->SetObjectPrimitiveType(frid->PRIM);

    p_RecordData->ulID = ulRCID;
    p_RecordData->ucType = frid->RCNM;

    return 2;
}

int CDRrecord::tagVRID(QByteArray data, struct RECORD_DATA *p_RecordData)
{
    if (m_pParent == nullptr) return false;
    struct VRID *vrid = (struct VRID *)(data.constData());

    if (data.size() != sizeof(struct VRID) || vrid->TerminalByte != TERMINAL_BYTE) return -1;
    if (vrid->RCNM != ISOLATED_NODE && vrid->RCNM != CONNECTED_NODE && vrid->RCNM != EDGE_NODE && vrid->RCNM != FACE_NODE) return -1;

    unsigned long ulRCID = vrid->RCID;

    if (!((CAstS57Parser *)m_pParent)->GetObjectsList()->contains(ulRCID))
    {
        CAstS57Object S57Object;
        S57Object.SetID(ulRCID);
        ((CAstS57Parser *)m_pParent)->SetObject(S57Object);
    }

    CAstS57Object *pObj = ((CAstS57Parser *)m_pParent)->GetObject(ulRCID);
    pObj->SetObjectClass(DMYOBJ);
    pObj->SetObjectPrimitiveType(NONE_PTYPE);

    p_RecordData->ulID = ulRCID;
    p_RecordData->ucType = vrid->RCNM;

    return 2;
}

int CDRrecord::tagFSPT(QByteArray data, struct RECORD_DATA RecordData)
{
    if (m_pParent == nullptr) return false;
    if (RecordData.ulID == 0) return -1;

    if (((data.size() - 1) % sizeof(struct FSPT)) != 0 || *(data.end() - 1) != TERMINAL_BYTE) return -1;  // Проверка длины поля. Должна быть кратна длине 2-х полей координат (X и Y) за исключением терминального байта 0x1E

    int iIndex = 0;
    struct FSPT *data_pointer = (struct FSPT *)data.constData();
    while (iIndex < data.size() - 1)  // Учёт терминального байта в текущем поле
    {
        ((CAstS57Parser *)m_pParent)->GetObject(RecordData.ulID)->SetRelatedObjectID(data_pointer->RCID);

        iIndex += sizeof(struct FSPT);
        data_pointer++;
    }

    return 2;
}

int CDRrecord::tagVRPT(QByteArray data, struct RECORD_DATA RecordData)
{
    if (m_pParent == nullptr) return false;
    if (RecordData.ulID == 0) return -1;

    if ((data.size() - 1) != (2 * sizeof(struct VRPT)) || *(data.end() - 1) != TERMINAL_BYTE) return -1;  // Проверка длины поля. Должна быть кратна длине 2-х полей координат (X и Y) за исключением терминального байта 0x1E

    struct VRPT *data_pointer = (struct VRPT *)data.constData();
    ((CAstS57Parser *)m_pParent)->GetVector(RecordData.ulID)->SetBegVectorID((data_pointer++)->RCID);
    ((CAstS57Parser *)m_pParent)->GetVector(RecordData.ulID)->SetEndVectorID(data_pointer->RCID);

    return 2;
}

int CDRrecord::tagSG2D(QByteArray data, struct RECORD_DATA RecordData)
{
    if (m_pParent == nullptr) return false;
    if (RecordData.ulID == 0) return -1;
    if (((data.size() - 1) % sizeof(struct SG2D_COORDINATES)) != 0 || *(data.end() - 1) != TERMINAL_BYTE) return -1;  // Проверка длины поля. Должна быть кратна длине 2-х полей координат (X и Y) за исключением терминального байта 0x1E

    int iIndex = 0;
    struct SG2D_COORDINATES *data_pointer = (struct SG2D_COORDINATES *)data.constData();
    while (iIndex < data.size() - 1)  // Учёт терминального байта в текущем поле
    {
        ((CAstS57Parser *)m_pParent)->GetVector(RecordData.ulID)->AddNode(data_pointer->X, data_pointer->Y, 0, RecordData.ucType);
        ((CAstS57Parser *)m_pParent)->DefineMinMaxPosition(data_pointer->X, data_pointer->Y);

        iIndex += sizeof(struct SG2D_COORDINATES);
        data_pointer++;
    }

    return 2;
}

int CDRrecord::tagSG3D(QByteArray data, struct RECORD_DATA RecordData)
{
    if (m_pParent == nullptr) return false;
    if (RecordData.ulID == 0) return -1;
    if (((data.size() - 1) % sizeof(struct SG3D_COORDINATES)) != 0 || *(data.end() - 1) != TERMINAL_BYTE) return -1;  // Проверка длины поля. Должна быть кратна длине 3-х полей координат (X, Y и V) за исключением терминального байта 0x1E

    int iIndex = 0;
    struct SG3D_COORDINATES *data_pointer = (struct SG3D_COORDINATES *)data.constData();
    while (iIndex < data.size() - 1)  // Учёт терминального байта в текущем поле
    {
        ((CAstS57Parser *)m_pParent)->GetVector(RecordData.ulID)->AddNode(data_pointer->X, data_pointer->Y, data_pointer->V, RecordData.ucType);
        ((CAstS57Parser *)m_pParent)->DefineMinMaxPosition(data_pointer->X, data_pointer->Y);

        iIndex += sizeof(struct SG3D_COORDINATES);
        data_pointer++;
    }

    return 2;
}

CAstS57Parser::CAstS57Parser()
{
    clear();
}

CAstS57Parser::CAstS57Parser(QByteArray RAWdata)
{
    clear();

    SetDataRecordsList(RAWdata);
}

void CAstS57Parser::clear(void)
{
    m_DRrecords.clear();
    m_MapObjects.clear();

    m_bMinMaxPositionDetect = false;
    m_lMinXPosition = 0;
    m_lMaxXPosition = 0;
    m_lMinYPosition = 0;
    m_lMaxYPosition = 0;

    m_ulCOMF = 1;
    m_ulSOMF = 1;
    m_Comment.clear();

}

void CAstS57Parser::SetDataRecordsList(QByteArray RAWdata)
{
    // Нахождение всех полей Data Record и создание списка записей 
    int iDRindex = 0;
    int iDRlen = 0;
    while ((iDRindex = RAWdata.indexOf(" D     ", iDRindex + iDRlen)) != -1)
    {
        if (iDRindex < ENTRY_Len_RECORD_LENGTH)
            break;

        bool error;
        iDRlen = RAWdata.mid(iDRindex + ENTRY_RP_RECORD_LENGTH, ENTRY_Len_RECORD_LENGTH).toInt(&error);
        if (!error || iDRlen < ENTRY_RP_DATA)
            continue;

        CDRrecord Record(this);

        if (!Record.SetData(RAWdata.mid(iDRindex + ENTRY_RP_RECORD_LENGTH, iDRlen)))
            continue;

        SetRecord(Record);
    }
}

void CAstS57Parser::DefineMinMaxPosition(long lXPosition, long lYPosition)
{
    if (m_bMinMaxPositionDetect)
    {
        if (lXPosition < m_lMinXPosition)
        {
            m_lMinXPosition = lXPosition;
        }
        else
        {
            if (lXPosition > m_lMaxXPosition)
            {
                m_lMaxXPosition = lXPosition;
            }
        }

        if (lYPosition < m_lMinYPosition)
        {
            m_lMinYPosition = lYPosition;
        }
        else
        {
            if (lYPosition > m_lMaxYPosition)
            {
                m_lMaxYPosition = lYPosition;
            }
        }

    }
    else
    {
        m_lMinXPosition = lXPosition;
        m_lMaxXPosition = lXPosition;
        m_lMinYPosition = lYPosition;
        m_lMaxYPosition = lYPosition;

        m_bMinMaxPositionDetect = true;
    }
}
