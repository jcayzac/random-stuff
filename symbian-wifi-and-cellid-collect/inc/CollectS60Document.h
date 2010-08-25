/*
 ============================================================================
 Name		: CollectS60Document.h
 Author	  : Mappy
 Copyright   : Confidential
 Description : Declares document class for application.
 ============================================================================
 */

#ifndef __COLLECTS60DOCUMENT_h__
#define __COLLECTS60DOCUMENT_h__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class CCollectS60AppUi;
class CEikApplication;

// CLASS DECLARATION

/**
 * CCollectS60Document application class.
 * An instance of class CCollectS60Document is the Document part of the
 * AVKON application framework for the CollectS60 example application.
 */
class CCollectS60Document : public CAknDocument
	{
public:
// Constructors and destructor

/**
 * NewL.
 * Two-phased constructor.
 * Construct a CCollectS60Document for the AVKON application aApp
 * using two phase construction, and return a pointer
 * to the created object.
 * @param aApp Application creating this document.
 * @return A pointer to the created instance of CCollectS60Document.
 */
static CCollectS60Document* NewL (CEikApplication& aApp);

/**
 * NewLC.
 * Two-phased constructor.
 * Construct a CCollectS60Document for the AVKON application aApp
 * using two phase construction, and return a pointer
 * to the created object.
 * @param aApp Application creating this document.
 * @return A pointer to the created instance of CCollectS60Document.
 */
static CCollectS60Document* NewLC (CEikApplication& aApp);

/**
 * ~CCollectS60Document
 * Virtual Destructor.
 */
virtual ~CCollectS60Document ();

public:
// Functions from base classes

/**
 * CreateAppUiL
 * From CEikDocument, CreateAppUiL.
 * Create a CCollectS60AppUi object and return a pointer to it.
 * The object returned is owned by the Uikon framework.
 * @return Pointer to created instance of AppUi.
 */
CEikAppUi* CreateAppUiL ();

private:
// Constructors

/**
 * ConstructL
 * 2nd phase constructor.
 */
void ConstructL ();

/**
 * CCollectS60Document.
 * C++ default constructor.
 * @param aApp Application creating this document.
 */
CCollectS60Document (CEikApplication& aApp);

	};

#endif // __COLLECTS60DOCUMENT_h__
// End of File
