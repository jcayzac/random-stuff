#if !defined(__SPHERE_H)
#define __SPHERE_H

#include "object.h"

class CSphere: public C3DObject {
	void	tesselate(unsigned);
public:
	CSphere(unsigned);
	virtual ~CSphere(void);
};

#endif

