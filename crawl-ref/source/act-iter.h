/**
 * @file
 * @brief Provide a way to iterator over all actors, subject to a few common restrictions.
**/

#pragma once

#include "los-type.h"

class actor_near_iterator
{
public:
    actor_near_iterator(coord_def c, los_type los = LOS_DEFAULT);
    actor_near_iterator(const actor* a, los_type los = LOS_DEFAULT);

    operator bool() const;
    actor* operator*() const;
    actor* operator->() const;
    actor_near_iterator& operator++();
    actor_near_iterator operator++(int);

protected:
    const coord_def center;
    los_type _los;
    const actor* viewer;
    int i;

    bool valid(const actor* a) const;
    void advance();
};

class monster_near_iterator
{
public:
    monster_near_iterator(coord_def c, los_type los = LOS_DEFAULT);
    monster_near_iterator(const actor* a, los_type los = LOS_DEFAULT);

    operator bool() const;
    monster* operator*() const;
    monster* operator->() const;
    monster_near_iterator& operator++();
    monster_near_iterator operator++(int);
    bool operator==(const monster_near_iterator &other);
    bool operator!=(const monster_near_iterator &other);
    monster_near_iterator begin();
    monster_near_iterator end();

protected:
    const coord_def center;
    los_type _los;
    const actor* viewer;
    int i;
    int begin_point;

    bool valid(const monster* a) const;
    void advance();
};

class monster_iterator
{
public:
    monster_iterator();

    operator bool() const;
    monster* operator*() const;
    monster* operator->() const;
    monster_iterator& operator++();
    monster_iterator operator++(int);

protected:
    int i;
    void advance();
};
