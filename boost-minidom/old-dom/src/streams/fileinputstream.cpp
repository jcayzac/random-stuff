// $Id: fileinputstream.cpp 8630 2007-12-04 17:40:17Z jcayzac $
#include <llsdk/inc/streams/fileinputstream.h>
#include <llsdk/inc/memory/memory.h>
namespace msdk {

CFileInputStream::CFileInputStream(const CString& path)
: m_ulFileOffset(0)
, m_ulRemainingSize(0)
{
	msdk::CPath oPath;
	oPath.Assign(path);
	m_oFile = CLocalFile(oPath);
	m_oFile.GetSize(m_ulRemainingSize);
	m_oFile.Open(CLocalFile::ReadFlag);
	if (!m_oFile.IsOpened()) {
		m_ulRemainingSize=0;
	}
}
CFileInputStream::CFileInputStream(const CPath& path)
: m_oFile(path)
, m_ulFileOffset(0)
, m_ulRemainingSize(0)
{
	m_oFile.GetSize(m_ulRemainingSize);
	m_oFile.Open(CLocalFile::ReadFlag);
	if (!m_oFile.IsOpened()) {
		m_ulRemainingSize=0;
	}
}
CFileInputStream::CFileInputStream(const CLocalFile& file)
: m_oFile(file)
, m_ulFileOffset(0)
, m_ulRemainingSize(0)
{
	m_oFile.GetSize(m_ulRemainingSize);
	m_oFile.Open(CLocalFile::ReadFlag);
	if (!m_oFile.IsOpened()) {
		m_ulRemainingSize=0;
	}
}
CFileInputStream::~CFileInputStream()
{ }

bool CFileInputStream::opened() const {
	return !eof();
}

unsigned long CFileInputStream::available() const {
	return m_ulRemainingSize;
}
bool CFileInputStream::eof() const {
	return m_oFile.IsOpened() && (m_ulRemainingSize==0);
}
bool CFileInputStream::read(unsigned char& b) {
	if (1==read(&b,0,1))
		return true;
	return false;
}
unsigned long CFileInputStream::read(unsigned char* b, unsigned long off, unsigned long len) {
	if (!m_ulRemainingSize) return 0;
	len=(len>m_ulRemainingSize)?m_ulRemainingSize:len;
	unsigned long result=0;
	m_oFile.Read(m_ulFileOffset,len,b+off,result);
	m_ulRemainingSize-=result;
	m_ulFileOffset+=result;
	return result;
}
unsigned long CFileInputStream::skip(unsigned long n) {
	n=(n>m_ulRemainingSize)?m_ulRemainingSize:n;
	m_ulRemainingSize-=n;
	m_ulFileOffset+=n;
	return n;
}

} // namespace msdk
