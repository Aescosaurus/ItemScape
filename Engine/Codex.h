#pragma once

#include <map>
#include <string>
#include "Font.h"
#include "Surface.h"

template<typename T>
class Codex
{
private:
	class Entry
	{
	public:
		Entry()
			:
			pResource( nullptr )
		{}
		// Give me a new T to nom on.
		Entry( T* inputData )
			:
			pResource( inputData )
		{}
		Entry( const Entry& rhs )
		{
			*this = rhs;
		}
		Entry& operator=( const Entry& rhs )
		{ // Shallow copy pls.
			pResource = rhs.pResource;
			return( *this );
		} // Yes I know that's default behavior.
	public:
		T* pResource;
	};
public:
	// Gives pointer to resource if it exists already or
	//  loads it if it doesn't already exist.
	static const T* Fetch( const std::string& path )
	{
		Codex& codex = Generate();

		const auto entryIt = codex.entries.find( path );
		if( entryIt == codex.entries.end() )
		{
			T* pData = new T{ path };
			codex.entries.insert( { path,Entry{ pData } } );
		}

		return( codex.entries[path].pResource );
	}
	static void Purge()
	{
		Codex& codex = Generate();
		for( auto& pair : codex.entries )
		{
			delete( pair.second.pResource );
		}
		codex.entries.clear();
	}
private:
	static Codex& Generate()
	{
		static Codex codex;
		return( codex );
	}
	~Codex()
	{
		for( auto& pair : entries )
		{
			delete( pair.second.pResource );
		}
	}
private:
	std::map<std::string,Entry> entries;
};

typedef Codex<Font> FontCodex;
typedef Codex<Surface> SurfCodex;

typedef const Font* CFontPtr;
typedef const Surface* CSurfPtr;