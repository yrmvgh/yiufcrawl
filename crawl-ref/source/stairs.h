/**
 * @file
 * @brief Moving between levels.
**/

#pragma once

bool check_annotation_exclusion_warning();
level_id stair_destination(dungeon_feature_type feat, const string &dst,
                           bool for_real = false);
level_id stair_destination(coord_def pos, bool for_real = false);
void floor_transition(dungeon_feature_type how,
                      const dungeon_feature_type whence, level_id whither,
                      bool forced, bool going_up = false, bool shaft = false,
                      bool update_travel_cache = true);
void take_stairs(dungeon_feature_type force_stair = DNGN_UNSEEN,
                 bool going_up = false, bool force_known_shaft = false,
                 bool update_travel_cache = true);
void down_stairs(dungeon_feature_type force_stair = DNGN_UNSEEN,
                 bool force_known_shaft = false, bool update_travel_cache = true);
void up_stairs(dungeon_feature_type force_stair = DNGN_UNSEEN, bool update_travel_cache = true);
void new_level(bool restore = false);
void leaving_level_now(dungeon_feature_type stair_used);
