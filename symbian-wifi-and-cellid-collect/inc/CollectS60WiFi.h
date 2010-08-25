#ifndef __COLLECTS60WIFI_H__
#define __COLLECTS60WIFI_H__
#include <wlanmgmtclient.h>
#include <wlanscaninfo.h>

class HMAC {
public:
	unsigned char m_pValue[6];
public:
	HMAC();
	HMAC(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f);
	HMAC(const unsigned char* value);
	HMAC(const HMAC& o);
	bool operator==(const HMAC& o) const;
	bool operator!=(const HMAC& o) const { return !(o==*this); }
	bool Null() const {
		return (m_pValue[0]==0 && m_pValue[1]==0 && m_pValue[2]==0 &&
				m_pValue[3]==0 && m_pValue[4]==0 && m_pValue[5]==0);
	}
};

class WiFiInfo {
public:
	HMAC	m_oHMAC;
	int		m_iSignalStrength;
public:
	WiFiInfo();
	WiFiInfo(const HMAC& hmac, int ss);
	WiFiInfo(const WiFiInfo& o);
	bool operator==(const WiFiInfo& o) const;
	bool operator!=(const WiFiInfo& o) const { return !(o==*this); }
};

class CWifi {
public:
	static CWifi* NewL();
	static CWifi* NewLC();
	virtual ~CWifi();
	bool GetInfo(RArray<WiFiInfo>& wia);
protected:
	CWifi();
	void ConstructL();
private:
	CWlanScanInfo*		m_pScanInfo;
	CWlanMgmtClient*	m_pClient;
};

#endif // __COLLECTS60WIFI_H__

