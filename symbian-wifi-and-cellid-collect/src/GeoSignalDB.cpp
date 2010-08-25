#include <eikenv.h>
#include "GeoSignalDB.h"

static const unsigned long EMaxRows=1024;
_LIT(KTraceFile, "c:\\geotrace.bin");

CGeoSignalDB::CGeoSignalDB() {
	// nop?
}
CGeoSignalDB::~CGeoSignalDB() {
	WriteToDisk();
}

bool CGeoSignalDB::AddRow(const TRawRow& row) {
	if ((unsigned long) m_oData.Count()>=EMaxRows)
		if (!WriteToDisk()) return false;
	//
	TRawRow newRow;
	unsigned long count=m_oData.Count();
	if (KErrNone!=m_oData.Append(newRow))
		return false;
	m_oData[count]=row;
	return true;
}
bool CGeoSignalDB::WriteToDisk() {
	RFs fs;
	if (KErrNone!=fs.Connect()) {
		CEikonEnv::InfoWinL(_L("Error"),_L("Can't connect to RFs"));
		return false;
	}
	RFile file;
	TInt error=file.Create(fs,KTraceFile,EFileShareExclusive|EFileWrite|EFileStream);
	if (error!=KErrNone) error=file.Open(fs,KTraceFile,EFileShareExclusive|EFileWrite|EFileStream);
	if (error==KErrNone) {
		TInt zero=0;
		file.Seek(ESeekEnd, zero);
		//
		unsigned long count=m_oData.Count();
		unsigned long index=0;
		while(index<count) {
			TRawRow& row = m_oData[index++];
			const unsigned long* data = &row[0];
			file.Write(TPtrC8((const TUint8*)data, 8*sizeof(unsigned long)));
		}
		m_oData.Reset();
		return true;
	}
	return false;
}
