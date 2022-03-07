#include "utility.h"

int GetRegionDistance(IntVector2 region)
{
	cube::Game* game = cube::GetGame();
	if (!game)
	{
		return 0;
	}

	IntVector2 base_region = game->GetPlayer()->entity_data.equipment.unk_item.region;
	int xDiv = std::abs(base_region.x - region.x);
	int yDiv = std::abs(base_region.y - region.y);
	return std::max<int>(xDiv, yDiv);
}

int GetItemLevel(cube::Item* item)
{
	if (!item)
	{
		return 1;
	}

	cube::Game* game = cube::GetGame();
	if (!game)
	{
		return item->rarity;
	}

	return 1 + 20 * GetRegionDistance(item->region) + GetLevelVariation(item->modifier, 20);
}

int GetCreatureLevel(cube::Creature* creature)
{
	cube::Game* game = cube::GetGame();
	if (!game)
	{
		return creature->entity_data.level;
	}

	IntVector2 base_region = game->GetPlayer()->entity_data.equipment.unk_item.region;
	IntVector2 current_region = creature->entity_data.current_region;

	// Spawned enemies apparently have no current region and that is still set to 0.
	if (current_region == IntVector2(0, 0))
	{
		// Backup (hacky) method.
		current_region = game->GetPlayer()->entity_data.current_region;
	}

	int distance = GetRegionDistance(current_region);

	switch (creature->entity_data.hostility_type)
	{
	case cube::Creature::EntityBehaviour::Player:
	case cube::Creature::EntityBehaviour::Pet:
		return creature->entity_data.level;
	default:
		// Variation where packs of creatures have the same level
		//return (1 + 5 * distance) + GetLevelVariation(creature->entity_data.race * distance, 5);
		return (1 + 20 * distance) + GetLevelVariation(creature->id, 20);
	}
}

// Can also be done without rands but just as a `modifier % range`
int GetLevelVariation(long long modifier, int range)
{
	std::srand(modifier);
	return std::rand() % range;
}
