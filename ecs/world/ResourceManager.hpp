#pragma once
#include<SFML/Graphics.hpp>
#include<map>
#include<memory>
#include<cassert>
#include<stdexcept>
#include<string>
#include<iostream>

template<typename Resource, typename Identifier>
class ResourceHolder
{
public:
        void load(Identifier id, const std::string& filename);

    template<typename Paragram>
        void load(Identifier id, const std::string& filename, const Paragram& SecondArgument);

        Resource& get(Identifier id);
        const Resource& get(Identifier id) const;

private:
    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
    std::unique_ptr<Resource> temp (new Resource);
    if(temp->loadFromFile(filename) == false)
    {
        throw std::runtime_error("ResourceHolder::load() - Failed to load " + filename);
    }
    auto inserted = mResourceMap.insert(std::make_pair(id, std::move(temp)));
    assert(inserted.second);
}

template<typename Resource, typename Identifier>
template<typename Paragram>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename, const Paragram& SecondArgument)
{
    std::unique_ptr<Resource> temp (new Resource);
    if(temp->loadFromFile(filename, SecondArgument) == false)
    {
        throw std::runtime_error("ResourceHolder::load() - Failed to load " + filename);
    }
    auto inserted = mResourceMap.insert(std::make_pair(id, std::move(temp)));
    assert(inserted.second);
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
	auto found = mResourceMap.find(id);
	assert(found != mResourceMap.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
	auto found = mResourceMap.find(id);
	assert(found != mResourceMap.end());

	return *found->second;
}


