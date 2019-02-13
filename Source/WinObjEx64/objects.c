/*******************************************************************************
*
*  (C) COPYRIGHT AUTHORS, 2017 - 2019
*
*  TITLE:       OBJECTS.C
*
*  VERSION:     1.72
*
*  DATE:        10 Feb 2019
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
* ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
* TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
* PARTICULAR PURPOSE.
*
*******************************************************************************/

#include "global.h"

/*
* ObManagerComparerName
*
* Purpose:
*
* Support comparer routine to work with objects array.
*
*/
INT ObManagerComparerName(
    _In_ PCVOID FirstObject,
    _In_ PCVOID SecondObject
)
{
    WOBJ_TYPE_DESC *firstObject = (WOBJ_TYPE_DESC*)FirstObject;
    WOBJ_TYPE_DESC *secondObject = (WOBJ_TYPE_DESC*)SecondObject;

    if (firstObject == secondObject) 
        return 0;

    return (_strcmpi(firstObject->Name, secondObject->Name));
}

/*
* ObManagerGetNameByIndex
*
* Purpose:
*
* Returns object name by index of known type.
*
*/
LPWSTR ObManagerGetNameByIndex(
    _In_ ULONG TypeIndex
)
{
    ULONG nIndex;

    for (nIndex = TYPE_FIRST; nIndex < TYPE_LAST; nIndex++) {
        if (g_ObjectTypes[nIndex].Index == (WOBJ_OBJECT_TYPE)TypeIndex)
            return g_ObjectTypes[nIndex].Name;
    }

    return OBTYPE_NAME_UNKNOWN;
}

/*
* ObManagerGetImageIndexByTypeIndex
*
* Purpose:
*
* Returns object image index by index of known type.
*
*
*/
UINT ObManagerGetImageIndexByTypeIndex(
    _In_ ULONG TypeIndex
)
{
    ULONG nIndex;

    for (nIndex = TYPE_FIRST; nIndex < TYPE_LAST; nIndex++) {
        if (g_ObjectTypes[nIndex].Index == (WOBJ_OBJECT_TYPE)TypeIndex)
            return g_ObjectTypes[nIndex].ImageIndex;
    }

    return ObjectTypeUnknown;
}

/*
* ObManagerGetEntryByTypeName
*
* Purpose:
*
* Returns object description entry by type name.
*
*/
WOBJ_TYPE_DESC *ObManagerGetEntryByTypeName(
    _In_opt_ LPCWSTR lpTypeName
)
{
    WOBJ_TYPE_DESC SearchItem;
    WOBJ_TYPE_DESC *Result;

    if (lpTypeName == NULL) {
        return &g_ObjectTypes[ObjectTypeUnknown];
    }

    SearchItem.Name = (LPWSTR)lpTypeName;

    Result = (WOBJ_TYPE_DESC*)supBSearch((PCVOID)&SearchItem,
        (PCVOID)&g_ObjectTypes,
        RTL_NUMBER_OF(g_ObjectTypes),
        sizeof(WOBJ_TYPE_DESC),
        ObManagerComparerName);   

    if (Result == NULL) {
        Result = &g_ObjectTypes[ObjectTypeUnknown];
    }

    return Result;
}

/*
* ObManagerGetIndexByTypeName
*
* Purpose:
*
* Returns object index of known type.
*
*/
UINT ObManagerGetIndexByTypeName(
    _In_opt_ LPCWSTR lpTypeName
)
{
    WOBJ_TYPE_DESC SearchItem;
    WOBJ_TYPE_DESC *Result;

    if (lpTypeName == NULL) {
        return ObjectTypeUnknown;
    }

    SearchItem.Name = (LPWSTR)lpTypeName;

    Result = (WOBJ_TYPE_DESC*)supBSearch((PCVOID)&SearchItem,
        (PCVOID)&g_ObjectTypes, 
        RTL_NUMBER_OF(g_ObjectTypes), 
        sizeof(WOBJ_TYPE_DESC), 
        ObManagerComparerName);

    if (Result) {
        return Result->Index;
    }
    else {
        return ObjectTypeUnknown;
    }
}

/*
* ObManagerGetImageIndexByTypeName
*
* Purpose:
*
* Returns object image index of known type.
*
*/
UINT ObManagerGetImageIndexByTypeName(
    _In_opt_ LPCWSTR lpTypeName
)
{
    WOBJ_TYPE_DESC SearchItem;
    WOBJ_TYPE_DESC *Result;

    if (lpTypeName == NULL) {
        return ObjectTypeUnknown;
    }

    SearchItem.Name = (LPWSTR)lpTypeName;

    Result = (WOBJ_TYPE_DESC*)supBSearch((PCVOID)&SearchItem,
        (PCVOID)&g_ObjectTypes,
        RTL_NUMBER_OF(g_ObjectTypes),
        sizeof(WOBJ_TYPE_DESC),
        ObManagerComparerName);

    if (Result) {
        return Result->ImageIndex;
    }
    else {
        return ObjectTypeUnknown;
    }
}

/*
* ObManagerLoadImageList
*
* Purpose:
*
* Create and load image list from icon resource type.
*
*/
HIMAGELIST ObManagerLoadImageList(
    VOID
)
{
    UINT       i;
    HIMAGELIST list;
    HICON      hIcon;

    list = ImageList_Create(
        16, 
        16, 
        ILC_COLOR32 | ILC_MASK,
        TYPE_LAST, 
        8);

    if (list) {
        for (i = TYPE_FIRST; i <= TYPE_LAST; i++) { //must include ObjectTypeUnknown
               
            hIcon = (HICON)LoadImage(g_WinObj.hInstance, 
                MAKEINTRESOURCE(g_ObjectTypes[i].ResourceImageId),
                IMAGE_ICON, 
                16, 
                16, 
                LR_DEFAULTCOLOR);

            if (hIcon) {
                g_ObjectTypes[i].ImageIndex = ImageList_ReplaceIcon(list, -1, hIcon);
                DestroyIcon(hIcon);
            }
            else {
                g_ObjectTypes[i].ImageIndex = I_IMAGENONE;
            }
        }
    }
    return list;
}
