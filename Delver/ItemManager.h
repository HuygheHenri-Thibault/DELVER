#pragma once

#include <vector>
#include "Player.h"
class Item;

class ItemManager
{
public:
	ItemManager(const ItemManager& other) = delete;
	ItemManager& operator=(const ItemManager& other) = delete;
	ItemManager(ItemManager&& other) = delete;
	ItemManager& operator=(ItemManager&& other) = delete;
	~ItemManager();

	static ItemManager* GetInstance();

	void AddItem(Item* item);

	void QueueForDestroy(Item* item);

	void UpdateItems(float elapsedSec, Player& player);
	void DrawItems() const;

	size_t GetSize() const;

	void ClearAllOnLevel();

private:
	std::vector<Item*> m_Items;
	std::vector<Item*> m_ItemsToDelete;

	ItemManager();
	void DestroyItems();
};

