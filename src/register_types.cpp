/**
 * @file register_types.cpp
 * @brief Implementation of register types functions.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2018
 * @version 1
 * @date 2018-02-23
 */


#include "register_types.h"
#include "class_db.h"

#include "Actor.hpp"

void register_antfarm_types()
{
  ClassDB::register_class<Actor>();
}

void unregister_antfarm_types()
{
  // nothing to do
}
