#ifndef GEOSIGNALDB_H_
#define GEOSIGNALDB_H_

#include "CollectS60RIL.h"
#include "CollectS60WiFi.h"

struct GeoPosition {
	GeoPosition()
	: m_fLongitude(.0f), m_fLatitude(.0f)
	{ }
	GeoPosition(const GeoPosition& o)
	: m_fLongitude(o.m_fLongitude), m_fLatitude(o.m_fLatitude)
	{ }
	GeoPosition& operator=(const GeoPosition& o) {
		m_fLongitude=o.m_fLongitude;
		m_fLatitude=o.m_fLatitude;
	}
	float m_fLongitude;
	float m_fLatitude;
};

// Row indices
static const unsigned long EDBType		= 0;
static const unsigned long EDBMCC		= 1;
static const unsigned long EDBHMACHI24	= 1;
static const unsigned long EDBMNC		= 2;
static const unsigned long EDBHMACLO24	= 2;
static const unsigned long EDBAID		= 3;
static const unsigned long EDBCID		= 4;
static const unsigned long EDBLongitude	= 5;
static const unsigned long EDBLatitude	= 6;
static const unsigned long EDB2ASU		= 7;
// Values for column EDBType
static const unsigned long EDBTypeGSM  = 0;
static const unsigned long EDBTypeWLAN = 1;

struct TRawRow {
	unsigned long m_pData[8];
	//
	TRawRow() {
		m_pData[7]=m_pData[6]=m_pData[5]=m_pData[4]=m_pData[3]=m_pData[2]=m_pData[1]=m_pData[0]=0;
	}
	TRawRow(unsigned long type) {
		m_pData[7]=m_pData[6]=m_pData[5]=m_pData[4]=m_pData[3]=m_pData[2]=m_pData[1]=0;
		SetType(type);
	}
	TRawRow(const TRawRow& o) {
		for (int i=0; i<8; ++i) 
			m_pData[i]=o[i];
	}
	TRawRow& operator=(const TRawRow& o) {
	for (int i=0; i<8; ++i) 
		m_pData[i]=o[i];
	}
	unsigned long& operator[](int index) { return m_pData[index]; }
	const unsigned long& operator[](int index) const { return m_pData[index]; }
	//
	inline void Reset() {
		*this = TRawRow();
	}
	// Readers
	inline bool GetType(unsigned long& result) const {
		result=m_pData[EDBType];
		return true;
	}
	inline bool GetMCC(unsigned long& result) const {
		if (m_pData[EDBType]==EDBTypeGSM) {		result=m_pData[EDBMCC];	return true; }
		return false;
	}
	inline bool GetMNC(unsigned long& result) const {
		if (m_pData[EDBType]==EDBTypeGSM) {		result=m_pData[EDBMNC];	return true; }
		return false;
	}
	inline bool GetAID(unsigned long& result) const {
		if (m_pData[EDBType]==EDBTypeGSM) {		result=m_pData[EDBAID];	return true; }
		return false;
	}
	inline bool GetCID(unsigned long& result) const {
		if (m_pData[EDBType]==EDBTypeGSM) {		result=m_pData[EDBCID];	return true; }
		return false;
	}
	inline bool GetHMAC(HMAC& hmac) const {
		if (m_pData[EDBType]==EDBTypeWLAN) {
			unsigned long v=m_pData[EDBHMACHI24];
			hmac.m_pValue[0]=(v&0x00ff0000)>>16;
			hmac.m_pValue[1]=(v&0x0000ff00)>>8;
			hmac.m_pValue[2]=(v&0x000000ff);
			v=m_pData[EDBHMACLO24];
			hmac.m_pValue[3]=(v&0x00ff0000)>>16;
			hmac.m_pValue[4]=(v&0x0000ff00)>>8;
			hmac.m_pValue[5]=(v&0x000000ff);
			return true;
		}
		return false;
	}
	inline bool GetZoneInfo(ZoneInfo& zi) const {
		if (m_pData[EDBType]==EDBTypeGSM) {
			zi.m_ulMCC = m_pData[EDBMCC];
			zi.m_ulNID = m_pData[EDBMNC];
			zi.m_ulAID = m_pData[EDBAID];
			zi.m_ulCID = m_pData[EDBCID];
			return true;
		}
		return false;
	}
	inline bool GetPosition(GeoPosition& pos) const {
		union {
			unsigned long uval;
			float fval;
		} u;
		u.uval=m_pData[EDBLongitude];
		pos.m_fLongitude=u.fval;
		u.uval=m_pData[EDBLatitude];
		pos.m_fLatitude=u.fval;
		return true;
	}
	inline bool Get2ASU(unsigned long& result) const {
		result=m_pData[EDB2ASU];
		return true;
	}
	// Writers
	inline void SetType(unsigned long value) {	m_pData[EDBType]=value; }
	inline void SetMCC(unsigned long value)  {	m_pData[EDBMCC]=value;  }
	inline void SetMNC(unsigned long value)  {	m_pData[EDBMNC]=value;  }
	inline void SetAID(unsigned long value)  {	m_pData[EDBAID]=value;  }
	inline void SetCID(unsigned long value)  {	m_pData[EDBCID]=value;  }
	inline void SetHMAC(const HMAC& value)  {
		unsigned long v = ((unsigned long)(value.m_pValue[0]))<<16;
		v|=((unsigned long)(value.m_pValue[1]))<<8;
		v|=((unsigned long)(value.m_pValue[2]));
		m_pData[EDBHMACHI24]=v; 
		v = ((unsigned long)(value.m_pValue[3]))<<16;
		v|=((unsigned long)(value.m_pValue[4]))<<8;
		v|=((unsigned long)(value.m_pValue[5]));
		m_pData[EDBHMACLO24]=v; 
	}
	inline void SetZoneInfo(const ZoneInfo& zi) {
		m_pData[EDBMCC]=zi.m_ulMCC;
		m_pData[EDBMNC]=zi.m_ulNID;
		m_pData[EDBAID]=zi.m_ulAID;
		m_pData[EDBCID]=zi.m_ulCID;
	}
	inline void SetPosition(const GeoPosition& value)  {
		union {
			unsigned long uval;
			float fval;
		} u;
		u.fval=value.m_fLongitude;
		m_pData[EDBLongitude]=u.uval;
		u.fval=value.m_fLatitude;
		m_pData[EDBLatitude]=u.uval;
	}
	inline void Set2ASU(unsigned long value) {
		m_pData[EDB2ASU]=value;
	}
};


class CGeoSignalDB {
private:
	CGeoSignalDB(const CGeoSignalDB&);
	CGeoSignalDB& operator=(const CGeoSignalDB&);
public:
	CGeoSignalDB();
	~CGeoSignalDB();
	bool AddRow(const TRawRow& row);
private:
	bool WriteToDisk();
private:
	RArray<TRawRow>	m_oData;
};


#endif /*GEOSIGNALDB_H_*/
