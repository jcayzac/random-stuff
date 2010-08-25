#if !defined(__NOISYSPHERE_H)
#define __NOISYSPHERE_H

#include "sphere.h"

class CNoisySphere: public CSphere {
public:
	CNoisySphere(unsigned,float);
	virtual ~CNoisySphere(void);
};

#endif
