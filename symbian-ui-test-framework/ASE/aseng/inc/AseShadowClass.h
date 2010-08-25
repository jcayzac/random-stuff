/////////////////////// Shadow classes creation ///////////////////////
// Todo: rewrite this using templates (should shrink generated code's size)
// Todo: make it thread safe! (cannot be used in the new implementation!)
//
//
//		Sample usage:
//		------------
//
//		shadowClass(SCToto) // please follow the SC... convention for shadow classes' name
//			{
//			shadowImpl(SCToto);
//		public:
//			static SCToto* NewL();
//			bool zou(skRValueArray& /*aArguments*/, skRValue& /*aResult*/);
//		private:
//			void ConstructL();
//		private:
//			// members are declared in ConstructL()
//			};
//
//		static SCToto* SCToto::NewL()
//			{
//			SCToto* self = new(ELeave) SCToto;
//			CleanupStack::PushL(self);
//			self->ConstructL();
//			CleanupStack::Pop(self);
//			return self;
//			}
//
//		void SCToto::ConstructL()
//			{
//			// add a member named "int", whose value is 10
//			memberL(_L("int")) = 10;
//			// add a method named "zou"
//			AddMethodL(_L("zou"), &SCToto::zou);
//			}
//
//		bool SCToto::zou(skRValueArray& /*aArguments*/, skRValue& /*aResult*/)
//			{
//			User::InfoPrint(_L("Zou!"));
//			return true;
//			}
//	
///////////////////////////////////////////////////////////////////////////////////

#if !defined(ASE_SHADOW_CLASS_H_INCLUDED)
#define ASE_SHADOW_CLASS_H_INCLUDED

template<class T>
class CArrayFixSegPtr32: public CArrayFixSeg<T>
	{
public:
	CArrayFixSegPtr32(): CArrayFixSeg<T>(32) { }
	virtual ~CArrayFixSegPtr32()
		{
		for (int i=0; i<Count(); ++i)
			{
			delete (*this)[i];
			(*this)[i] = 0;
			}
		}
	};

#define shadowClass(classname) \
class classname: public skExecutable

#define shadowClassTypes(classname) \
typedef bool (classname::*TMethodPtr)(skRValueArray& aArguments, skRValue& aResult);	


#define shadowClassLookups \
private:														\
class CMember: public CBase										\
	{															\
	public:														\
		static CMember* NewL(const TDesC& aName)				\
			{													\
			CMember* self = new(ELeave) CMember;				\
			CleanupStack::PushL(self);							\
			self->ConstructL(aName);							\
			CleanupStack::Pop(self);							\
			return self;										\
			}													\
		virtual ~CMember()										\
			{													\
			delete iName;										\
			}													\
	private:													\
		CMember(const CMember& /*aCopy*/) { }					\
		CMember& operator=(const CMember& /*aCopy*/) { }		\
		CMember(): iMember(0)	{ }								\
		void ConstructL(const TDesC& aName)						\
			{													\
			iName = aName.AllocL();								\
			}													\
																\
	public:														\
		HBufC*			iName;									\
		skRValue		iMember;								\
	};															\
class CMethod: public CBase										\
	{															\
	public:														\
		static CMethod* NewL(const TDesC& aName)				\
			{													\
			CMethod* self = new(ELeave) CMethod;				\
			CleanupStack::PushL(self);							\
			self->ConstructL(aName);							\
			CleanupStack::Pop(self);							\
			return self;										\
			}													\
		virtual ~CMethod()										\
			{													\
			delete iName;										\
			}													\
	private:													\
		CMethod(const CMethod& /*aCopy*/) { }					\
		CMethod& operator=(const CMethod& /*aCopy*/){ }			\
		CMethod(): iMethod(0) { }								\
		void ConstructL(const TDesC& aName)						\
			{													\
			iName = aName.AllocL();								\
			}													\
																\
	public:														\
		HBufC*		iName;										\
		TMethodPtr	iMethod;									\
	};															\
CArrayFixSegPtr32<CMember*>	iMemberLookup;						\
CArrayFixSegPtr32<CMethod*>	iMethodLookup

#define shadowClassExecutableMethods \
public: \
bool method(const skString& aMethodName, skRValueArray& aArguments, skRValue& aResult, skExecutableContext& aContext) \
	{																		\
	bool res = false;														\
	for (int i=0; i<iMethodLookup.Count(); ++i)								\
		{																	\
		CMethod* method = iMethodLookup[i];									\
		if (aMethodName == *method->iName)									\
			{																\
			TMethodPtr function = method->iMethod;							\
			TRAPD(error, (*this.*function)(aArguments, aResult));			\
			if (error==KErrNone)											\
				{															\
				res = true;													\
				break;														\
				}															\
			}																\
		}																	\
	if (!res)																\
		{																	\
		res = skExecutable::method(aMethodName, aArguments, aResult, aContext); \
		}																	\
	return res;																\
	}																		\
	bool getValue (const skString& aFieldName, const skString& aAttribute, skRValue& aValue) \
		{																	\
		bool res = false;													\
		for (int i=0; i<iMemberLookup.Count(); ++i)							\
			{																\
			CMember* member = iMemberLookup[i];								\
			if (aFieldName==*member->iName)									\
				{															\
				res=true;													\
				aValue = iMemberLookup[i]->iMember;							\
				}															\
			}																\
		if (!res)															\
			{																\
			res = skExecutable::getValue(aFieldName, aAttribute, aValue);	\
			}																\
		return res;															\
		}																	\
	bool setValue (const skString& aFieldName, const skString& aAttribute, const skRValue& aValue) \
		{																	\
		bool res = false;													\
		for (int i=0; i<iMemberLookup.Count(); ++i)							\
			{																\
			CMember* member = iMemberLookup[i];								\
			if (aFieldName==*member->iName)									\
				{															\
				res=true;													\
				iMemberLookup[i]->iMember = aValue;							\
				}															\
			}																\
		if (!res)															\
			{																\
			res = skExecutable::setValue(aFieldName, aAttribute, aValue);	\
			}																\
		return res;															\
		}

#define shadowClassManipulators \
skRValue& memberL(const TDesC& aName)						\
	{														\
	for (int i=0; i<iMemberLookup.Count(); ++i)				\
		{													\
		CMember* member = iMemberLookup[i];					\
		if (aName==*member->iName)							\
			{												\
			return iMemberLookup[i]->iMember;				\
			}												\
		}													\
	/* not found, create */									\
	CMember* member = CMember::NewL(aName);					\
	CleanupStack::PushL(member);							\
	iMemberLookup.AppendL(member);							\
	CleanupStack::Pop(member);								\
	return member->iMember;									\
	}														\
															\
skRValue& memberL(const skString& aName)					\
	{														\
	return memberL(aName.ptr());							\
	}														\
															\
void AddMethodL(const TDesC& aName, TMethodPtr aMethod)		\
	{														\
	CMethod* mymethod = CMethod::NewL(aName);				\
	CleanupStack::PushL(mymethod);							\
	mymethod->iMethod = aMethod;							\
	iMethodLookup.AppendL(mymethod);						\
	CleanupStack::Pop(mymethod);							\
	}




#define shadowImpl(classname) \
shadowClassTypes(classname);	\
shadowClassLookups;				\
shadowClassExecutableMethods;	\
shadowClassManipulators

#endif
