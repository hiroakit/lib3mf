/*++

Copyright (C) 2018 3MF Consortium (Original Author)

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract: This is a stub class definition of CLib3MFObject

*/

#include "lib3mf_object.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "lib3mf_meshobject.hpp"
#include "lib3mf_componentsobject.hpp"
#include "lib3mf_metadatagroup.hpp"
#include "lib3mf_slicestack.hpp"

// Include custom headers here.
#include "Model/Classes/NMR_ModelMeshObject.h" 
#include "Model/Classes/NMR_ModelComponentsObject.h" 

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CLib3MFObject 
**************************************************************************************************************************/

NMR::CModelObject* CLib3MFObject::object()
{
	NMR::CModelObject* pObject = dynamic_cast<NMR::CModelObject*>(resource().get());
	if (pObject == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	return pObject;
}

ILib3MFObject* CLib3MFObject::fnCreateObjectFromModelResource(NMR::PModelResource pResource, bool bFailIfUnkownClass) {
	
	if (!pResource.get())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	NMR::CModelMeshObject * pMeshObject = dynamic_cast<NMR::CModelMeshObject *> (pResource.get());
	if (pMeshObject) {
		return new CLib3MFMeshObject(pResource);
	}

	NMR::CModelComponentsObject * pComponentsResource = dynamic_cast<NMR::CModelComponentsObject *> (pResource.get());
	if (pComponentsResource) {
		return new CLib3MFComponentsObject(pResource);
	}

	if (bFailIfUnkownClass)
		throw ELib3MFInterfaceException(NMR_ERROR_UNKNOWNMODELRESOURCE);

	return nullptr;
}

CLib3MFObject::CLib3MFObject(NMR::PModelResource pResource)
	:CLib3MFResource(pResource)
{
}


eLib3MFObjectType CLib3MFObject::GetType ()
{
	return eLib3MFObjectType(object()->getObjectType());
}

void CLib3MFObject::SetType (const eLib3MFObjectType eObjectType)
{
	object()->setObjectType(NMR::eModelObjectType(eObjectType));
}

std::string CLib3MFObject::GetName ()
{
	return object()->getName();
}

void CLib3MFObject::SetName (const std::string & sName)
{
	object()->setName(sName);
}

std::string CLib3MFObject::GetPartNumber ()
{
	return object()->getPartNumber();
}

void CLib3MFObject::SetPartNumber (const std::string & sPartNumber)
{
	object()->setPartNumber(sPartNumber);
}

bool CLib3MFObject::IsMeshObject ()
{
	// implemented by base class
	throw ELib3MFInterfaceException(LIB3MF_ERROR_SHOULDNOTBECALLED);
}

bool CLib3MFObject::IsComponentsObject ()
{
	// implemented by base class
	throw ELib3MFInterfaceException(LIB3MF_ERROR_SHOULDNOTBECALLED);
}

ILib3MFMeshObject * CLib3MFObject::AsMeshObject()
{
	if (dynamic_cast<NMR::CModelMeshObject*>(resource().get()))
	{
		return new CLib3MFMeshObject(resource());
	}
	throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);
}

ILib3MFComponentsObject * CLib3MFObject::AsComponentsObject()
{
	if (dynamic_cast<NMR::CModelComponentsObject*>(resource().get()))
	{
		return new CLib3MFComponentsObject(resource());
	}
	throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);
}

bool CLib3MFObject::IsValid ()
{
	// Implemented by base class
	throw ELib3MFInterfaceException (LIB3MF_ERROR_SHOULDNOTBECALLED);
}

ILib3MFMetaDataGroup * CLib3MFObject::GetMetaDataGroup ()
{
	return new CLib3MFMetaDataGroup(object()->metaDataGroup());
}

std::string CLib3MFObject::GetUUID(bool & bHasUUID)
{
	NMR::PUUID pUUID = object()->uuid();
	bHasUUID = (pUUID.get() != nullptr);
	if (bHasUUID)
		return pUUID->toString();
	else
		return "";
}

void CLib3MFObject::SetUUID(const std::string & sUUID)
{
	NMR::PUUID pUUID = std::make_shared<NMR::CUUID>(sUUID);
	object()->setUUID(pUUID);
}

void CLib3MFObject::SetSlicesMeshResolution(const eLib3MFSlicesMeshResolution eMeshResolution)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

eLib3MFSlicesMeshResolution CLib3MFObject::GetSlicesMeshResolution()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CLib3MFObject::HasSliceStack()
{
	return (object()->getSliceStack().get() != nullptr);
}

void CLib3MFObject::ClearSliceStack()
{
	object()->assignSliceStack(nullptr);
}

ILib3MFSliceStack * CLib3MFObject::GetSliceStack()
{
	NMR::PModelSliceStack pSliceStackResource = object()->getSliceStack();
	if (pSliceStackResource)
		return new CLib3MFSliceStack(pSliceStackResource);
	else
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDSLICESTACKRESOURCE);
}

void CLib3MFObject::AssignSliceStack(ILib3MFSliceStack* pSliceStackInstance)
{
	Lib3MF_uint32 id = pSliceStackInstance->GetResourceID();

	NMR::PModelSliceStack pSliceStackResource = std::dynamic_pointer_cast<NMR::CModelSliceStack>
		( object()->getModel()->findResource(id) );
	if (!pSliceStackResource)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDSLICESTACKRESOURCE);

	object()->assignSliceStack(pSliceStackResource);
}
