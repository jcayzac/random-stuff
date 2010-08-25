// 64-bit integer math for 32-bit cpus
#pragma once

class sixtyfour {
public:
	typedef struct {
		int hi;
		unsigned int lo;
	} type_t;

	static type_t add64_64(type_t a,type_t b) {
		if (a.lo+b.lo<a.lo) ++a.hi;
		a.lo += b.lo;
		a.hi += b.hi;
		return a;
	}

	static int ladd64_64(type_t a,type_t b) {
		return (int)(add64_64(a,b).lo);
	} 

	static type_t set64_int(int b) {
		type_t a;
		a.lo=b;
		if(b<0)
			a.hi=-1;
		else
			a.hi=0;
		return a;
	} 

	static type_t add64_int(type_t a,int b) {
		type_t temp=set64_int(b);
		return add64_64(a,temp);
	} 

	static type_t neg64(type_t a) {
		a.lo=~a.lo;
		a.hi=~a.hi;
		return add64_int(a,1);
	} 


	static short cmp64_64(type_t a,type_t b) {
		if(a.hi<b.hi)return -1;
		if(a.hi>b.hi)return	1;
		if(a.lo<b.lo)return -1;
		if(a.lo>b.lo)return	1;
		return 0;
	}

	static short cmp64_64off(type_t a,int offa,
		type_t b,int offb
	) {
		a=add64_int(a,offa);
		b=add64_int(b,offb);
		return cmp64_64(a,b);
	}

	static short cmp64_int(type_t a,int b) {
		type_t bb=set64_int(b);
		return cmp64_64(a,bb);
	}

	static type_t asr64(type_t a, short b) {
		if(b>=32) {
			a.lo=a.hi>>(b-32);
			a.hi=a.hi>>31;
		}
		else {
			a.lo=(a.lo>>b)|(a.hi<<(32-b));
			a.hi=(a.hi>>b);
		}
		return a;
	}

	static type_t asl64(type_t a, short b) {
		if(b>=32) {
			a.hi=a.lo<<(b-32);
			a.lo=0;
		}
		else {
			a.hi=(a.hi<<b)|(a.lo>>(32-b));
			a.lo=(a.lo<<b);
		}
		return a;
	}

	/* simple binary long division using subtractions */
	static type_t div64_int(type_t a, int b) {
		short sign=0,bit=0;
		type_t ret={0,0};

		if(!b)
			return ret;

		if(a.hi<0) {
			sign=1;
			a=neg64(a);
		}
		if(b<0) {
			sign = !sign;
			b = -b;
		}

		/* normalize b */
		while(!b&0x40000000L) {
			b<<=1;
			bit++;
		}

		/* hi section */
		while(bit) {
			--bit;
			if(a.hi>=b){
				ret.hi|=1<<bit;
				a.hi-=b;
			}
			a.hi<<=1;
			a.hi|=a.lo>>31;
			a.lo<<=1;
		}

		/* lo section */
		bit=32;
		while(bit) {
			--bit;
			if(a.hi>=b) {
				ret.lo|=1<<bit;
				a.hi-=b;
			}
			a.hi<<=1;
			a.hi|=a.lo>>31;
			a.lo<<=1;
		}

		return sign?neg64(ret):ret;
	}

	// long multiplication, much like int division; 32 adds
	static type_t mul64_int(type_t a, int b) {
		short sign=0,bit;
		type_t ret={0,0};

		if(!b)
			return ret;

		if (a.hi<0) {
			sign=1;
			a=neg64(a);
		}
		if (b<0) {
			sign = !sign;
			b = -b;
		}
	
		for(bit=0;bit<31;bit++){
			if(b&1)
				ret = add64_64(ret,a);
			b>>=1;
			a = asl64(a,1);
		}

		return sign?neg64(ret):ret;
	}
};

