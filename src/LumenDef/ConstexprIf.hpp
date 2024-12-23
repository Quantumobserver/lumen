
#pragma once

#define CONSTEXPR_IF_GLM_NORMALIZE 

#define CONSTEXPR_IF_SF_SPRITE_DEFAULT_CONSTRUCTOR
#define CONSTEXPR_IF_SF_SPRITE_MOVE_ASSIGNMENT
#define CONSTEXPR_IF_SF_SPRITE_GET_TEXTURE
#define CONSTEXPR_IF_SF_SPRITE_SET_TEXTURE
#define CONSTEXPR_IF_SF_SPRITE_SET_POSITION
#define CONSTEXPR_IF_SF_SPRITE_SET_SCALE
#define CONSTEXPR_IF_SF_SPRITE_GET_SCALE
#define CONSTEXPR_IF_SF_TEXTURE_DEFAULT_CONSTRUCTOR
#define CONSTEXPR_IF_SF_TEXTURE_MOVE_ASSIGNMENT
#define CONSTEXPR_IF_SF_RECTANGLE_SHAPE_CONSTRUCTOR
#define CONSTEXPR_IF_SF_EVENT

#define CONSTEXPR_IF_STD_VECTOR_MOVE_ASSIGNMENT
#define CONSTEXPR_IF_STD_VECTOR_CLEAR

#define CONSTEXPR_IF_ATOMIC_PLUS_PLUS

#if __cplusplus >= 201103L // C++11
        #define CONSTEXPR_IF_CXX_11 constexpr
#else
        #define CONSTEXPR_IF_CXX_11
#endif

#if __cplusplus >= 201402L // C++14
        #define CONSTEXPR_IF_CXX_14 constexpr
#else
        #define CONSTEXPR_IF_CXX_14
#endif

#if __cplusplus >= 201703L // C++17
        #define CONSTEXPR_IF_CXX_17 constexpr
#else
        #define CONSTEXPR_IF_CXX_17
#endif

#if __cplusplus >= 202002L // C++20
        #define CONSTEXPR_IF_CXX_20 constexpr
#else
        #define CONSTEXPR_IF_CXX_20
#endif

#if __cplusplus >= 202302L // C++23
        #define CONSTEXPR_IF_CXX_23 constexpr
#else
        #define CONSTEXPR_IF_CXX_23
#endif
