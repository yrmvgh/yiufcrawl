#pragma once

#include "spl-cast.h"

struct bolt;
class dist;

spret_type conjure_flame(const actor *agent, int pow, const coord_def& where,
                         bool fail);

spret_type cast_poisonous_vapours(int pow, const dist &beam, bool fail);

void big_cloud(cloud_type cl_type, const actor *agent, const coord_def& where,
               int pow, int size, int spread_rate = -1);

spret_type cast_big_c(int pow, spell_type spl, const actor *caster, bolt &beam,
                      bool fail);

spret_type cast_ring_of_flames(int power, bool fail);
void manage_fire_shield(int delay);

spret_type cast_corpse_rot(bool fail);
void corpse_rot(actor* caster);

std::string get_evaporate_result_list(int potion);
spret_type cast_evaporate(int pow, bolt& beem, int potion, bool fail);

void holy_flames(monster* caster, actor* defender);

spret_type cast_cloud_cone(const actor *caster, int pow, const coord_def &pos,
                           bool fail = false);
