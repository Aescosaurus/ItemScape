#pragma once

#include <vector>
#include "resource.h"
#include "Utils.h"
#include <algorithm>
#include <string>

// HEY!!
//  Use this class like Codex<Surface>::Retrieve( L"Images\\chilihead.bmp" );
//  or Codex<SoundEffect>::Retrieve( L"Sounds\\chili_hurt.sfx" );

// we will make this a singleton (there can be only one!)
template<class T>
class Codex
{
private:
	class Entry
	{
	public:
		Entry( const std::string& key,const T* pResource )
			:
			key( key ),
			pResource( pResource )
		{}
		std::string key;
		// this pointer owns the resource on the heap
		// put the resources on the heap to keep them STABLE
		const T* pResource;
	};
public:
	// retrieve a ptr to resource based on string (load if not exist)
	static const T* Retrieve( const std::string& key )
	{
		return Get()._Retrieve( key );
	}
	static const Surface* RetrieveSurf( const std::string& key,const Vei2& expandedAmount )
	{
		return( Get()._RetrieveSurf( key,expandedAmount ) );
	}
	// remove all entries from codex
	static void Purge()
	{
		Get()._Purge();
	}
private:
	Codex() = default;
	~Codex()
	{
		for( auto& e : entries )
		{
			delete e.pResource;
		}
	}
	// retrieve a ptr to resource based on string (load if not exist)
	const T* _Retrieve( const std::string& key )
	{
		// find position of resource OR where resource should be (with bin search)
		auto i = std::lower_bound( entries.begin(),entries.end(),key,
			[]( const Entry& e,const std::string& key )
		{
			return e.key < key;
		}
		);
		// if resource does not exist (i.e. i does not point to resource with right key)
		// load, store in sorted pos in codex, and set i to point to it
		if( i == entries.end() || i->key != key )
		{
			// construct entry in-place (i is the pos where the key *should* be)
			i = entries.emplace( i,key,new T( key ) );
		}
		// return ptr to resource in codex
		return i->pResource;
	}
	const Surface* _RetrieveSurf( const std::string& key,const Vei2& expandedSize )
	{
		auto i = std::lower_bound( entries.begin(),entries.end(),key,
			[]( const Entry& e,const std::string& key )
		{
			return e.key < key;
		}
		);
		if( i == entries.end() || i->key != key )
		{
			i = entries.emplace( i,key,new Surface{ key,expandedSize } );
		}
		return i->pResource;
	}
	// remove all entries from codex
	void _Purge()
	{
		for( auto& e : entries )
		{
			delete e.pResource;
		}
		entries.clear();
	}
	// gets the singleton instance (creates if doesn't already exist)
	static Codex& Get()
	{
		static Codex codex;
		return codex;
	}
private:
	std::vector<Entry> entries;
};

class Codx
{
public:
	static const Surface* Load( const std::string& src,const Vei2& expandAmount )
	{
		return( Codex<Surface>::RetrieveSurf( src,expandAmount ) );
	}
};