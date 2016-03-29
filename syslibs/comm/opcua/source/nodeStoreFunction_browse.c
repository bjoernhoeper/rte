
/******************************************************************************
 *
 *   FILE
 *   ----
 *   nodeStoreFunctions.c
 *
 *   History
 *   -------
 *   2014-10-21   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif


#include "libov/ov_macros.h"
#include "ksbase.h"
#include "opcua.h"
#include "opcua_helpers.h"
#include "NoneTicketAuthenticator.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "ks_logfile.h"


UA_StatusCode opcua_nsOv_fillReferenceDescription(
		OV_ELEMENT* pElement, UA_UInt16 referenceTypeNamespaceIndex, UA_Int32 referenceType, UA_UInt32 resultMask, UA_ReferenceDescription* dst){
	OV_INSTPTR_ov_object	pObject			=	NULL;
	UA_StatusCode			result			=	UA_STATUSCODE_GOOD;
	OV_STRING				path			=	NULL;
	OV_STRING				varPath			=	NULL;
	OV_UINT					varPathLen		=	0;
	if(!dst){
		return UA_STATUSCODE_BADINVALIDARGUMENT;
	}
	dst->nodeId.nodeId.identifierType = UA_NODEIDTYPE_STRING;
	dst->nodeId.nodeId.namespaceIndex = 1;
	if(pElement->elemtype == OV_ET_OBJECT || pElement->elemtype == OV_ET_VARIABLE || pElement->elemtype == OV_ET_MEMBER){
		pObject = pElement->pobj;
	} else {
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}
	ov_memstack_lock();
	path = ov_path_getcanonicalpath(pObject, 2);
	if(!path){
		ov_memstack_unlock();
		return UA_STATUSCODE_BADOUTOFMEMORY;
	}
	if(pElement->elemtype == OV_ET_VARIABLE){
		varPathLen = ov_string_getlength(path) + ov_string_getlength(pElement->elemunion.pvar->v_identifier) + 2;
		varPath = ov_memstack_alloc(varPathLen);
		if(!varPath){
			ov_memstack_unlock();
			return UA_STATUSCODE_BADOUTOFMEMORY;
		}
		snprintf(varPath, varPathLen, "%s.%s", path, pElement->elemunion.pvar->v_identifier);
		path = varPath;
	}
	dst->nodeId.nodeId.identifier.string = UA_String_fromChars(path);
	if(dst->nodeId.nodeId.identifier.string.length == -1){
		result = UA_STATUSCODE_BADOUTOFMEMORY;
	} else {
		result = UA_STATUSCODE_GOOD;
	}
	ov_memstack_unlock();

	if(resultMask & (1<<3)){
		if(pElement->elemtype == OV_ET_OBJECT){
			dst->browseName.name = UA_String_fromChars(pElement->pobj->v_identifier);
		} else if(pElement->elemtype == OV_ET_VARIABLE){
			dst->browseName.name = UA_String_fromChars(pElement->elemunion.pvar->v_identifier);
		}
		dst->browseName.namespaceIndex = 1;
	}
	if(resultMask & (1<<4)){
		if(pElement->elemtype == OV_ET_OBJECT){
			dst->displayName.text = UA_String_fromChars(pElement->pobj->v_identifier);
		} else if(pElement->elemtype == OV_ET_VARIABLE){
			dst->displayName.text = UA_String_fromChars(pElement->elemunion.pvar->v_identifier);
		}
	}
	if(resultMask & (1<<1)){
		dst->isForward = UA_TRUE;
	}
	if(resultMask & (1<<2)){
		dst->nodeClass = opcua_nsOv_getNodeClassAndAccess(pElement, NULL);
	}
	if(resultMask & (1<<0)){	// in ov-namespace beware that numerical ids may change when the server restarts (but associations can change between /acplt/... and /Libraries/..., too)
		dst->referenceTypeId.namespaceIndex = referenceTypeNamespaceIndex;
		dst->referenceTypeId.identifierType = UA_NODEIDTYPE_NUMERIC;
		dst->referenceTypeId.identifier.numeric = referenceType;
	}
	if(resultMask & (1<<5)){	// TODO fixme	This is the type-node: 0|58 is baseObjectType
		dst->typeDefinition.nodeId.namespaceIndex = 0;
		dst->typeDefinition.nodeId.identifierType = UA_NODEIDTYPE_NUMERIC;
		dst->typeDefinition.nodeId.identifier.numeric = 58;
	}
	return result;
}
/******************************************************************
 * helper for Organizes
 *****************************************************************/

UA_Int32 getReferenceDescriptions_Organizes(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		OV_BOOL fillDescription, UA_UInt32 resultMask, UA_ReferenceDescription* dst,
		UA_StatusCode* statusCode){
	OV_ELEMENT	childPart, parentElement;
	OV_BOOL		maskMatch;
	OV_ACCESS	access;
	//	this is mapped to ov/containment
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_FORWARD)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		// children
		childPart.elemtype = OV_ET_NONE;
		childPart.pobj = NULL;
		ov_element_getnextchild(pNode, &childPart);
		while(childPart.elemtype!=OV_ET_NONE){
			maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&childPart, browseDescription->nodeClassMask, &access);
			if(maskMatch && (access & OV_AC_READ)){
				if(fillDescription){
					*statusCode = opcua_nsOv_fillReferenceDescription(&childPart, 0, UA_NODEID_Organizes, resultMask, &(dst[*refCount]));
				}
				(*refCount)++;
			}
			ov_element_getnextchild(pNode, &childPart);
		}
		// parts
		childPart.elemtype = OV_ET_NONE;
		childPart.pobj = NULL;
		ov_element_getnextpart(pNode, &childPart, OV_ET_OBJECT | OV_ET_VARIABLE);
		while(childPart.elemtype!=OV_ET_NONE){
			maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&childPart, browseDescription->nodeClassMask, &access);
			if(maskMatch && (access & OV_AC_READ)){
				if(childPart.elemtype == OV_ET_OBJECT || childPart.elemtype == OV_ET_VARIABLE || childPart.elemtype == OV_ET_MEMBER){
					if(fillDescription){
						*statusCode = opcua_nsOv_fillReferenceDescription(&childPart, 0, UA_NODEID_Organizes, resultMask, &(dst[*refCount]));
					}
					(*refCount)++;
				}
			}
			ov_element_getnextpart(pNode, &childPart, OV_ET_OBJECT | OV_ET_VARIABLE);
		}
	}
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_INVERSE)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		if(pNode->elemtype == OV_ET_OBJECT){
			if(pNode->pobj->v_idL || pNode->pobj->v_idH){	// don't do it for ov root object ("/")
				parentElement.elemtype = OV_ET_OBJECT;
				parentElement.pobj = (pNode->pobj->v_pouterobject ? pNode->pobj->v_pouterobject : Ov_StaticPtrCast(ov_object, Ov_GetParent(ov_containment, pNode->pobj)));
				maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&parentElement, browseDescription->nodeClassMask, &access);
				if(maskMatch && (access & OV_AC_READ)){
					if(fillDescription){
						*statusCode = opcua_nsOv_fillReferenceDescription(&parentElement, 0, UA_NODEID_Organizes, resultMask, &(dst[*refCount]));
					}
					(*refCount)++;
				}
			} else {	//	ov root-node --> bridge node in ns0
				if(fillDescription){
					*statusCode = UA_STATUSCODE_BADNOTIMPLEMENTED;
				}
				(*refCount)++;
			}
		} else {
			parentElement.elemtype = OV_ET_OBJECT;
			parentElement.pobj = pNode->pobj;
			maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&parentElement, browseDescription->nodeClassMask, &access);
			if(maskMatch && (access & OV_AC_READ)){
				if(fillDescription){
					*statusCode = opcua_nsOv_fillReferenceDescription(&parentElement, 0, UA_NODEID_Organizes, resultMask, &(dst)[*refCount]);
				}
				(*refCount)++;
			}
		}

	}
	return UA_STATUSCODE_GOOD;
}

UA_Int32 countReferenceDescriptions_Organizes(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount){
	return getReferenceDescriptions_Organizes(browseDescription, pNode,refCount, FALSE, 0, NULL, NULL);
}

UA_Int32 fillReferenceDescriptions_Organizes(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		UA_UInt32 resultMask, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
	return getReferenceDescriptions_Organizes(browseDescription, pNode, refCount, TRUE, resultMask, dst, statusCode);
}

/******************************************************************
 * helper for HasTypeDefinition
 *****************************************************************/

UA_Int32 getReferenceDescriptions_HasTypeDefinition(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		OV_BOOL fillDescription, UA_UInt32 resultMask, UA_ReferenceDescription* dst,
		UA_StatusCode* statusCode){
	OV_BOOL					maskMatch;
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_INSTPTR_ov_object	pChildObject	=	NULL;
	OV_ELEMENT				parentTypeDefinition, childTypeDefinition;
	//	maps to ov/instantiation
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_FORWARD)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		//	source is object target is type --> this direction is an instantiation getparent for objects,
		//		and analog for links and variables
		if(pNode->elemtype == OV_ET_OBJECT || pNode->elemtype == OV_ET_VARIABLE
				|| pNode->elemtype == OV_ET_MEMBER || pNode->elemtype == OV_ET_CHILDLINK || pNode->elemtype == OV_ET_PARENTLINK){
			maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(pNode, browseDescription->nodeClassMask, &access);
			if(maskMatch && (access & OV_AC_READ)){
				if(fillDescription){
					if(pNode->elemtype == OV_ET_OBJECT){
						parentTypeDefinition.elemtype = OV_ET_OBJECT;
						parentTypeDefinition.pobj = Ov_PtrUpCast(ov_object, Ov_GetParent(ov_instantiation, pNode->pobj));
						*statusCode = opcua_nsOv_fillReferenceDescription(&parentTypeDefinition, 0, UA_NODEID_HasTypeDefinition, resultMask, &(dst[*refCount]));
					} else if (pNode->elemtype == OV_ET_VARIABLE || pNode->elemtype == OV_ET_MEMBER){
						parentTypeDefinition.elemtype = OV_ET_OBJECT;
						parentTypeDefinition.pobj = Ov_PtrUpCast(ov_object, pNode->elemunion.pvar);
						*statusCode = opcua_nsOv_fillReferenceDescription(&parentTypeDefinition, 0, UA_NODEID_HasTypeDefinition, resultMask, &(dst[*refCount]));
					}else if (pNode->elemtype == OV_ET_PARENTLINK || pNode->elemtype == OV_ET_CHILDLINK){
						parentTypeDefinition.elemtype = OV_ET_OBJECT;
						parentTypeDefinition.pobj = Ov_PtrUpCast(ov_object, pNode->elemunion.passoc);
						*statusCode = opcua_nsOv_fillReferenceDescription(&parentTypeDefinition, 0, UA_NODEID_HasTypeDefinition, resultMask, &(dst[*refCount]));
					}
				}
				(*refCount)++;
			}
		}
	}
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_INVERSE)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		//	this is an instantiation getchild for objects (classes), for variables and links this direction delivers nothing
		if(pNode->elemtype == OV_ET_OBJECT && Ov_GetClassPtr(pNode->pobj) == pclass_ov_class){
			Ov_ForEachChild(ov_instantiation, Ov_StaticPtrCast(ov_class, pNode->pobj), pChildObject){
				childTypeDefinition.elemtype = OV_ET_OBJECT;
				childTypeDefinition.pobj = pChildObject;
				maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&childTypeDefinition, browseDescription->nodeClassMask, &access);
				if(maskMatch && (access & OV_AC_READ)){
					if(fillDescription){
						*statusCode = opcua_nsOv_fillReferenceDescription(&childTypeDefinition, 0, UA_NODEID_HasTypeDefinition, resultMask, &(dst[*refCount]));
					}
					(*refCount)++;
				}
			}
		}
	}
	return UA_STATUSCODE_GOOD;
}

UA_Int32 countReferenceDescriptions_HasTypeDefinition(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount){
	return getReferenceDescriptions_HasTypeDefinition(browseDescription, pNode,refCount, FALSE, 0, NULL, NULL);
}

UA_Int32 fillReferenceDescriptions_HasTypeDefinition(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		UA_UInt32 resultMask, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
	return getReferenceDescriptions_HasTypeDefinition(browseDescription, pNode, refCount, TRUE, resultMask, dst, statusCode);
}

/******************************************************************
 * helper for HasSubtype
 *****************************************************************/

UA_Int32 getReferenceDescriptions_HasSubtype(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		OV_BOOL fillDescription, UA_UInt32 resultMask, UA_ReferenceDescription* dst,
		UA_StatusCode* statusCode){
	OV_BOOL					maskMatch;
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_INSTPTR_ov_class		pChildClass	=	NULL;
	OV_INSTPTR_ov_class		pParentClass	=	NULL;
	OV_ELEMENT				referencedElement;
	//	maps to ov/inheritance
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_FORWARD)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		if(pNode->elemtype == OV_ET_OBJECT && Ov_GetClassPtr(pNode->pobj) == pclass_ov_class){
			Ov_ForEachChild(ov_inheritance, Ov_StaticPtrCast(ov_class, pNode->pobj), pChildClass){
				referencedElement.elemtype = OV_ET_OBJECT;
				referencedElement.pobj = Ov_PtrUpCast(ov_object, pChildClass);
				maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
				if(maskMatch && (access & OV_AC_READ)){
					if(fillDescription){
						*statusCode = opcua_nsOv_fillReferenceDescription(&referencedElement, 0, UA_NODEID_HasSubtype, resultMask, &(dst[*refCount]));
					}
					(*refCount)++;
				}
			}
		}
	}
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_INVERSE)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		if(pNode->elemtype == OV_ET_OBJECT && pNode->pobj == Ov_PtrUpCast(ov_object, pclass_ov_class)){
			pParentClass = Ov_GetParent(ov_inheritance, Ov_StaticPtrCast(ov_class, pNode->pobj));
			if(pParentClass){
				referencedElement.elemtype = OV_ET_OBJECT;
				referencedElement.pobj = Ov_PtrUpCast(ov_object, pParentClass);
				maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
				if(maskMatch && (access & OV_AC_READ)){
					if(fillDescription){
						*statusCode = opcua_nsOv_fillReferenceDescription(&referencedElement, 0, UA_NODEID_HasSubtype, resultMask, &(dst[*refCount]));
					}
					(*refCount)++;
				}
			}
		}
	}
	return UA_STATUSCODE_GOOD;
}

UA_Int32 countReferenceDescriptions_HasSubtype(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount){
	return getReferenceDescriptions_HasSubtype(browseDescription, pNode,refCount, FALSE, 0, NULL, NULL);
}

UA_Int32 fillReferenceDescriptions_HasSubtype(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		UA_UInt32 resultMask, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
	return getReferenceDescriptions_HasSubtype(browseDescription, pNode, refCount, TRUE, resultMask, dst, statusCode);
}

/******************************************************************
 * helper for HasProperty
 *****************************************************************/

UA_Int32 getReferenceDescriptions_HasProperty(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		OV_BOOL fillDescription, UA_UInt32 resultMask, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
	OV_BOOL					maskMatch;
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_ELEMENT				referencedElement;
	//	maps to variables in objects
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_FORWARD)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		if(pNode->elemtype == OV_ET_OBJECT){
			referencedElement.elemtype = OV_ET_NONE;
			ov_element_getnextpart(pNode, &referencedElement, OV_ET_VARIABLE | OV_ET_MEMBER);
			while(referencedElement.elemtype != OV_ET_NONE){
				maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
				if(maskMatch && (access & OV_AC_READ)){
					if(fillDescription){
						*statusCode = opcua_nsOv_fillReferenceDescription(&referencedElement, 0, UA_NODEID_HasProperty, resultMask, &(dst[*refCount]));
					}
					(*refCount)++;
				}
				ov_element_getnextpart(pNode, &referencedElement, OV_ET_VARIABLE | OV_ET_MEMBER);
			}
		}
	}
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_INVERSE)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		if(pNode->elemtype == OV_ET_VARIABLE || pNode->elemtype == OV_ET_MEMBER){
			referencedElement.elemtype = OV_ET_OBJECT;
			referencedElement.pobj = pNode->pobj;
			maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
			if(maskMatch && (access & OV_AC_READ)){
				if(fillDescription){
					*statusCode = opcua_nsOv_fillReferenceDescription(&referencedElement, 0, UA_NODEID_HasProperty, resultMask, &(dst[*refCount]));
				}
				(*refCount)++;
			}
		}
	}
	return UA_STATUSCODE_GOOD;
}

UA_Int32 countReferenceDescriptions_HasProperty(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount){
	return getReferenceDescriptions_HasProperty(browseDescription, pNode,refCount, FALSE, 0, NULL, NULL);
}

UA_Int32 fillReferenceDescriptions_HasProperty(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		UA_UInt32 resultMask, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
	return getReferenceDescriptions_HasProperty(browseDescription, pNode, refCount, TRUE, resultMask, dst, statusCode);
}

/******************************************************************
 * helper for HasComponent
 *****************************************************************/

UA_Int32 getReferenceDescriptions_HasComponent(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		OV_BOOL fillDescription, UA_UInt32 resultMask, UA_ReferenceDescription* dst,
		UA_StatusCode* statusCode){
	OV_BOOL					maskMatch;
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_ELEMENT				referencedElement;
	//	maps to variables in objects
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_FORWARD)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		if(pNode->elemtype == OV_ET_OBJECT){
			referencedElement.elemtype = OV_ET_NONE;
			ov_element_getnextpart(pNode, &referencedElement, OV_ET_OBJECT);
			while(referencedElement.elemtype != OV_ET_NONE){
				maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
				if(maskMatch && (access & OV_AC_READ)){
					if(fillDescription){
						*statusCode = opcua_nsOv_fillReferenceDescription(&referencedElement, 0, UA_NODEID_HasComponent, resultMask, &(dst[*refCount]));
					}
					(*refCount)++;
				}
				ov_element_getnextpart(pNode, &referencedElement, OV_ET_OBJECT);
			}
		}
	}
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_INVERSE)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		if(pNode->elemtype == OV_ET_OBJECT){
			if(pNode->pobj->v_pouterobject){
				referencedElement.elemtype = OV_ET_OBJECT;
				referencedElement.pobj = pNode->pobj->v_pouterobject;
				maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
				if(maskMatch && (access & OV_AC_READ)){
					if(fillDescription){
						*statusCode = opcua_nsOv_fillReferenceDescription(&referencedElement, 0, UA_NODEID_HasComponent, resultMask, &(dst[*refCount]));
					}
					(*refCount)++;
				}
			}
		}
	}
	return UA_STATUSCODE_GOOD;
}

UA_Int32 countReferenceDescriptions_HasComponent(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount){
	return getReferenceDescriptions_HasComponent(browseDescription, pNode,refCount, FALSE, 0, NULL, NULL);
}

UA_Int32 fillReferenceDescriptions_HasComponent(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		UA_UInt32 resultMask, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
	return getReferenceDescriptions_HasComponent(browseDescription, pNode, refCount, TRUE, resultMask, dst, statusCode);
}

/******************************************************************
 * helper for OVReferences
 *****************************************************************/

UA_Int32 getReferenceDescriptions_OvReferences(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, signed char flag, OV_INSTPTR_ov_association passoc,
		OV_UINT* refCount, OV_BOOL fillDescription, UA_UInt32 resultMask, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
	OV_BOOL					maskMatch;
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_ELEMENT				linkElement;
	OV_ELEMENT				referencedElement;

	if(flag < 0){	// -1: association definde by passoc; -2 all associations
		if((browseDescription->browseDirection == UA_BROWSEDIRECTION_FORWARD)
				|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
			if(pNode->elemtype == OV_ET_OBJECT){
				linkElement.elemtype = OV_ET_NONE;
				ov_element_getnextpart(pNode, &linkElement, OV_ET_PARENTLINK);
				while(linkElement.elemtype != OV_ET_NONE){
					if(flag == -2 || linkElement.elemunion.passoc == passoc){
						if(linkElement.elemunion.passoc->v_assoctype == OV_AT_ONE_TO_MANY){
							referencedElement.elemtype = OV_ET_OBJECT;
							Ov_Association_ForEachChild(linkElement.elemunion.passoc, pNode->pobj, referencedElement.pobj){
								maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
								if(maskMatch && (access & OV_AC_READ)){
									if(fillDescription){
										*statusCode = opcua_nsOv_fillReferenceDescription(&referencedElement, 1,
												linkElement.elemunion.passoc->v_idL, resultMask, &(dst[*refCount]));
									}
									(*refCount)++;
								}
							}
						} else if(linkElement.elemunion.passoc->v_assoctype == OV_AT_ONE_TO_ONE) {
							referencedElement.elemtype = OV_ET_OBJECT;
							referencedElement.pobj = Ov_Association_GetChild(linkElement.elemunion.passoc, pNode->pobj);
							if(referencedElement.pobj){
								maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
								if(maskMatch && (access & OV_AC_READ)){
									if(fillDescription){
										*statusCode = opcua_nsOv_fillReferenceDescription(&referencedElement, 1,
												linkElement.elemunion.passoc->v_idL, resultMask, &(dst[*refCount]));
									}
									(*refCount)++;
								}
							}
						}
						if(flag == -1){
							break;
						}
					}
					ov_element_getnextpart(pNode, &linkElement, OV_ET_PARENTLINK);
				}
			}
		}
		if((browseDescription->browseDirection == UA_BROWSEDIRECTION_INVERSE)
				|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
			if(pNode->elemtype == OV_ET_OBJECT){
				linkElement.elemtype = OV_ET_NONE;
				ov_element_getnextpart(pNode, &linkElement, OV_ET_CHILDLINK);
				while(linkElement.elemtype != OV_ET_NONE){
					if(flag == -2 || linkElement.elemunion.passoc == passoc){
						referencedElement.elemtype = OV_ET_OBJECT;
						referencedElement.pobj = Ov_Association_GetParent(linkElement.elemunion.passoc, pNode->pobj);
						if(referencedElement.pobj){
							maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
							if(maskMatch && (access & OV_AC_READ)){
								if(fillDescription){
									*statusCode = opcua_nsOv_fillReferenceDescription(&referencedElement, 1,
											linkElement.elemunion.passoc->v_idL, resultMask, &(dst[*refCount]));
								}
								(*refCount)++;
							}
						}
						if(flag == -1){
							break;
						}
					}
					ov_element_getnextpart(pNode, &linkElement, OV_ET_CHILDLINK);
				}
			}
		}
	}
	return UA_STATUSCODE_GOOD;
}

UA_Int32 countReferenceDescriptions_OvReferences(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, signed char flag, OV_INSTPTR_ov_association passoc,
		OV_UINT* refCount){
	return getReferenceDescriptions_OvReferences(browseDescription, pNode, flag, passoc, refCount, FALSE, 0, NULL, NULL);
}

UA_Int32 fillReferenceDescriptions_OvReferences(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, signed char flag, OV_INSTPTR_ov_association passoc,
		OV_UINT* refCount, UA_UInt32 resultMask, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
	return getReferenceDescriptions_OvReferences(browseDescription, pNode, flag, passoc, refCount, TRUE, resultMask, dst, statusCode);
}

/***********************************************************************
 * main browse function
 **********************************************************************/

OV_DLLFNCEXPORT UA_Int32 opcua_nodeStoreFunctions_browseNodes(
		void *ensHandle,
		const UA_RequestHeader *requestHeader,
		UA_BrowseDescription *browseDescriptions,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_UInt32 requestedMaxReferencesPerNode,
		UA_BrowseResult *browseResults,
		UA_DiagnosticInfo *diagnosticInfos
) {
	OV_ELEMENT*				pNode		=	NULL;
	OV_UINT					index, j;
	OV_PATH					nodePath;
	OV_PATH					assocPath;
	UA_Int32				uaResult;
	OV_INSTPTR_ov_association	pAssoc	=	NULL;
	OV_UINT					refCount	=	0;
	OV_UINT					refCountBefore	=	0;


#define UAREFS_LENGTH		12
#define OVREFSARRAY_CHUNKSIZE	32
	signed char				uaReferences [UAREFS_LENGTH];	//array for ua-references: 0: leave out, -1: to do, -1: done; specific for ovReferences: -2 do all
#define INDEX_References					0
#define INDEX_NonHierarchicalReferences		1
#define INDEX_HierarchicalReferences		2
#define INDEX_HasChild						3
#define INDEX_Organizes						4
#define INDEX_HasTypeDefinition				5
#define INDEX_Aggregates					6
#define INDEX_HasSubtype					7
#define INDEX_HasProperty					8
#define INDEX_HasComponent					9
#define INDEX_OvReference					10

	for(index = 0; index<indicesSize;index++){
		ov_memstack_lock();
		uaResult = opcua_nodeStoreFunctions_resolveNodeIdToPath(browseDescriptions[indices[index]].nodeId, &nodePath);
		if(uaResult != UA_STATUSCODE_GOOD){
			KS_logfile_debug(("nodeStore_browseNodes: could not resolve node for index %u in list. Skipping.", index));
			browseResults[indices[index]].statusCode = UA_STATUSCODE_BADNODEIDUNKNOWN;
			ov_memstack_unlock();
			continue;
		}
		pNode = &(nodePath.elements[nodePath.size - 1]);
		for(j=0; j<UAREFS_LENGTH; j++){
			uaReferences[j] = 0;
		}
/******************************************************************************************************************************
 * mark all reference types to check
 *****************************************************************************************************************************/
		//	check all supported basic (ua) references
		if(browseDescriptions[indices[index]].referenceTypeId.namespaceIndex == 0){	//ns0
			if(browseDescriptions[indices[index]].referenceTypeId.identifierType != UA_NODEIDTYPE_NUMERIC){
				browseResults[indices[index]].statusCode = UA_STATUSCODE_BADNODEIDREJECTED;
				ov_memstack_unlock();
				continue;
			} else {
				switch(browseDescriptions[indices[index]].referenceTypeId.identifier.numeric){
				case UA_NODEID_References:
					uaReferences[INDEX_References] = -1;
					if(browseDescriptions[indices[index]].includeSubtypes == UA_TRUE){
						for(j=1; j<UAREFS_LENGTH; j++){
							uaReferences[j] = -1;
						}
						uaReferences[INDEX_OvReference] = -2;
					}
					break;
				case UA_NODEID_HierarchicalReferences:
					uaReferences[INDEX_HierarchicalReferences] = -1;
					if(browseDescriptions[indices[index]].includeSubtypes == UA_TRUE){
						uaReferences[INDEX_HasChild]	=	-1;
						uaReferences[INDEX_Organizes]	=	-1;
						uaReferences[INDEX_Aggregates]	=	-1;
						uaReferences[INDEX_HasSubtype]	=	-1;
						uaReferences[INDEX_HasProperty]	=	-1;
						uaReferences[INDEX_HasComponent]	=	-1;
						uaReferences[INDEX_OvReference] = -2;
					}
					break;
				case UA_NODEID_NonHierarchicalReferences:
					uaReferences[INDEX_NonHierarchicalReferences] = -1;
					if(browseDescriptions[indices[index]].includeSubtypes == UA_TRUE){
						uaReferences[INDEX_HasTypeDefinition]	=	-1;
					}
					break;
				case UA_NODEID_HasChild:
					uaReferences[INDEX_HasChild] = -1;
					if(browseDescriptions[indices[index]].includeSubtypes == UA_TRUE){
						uaReferences[INDEX_Aggregates]	=	-1;
						uaReferences[INDEX_HasSubtype]	=	-1;
						uaReferences[INDEX_HasProperty]	=	-1;
						uaReferences[INDEX_HasComponent]	=	-1;
					}
					break;
				case UA_NODEID_Organizes:
					uaReferences[INDEX_Organizes] = -1;
					break;
				case UA_NODEID_HasTypeDefinition:
					uaReferences[INDEX_HasTypeDefinition] = -1;
					break;
				case UA_NODEID_Aggregates:
					uaReferences[INDEX_Aggregates] = -1;
					if(browseDescriptions[indices[index]].includeSubtypes == UA_TRUE){
						uaReferences[INDEX_HasProperty]	=	-1;
						uaReferences[INDEX_HasComponent]	=	-1;
					}
					break;
				case UA_NODEID_HasSubtype:
					uaReferences[INDEX_HasSubtype] = -1;
					break;
				case UA_NODEID_HasProperty:
					uaReferences[INDEX_HasProperty] = -1;
					break;
				case UA_NODEID_HasComponent:
					uaReferences[INDEX_HasComponent] = -1;
					break;
				}
			}
		}
		// check ov-references
		if(browseDescriptions[indices[index]].referenceTypeId.namespaceIndex == 1){
			uaResult = opcua_nodeStoreFunctions_resolveNodeIdToPath(browseDescriptions[indices[index]].referenceTypeId, &assocPath);
			if(uaResult != UA_STATUSCODE_GOOD){
				KS_logfile_debug(("nodeStore_browseNodes: could not resolve association for index %u in list. Skipping.", index));
				browseResults[indices[index]].statusCode = UA_STATUSCODE_BADNODEIDUNKNOWN;
				ov_memstack_unlock();
				continue;
			}
			if(assocPath.elements[assocPath.size - 1].elemtype == OV_ET_OBJECT){
				pAssoc = Ov_DynamicPtrCast(ov_association, assocPath.elements[assocPath.size - 1].pobj);
			} else {
				pAssoc = NULL;
			}
			if(pAssoc){
				uaReferences[INDEX_OvReference] = -1;
			} else {
				KS_logfile_debug(("nodeStore_browseNodes: nodeId for index %u in list does not match a ReferenceType. Skipping.", index));
				browseResults[indices[index]].statusCode = UA_STATUSCODE_BADTYPEMISMATCH;
				ov_memstack_unlock();
				continue;
			}
		} else if(uaReferences[INDEX_OvReference] == -2){
			pAssoc = NULL;
		}
/*********************************************************************************************************************************
 * count and add up results for all reference types
 ********************************************************************************************************************************/
		if(uaReferences[INDEX_References] == -1){
			//	do nothing as this reference type is abstract
			// just for readability - compiler should erase this if during optimization
		}
		if(uaReferences[INDEX_NonHierarchicalReferences] == -1){
			//	do nothing as this reference type is abstract
			// just for readability - compiler should erase this if during optimization
		}
		if(uaReferences[INDEX_HierarchicalReferences] == -1){
			//	do nothing as this reference type is abstract
			// just for readability - compiler should erase this if during optimization
		}
		if(uaReferences[INDEX_HasChild] == -1){
			//	do nothing as this reference type is abstract
			// just for readability - compiler should erase this if during optimization
		}
		if(uaReferences[INDEX_Aggregates] == -1){
			//	do nothing as this reference type is abstract
			// just for readability - compiler should erase this if during optimization
		}
		/*************
		 * organizes
		 *************/
		if(uaReferences[INDEX_Organizes] == -1){
			countReferenceDescriptions_Organizes(&(browseDescriptions[indices[index]]), pNode, &refCount);
			if(refCount == refCountBefore){
				uaReferences[INDEX_Organizes] = 1;
			}
			refCountBefore = refCount;
		}
		/*************
		 * HasTypeDefinition
		 *************/
		if(uaReferences[INDEX_HasTypeDefinition] == -1){
			countReferenceDescriptions_HasTypeDefinition(&(browseDescriptions[indices[index]]), pNode, &refCount);
			if(refCount == refCountBefore){
				uaReferences[INDEX_HasTypeDefinition] = 1;
			}
			refCountBefore = refCount;
		}
		/*************
		 * HasSubtype
		 *************/
		if(uaReferences[INDEX_HasSubtype] == -1){
			countReferenceDescriptions_HasSubtype(&(browseDescriptions[indices[index]]), pNode, &refCount);
			if(refCount == refCountBefore){
				uaReferences[INDEX_HasSubtype] = 1;
			}
			refCountBefore = refCount;
		}
		/*************
		 * HasProperty
		 *************/
		if(uaReferences[INDEX_HasProperty] == -1){
			countReferenceDescriptions_HasProperty(&(browseDescriptions[indices[index]]), pNode, &refCount);
			if(refCount == refCountBefore){
				uaReferences[INDEX_HasProperty] = 1;
			}
			refCountBefore = refCount;
		}
		/*************
		 * HasComponent
		 *************/
		if(uaReferences[INDEX_HasComponent] == -1){
			countReferenceDescriptions_HasComponent(&(browseDescriptions[indices[index]]), pNode, &refCount);
			if(refCount == refCountBefore){
				uaReferences[INDEX_HasComponent] = 1;
			}
			refCountBefore = refCount;
		}
		/*************
		 * OvReferences
		 *************/
		if(uaReferences[INDEX_OvReference] < 0){
			countReferenceDescriptions_OvReferences(&(browseDescriptions[indices[index]]), pNode, uaReferences[INDEX_OvReference], pAssoc, &refCount);
			if(refCount == refCountBefore){
				uaReferences[INDEX_OvReference] = 1;
			}
			refCountBefore = refCount;
		}

		/*************************************************************************************************************
		 * create Array
		 ************************************************************************************************************/
		browseResults[indices[index]].references = UA_Array_new(&UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION], refCount);
		if(!browseResults[indices[index]].references && refCount>0){
			browseResults[indices[index]].statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
			ov_memstack_unlock();
			break;
		}
		browseResults[indices[index]].referencesSize = refCount;
		refCount = 0;
		/*********************************************************************************************************************************
		 * fill in results for all reference types
		 ********************************************************************************************************************************/

		/*************
		 * organizes
		 *************/
		if(uaReferences[INDEX_Organizes] == -1){
			fillReferenceDescriptions_Organizes(&(browseDescriptions[indices[index]]), pNode, &refCount, browseDescriptions[indices[index]].resultMask,
					browseResults[indices[index]].references, &(browseResults[indices[index]].statusCode));
		}
		/*************
		 * HasTypeDefinition
		 *************/
		if(uaReferences[INDEX_HasTypeDefinition] == -1){
			fillReferenceDescriptions_HasTypeDefinition(&(browseDescriptions[indices[index]]), pNode, &refCount, browseDescriptions[indices[index]].resultMask,
								browseResults[indices[index]].references, &(browseResults[indices[index]].statusCode));
		}
		/*************
		 * HasSubtype
		 *************/
		if(uaReferences[INDEX_HasSubtype] == -1){
			fillReferenceDescriptions_HasSubtype(&(browseDescriptions[indices[index]]), pNode, &refCount, browseDescriptions[indices[index]].resultMask,
								browseResults[indices[index]].references, &(browseResults[indices[index]].statusCode));
		}
		/*************
		 * HasProperty
		 *************/
		if(uaReferences[INDEX_HasProperty] == -1){
			fillReferenceDescriptions_HasProperty(&(browseDescriptions[indices[index]]), pNode, &refCount, browseDescriptions[indices[index]].resultMask,
								browseResults[indices[index]].references, &(browseResults[indices[index]].statusCode));
		}
		/*************
		 * HasComponent
		 *************/
		if(uaReferences[INDEX_HasComponent] == -1){
			fillReferenceDescriptions_HasComponent(&(browseDescriptions[indices[index]]), pNode, &refCount, browseDescriptions[indices[index]].resultMask,
								browseResults[indices[index]].references, &(browseResults[indices[index]].statusCode));
		}
		/*************
		 * OvReferences
		 *************/
		if(uaReferences[INDEX_OvReference] < 0){
			fillReferenceDescriptions_OvReferences(&(browseDescriptions[indices[index]]), pNode, uaReferences[INDEX_OvReference], pAssoc,
								&refCount, browseDescriptions[indices[index]].resultMask, browseResults[indices[index]].references,
								&(browseResults[indices[index]].statusCode));
		}

		ov_memstack_unlock();
	}
	return UA_STATUSCODE_GOOD;
}
