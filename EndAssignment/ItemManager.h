#pragma once
class Actor;

class ItemManager final
{
public:
	ItemManager();
	ItemManager(const ItemManager& other) = delete;
	ItemManager& operator=(const ItemManager& other) = delete;
	~ItemManager();

	void Update(float elapsedSec);
	void Draw() const;

private:
	std::vector<Actor*> m_pItemVector;
};

