#ifndef BASE_H
#define BASE_H

#ifndef UNICODE
#define UNICODE
#endif

#include <ntwin.h>
#include <intrin.h>
#include <wchar.h>
#include <stdio.h>

// nonstandard extension used : nameless struct/union
#pragma warning(disable: 4201)
// nonstandard extension used : bit field types other than int
#pragma warning(disable: 4214)
// 'function': was declared deprecated
#pragma warning(disable: 4996)

#define FASTCALL __fastcall

#define PTR_ADD_OFFSET(Pointer, Offset) ((PVOID)((ULONG_PTR)(Pointer) + (ULONG_PTR)(Offset)))
#define PTR_SUB_OFFSET(Pointer, Offset) ((PVOID)((ULONG_PTR)(Pointer) - (ULONG_PTR)(Offset)))
#define REBASE_ADDRESS(Pointer, OldBase, NewBase) \
    ((PVOID)((ULONG_PTR)(Pointer) - (ULONG_PTR)(OldBase) + (ULONG_PTR)(NewBase)))

#define PAGE_SIZE 0x1000

#define WCHAR_LONG_TO_SHORT(Long) (((Long) & 0xff) | (((Long) & 0xff0000) >> 16))

#define PH_TIMEOUT_TO_MS ((LONGLONG)1 * 10 * 1000)
#define PH_TIMEOUT_TO_SEC (PH_TIMEOUT_TO_MS * 1000)

#define PhRaiseStatus(Status) RaiseException(Status, 0, 0, NULL)

// Annotations

/**
 * Indicates that a function assumes the relevant 
 * locks have been acquired.
 */
#define __assumeLocked

/**
 * Indicates that a function may raise a software
 * exception.
 *
 * \remarks Do not use this annotation for 
 * temporary usages of exceptions, e.g. unimplemented 
 * functions.
 */
#define __mayRaise

/**
 * Indicates that a return value or output parameter 
 * will not alias the given parameter.
 */
#define __outRestrict(param)

// Strings

#define PH_INT32_STR_LEN 12
#define PH_INT32_STR_LEN_1 (PH_INT32_STR_LEN + 1)

#define PH_INT64_STR_LEN 50
#define PH_INT64_STR_LEN_1 (PH_INT64_STR_LEN + 1)

#define PH_PTR_STR_LEN 24
#define PH_PTR_STR_LEN_1 (PH_PTR_STR_LEN + 1)

#define STR_EQUAL(Str1, Str2) (strcmp(Str1, Str2) == 0)
#define WSTR_EQUAL(Str1, Str2) (wcscmp(Str1, Str2) == 0)

#define STR_IEQUAL(Str1, Str2) (stricmp(Str1, Str2) == 0)
#define WSTR_IEQUAL(Str1, Str2) (wcsicmp(Str1, Str2) == 0)

// Sorting

typedef enum _PH_SORT_ORDER
{
    AscendingSortOrder,
    DescendingSortOrder,
    NoSortOrder
} PH_SORT_ORDER, *PPH_SORT_ORDER;

FORCEINLINE INT PhModifySort(
    __in INT Result,
    __in PH_SORT_ORDER Order
    )
{
    if (Order == AscendingSortOrder)
        return Result;
    else if (Order == DescendingSortOrder)
        return -Result;
    else
        return Result;
}

#define PH_BUILTIN_COMPARE(value1, value2) \
    if (value1 > value2) \
        return 1; \
    else if (value1 < value2) \
        return -1; \
    \
    return 0

FORCEINLINE int intcmp(
    __in int value1,
    __in int value2
    )
{
    PH_BUILTIN_COMPARE(value1, value2);
}

FORCEINLINE int uintcmp(
    __in unsigned int value1,
    __in unsigned int value2
    )
{
    PH_BUILTIN_COMPARE(value1, value2);
}

FORCEINLINE int int64cmp(
    __in __int64 value1,
    __in __int64 value2
    )
{
    PH_BUILTIN_COMPARE(value1, value2);
}

FORCEINLINE int uint64cmp(
    __in unsigned __int64 value1,
    __in unsigned __int64 value2
    )
{
    PH_BUILTIN_COMPARE(value1, value2);
}

FORCEINLINE int uintptrcmp(
    __in ULONG_PTR value1,
    __in ULONG_PTR value2
    )
{
    PH_BUILTIN_COMPARE(value1, value2);
}

// Misc.

FORCEINLINE ULONG PhCountBits(
    __in ULONG Value
    )
{
    ULONG count = 0;

    while (Value)
    {
        count++;
        Value &= Value - 1;
    }

    return count;
}

FORCEINLINE ULONG PhRoundNumber(
    __in ULONG Value,
    __in ULONG Multiplier
    )
{
    ULONG newValue;

    newValue = Value / Multiplier * Multiplier;

    // This new value has the multiplier truncated.
    // E.g. 1099 / 100 * 100 = 1000.
    // If the difference is less than half the multiplier, 
    // use the new value.
    // E.g.
    // 1099 -> 1000 (100). 1099 - 1000 >= 50, so use 
    // the new value plus the multiplier.
    // 1010 -> 1000 (100). 1010 - 1000 < 50, so use 
    // the new value.

    if (Value - newValue < Multiplier / 2)
        return newValue;
    else
        return newValue + Multiplier;
}

FORCEINLINE PVOID PhGetProcAddress(
    __in PWSTR LibraryName,
    __in PSTR ProcName
    )
{
    return GetProcAddress(GetModuleHandle(LibraryName), ProcName);
}

FORCEINLINE VOID PhPrintInt32(
    __out PWSTR Destination,
    __in LONG Int32
    )
{
    _snwprintf(Destination, PH_INT32_STR_LEN, L"%d", Int32);
}

FORCEINLINE VOID PhPrintUInt32(
    __out PWSTR Destination,
    __in ULONG UInt32
    )
{
    _snwprintf(Destination, PH_INT32_STR_LEN, L"%u", UInt32);
}

FORCEINLINE VOID PhPrintInt64(
    __out PWSTR Destination,
    __in LONG64 Int64
    )
{
    _snwprintf(Destination, PH_INT64_STR_LEN, L"%I64d", Int64);
}

FORCEINLINE VOID PhPrintUInt64(
    __out PWSTR Destination,
    __in ULONG64 UInt64
    )
{
    _snwprintf(Destination, PH_INT64_STR_LEN, L"%I64u", UInt64);
}

FORCEINLINE VOID PhPrintPointer(
    __out PWSTR Destination,
    __in PVOID Pointer
    )
{
    _snwprintf(Destination, PH_PTR_STR_LEN, L"0x%Ix", Pointer);
}

#ifdef _M_IX86

FORCEINLINE PVOID _InterlockedCompareExchangePointer(
    __inout PVOID volatile *Destination,
    __in PVOID Exchange,
    __in PVOID Comparand
    )
{
    return (PVOID)_InterlockedCompareExchange(
        (PLONG_PTR)Destination,
        (LONG_PTR)Exchange,
        (LONG_PTR)Comparand
        );
}

FORCEINLINE PVOID _InterlockedExchangePointer(
    __inout PVOID volatile *Destination,
    __in PVOID Exchange
    )
{
    return (PVOID)_InterlockedExchange(
        (PLONG_PTR)Destination,
        (LONG_PTR)Exchange
        );
}

#endif

#endif
