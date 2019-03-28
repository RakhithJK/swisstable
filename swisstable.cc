#include <iostream>
#include <string>
#include <string_view>
#include "absl/container/flat_hash_set.h"
#include "absl/container/flat_hash_map.h"

extern "C" void * swisstable_set_create(void)
{
    return new absl::flat_hash_set<std::string_view>;
}

extern "C" void swisstable_set_free(void *root)
{
    absl::flat_hash_set<std::string_view> *set = (absl::flat_hash_set<std::basic_string_view<char> >*) root;
    delete set;
    return;
}

extern "C" void * swisstable_set_insert(void *root, void *key, size_t keysize)
{
    std::string_view result((char *)(key), keysize);
    absl::flat_hash_set<std::string_view> *set = (absl::flat_hash_set<std::basic_string_view<char> >*) root;
    result = *(set->insert(result).first);
    return const_cast<char *>(result.data());
}

extern "C" void swisstable_set_foreach(void *root, void (*callback)(void *key))
{
    absl::flat_hash_set<std::string_view> *set = (absl::flat_hash_set<std::basic_string_view<char> >*) root;
    for (const auto &p : *set) {
        callback(const_cast<char *>(p.data()));
    }
    return;
}

extern "C" void * swisstable_map_create(void)
{
    return new absl::flat_hash_map<std::string_view, void *>;
}

extern "C" void swisstable_map_free(void *root)
{
    absl::flat_hash_map<std::string_view, void *> *map = (absl::flat_hash_map<std::basic_string_view<char>, void *>*) root;
    delete map;
    return;
}

extern "C" void * swisstable_map_insert(void *root, void *key, size_t keysize, void *value)
{
    absl::flat_hash_map<std::string_view, void *> *map = (absl::flat_hash_map<std::basic_string_view<char>, void *>*) root;
    std::string_view result((char *)(key), keysize);
    result = (*(map->try_emplace(result, value).first)).first;
    return const_cast<char *>(result.data());
}

extern "C" void * swisstable_map_search(void *root, void *key, size_t keysize)
{
    absl::flat_hash_map<std::string_view, void *> *map = (absl::flat_hash_map<std::basic_string_view<char>, void *>*) root;
    std::string_view result((char *)(key), keysize);
    auto search = map->find(result);
    if (search == map->end())
        return NULL;
    return const_cast<void *>((*search).second);
}

extern "C" void swisstable_map_foreach(void *root, void (*callback)(void *key, void *value))
{
    absl::flat_hash_map<std::string_view, void *> *map = (absl::flat_hash_map<std::basic_string_view<char>, void *>*) root;
    for (const auto &p : *map) {
        callback(const_cast<char *>(p.first.data()), const_cast<void *>(p.second));
    }
    return;
}
