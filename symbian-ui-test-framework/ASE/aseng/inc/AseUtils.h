#if !defined(ASEUTILS_H_INCLUDED)
#define ASEUTILS_H_INCLUDED

#include <e32std.h>
#include <e32base.h>

//
// An array of owned pointers
//
template<class T>
class CArrayFixSegPtr32: public CArrayFixSeg<T>
	{
	public:
		inline CArrayFixSegPtr32(): CArrayFixSeg<T>(32) { }
		virtual inline ~CArrayFixSegPtr32()
			{
			for (int i=0; i<Count(); ++i)
				{
				delete (*this)[i];
				(*this)[i] = 0;
				}
			}
	};

#endif // ASEUTILS_H_INCLUDED
